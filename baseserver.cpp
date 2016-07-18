#include "baseserver.h"
#include <string.h>

BaseServer::BaseServer()
{
}

void BaseServer::run(const char *address, unsigned short port)
{
    assert(address!=NULL);
    event_base* base = event_base_new();
    memset(&m_sin,0,sizeof(m_sin));
    m_sin.sin_family = AF_INET;
    m_sin.sin_port   = htons(port);
    inet_aton(address,&m_sin.sin_addr);

    m_listener = evconnlistener_new_bind(base,listener_callback,(void*)base,LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE,10,(sockaddr*)&m_sin,sizeof(m_sin));

    assert(m_listener!=NULL);

    event_base_dispatch(base);

    evconnlistener_free(m_listener);
    event_base_free(base);


}

void BaseServer::listener_callback(struct evconnlistener *listener, int fd, struct sockaddr *sa, int socklen, void *arg)
{
    event_base *base = (event_base*)arg;
    struct bufferevent* bev = bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);
    assert(bev);
    bufferevent_setwatermark(bev,EV_READ,0,0);
    bufferevent_setwatermark(bev,EV_WRITE,0,0);

    bufferevent_setcb(bev,read_callback,write_callback,NULL,NULL);
    bufferevent_enable(bev,EV_READ);

}

void BaseServer::read_callback(bufferevent *pBufferEvent, void *user_data)
{

    evbuffer* inputbuf = bufferevent_get_input(pBufferEvent);
    evbuffer* outputbuf = bufferevent_get_output(pBufferEvent);
    size_t t;
    std::string request;
    request=evbuffer_readln(inputbuf,&t,EVBUFFER_EOL_CRLF);
    printf("request= %s\n",request.c_str());
    std::vector<std::string> result;
    result = split(request," ");

    std::string http_method = result[0];
    std::string http_path   = result[1];
    std::string http_version= result[2];

    std::cout<<"method: "<<http_method<<std::endl;
    std::cout<<"path: "<<http_path<<std::endl;
    std::cout<<"http_version: "<<http_version<<std::endl;

    printf("path=%d\n",http_path.size());

    std::string content_type;
    auto pos = http_path.find(".jpg");
    if(std::string::npos==pos)
        content_type = "text/html\r\n";
    else
        content_type = "image/jpg\r\n";

    if(http_path=="/")
    {
        http_path="index.html";
    }
    else
    {
        http_path=http_path.substr(1);

    }


    std::ifstream ifile(http_path.c_str());
    if(ifile)
    {
        std::stringstream stream;
        stream << ifile.rdbuf();
        std::string filecontent(stream.str());
        ifile.close();
        stream.str("");
        stream<<"HTTP/1.1 200 OK\r\n"<<"Content-Type: "<<content_type
             <<"Content-Length: "<<filecontent.size()<<"\r\n\r\n";
        stream<<filecontent;
        std::string response(stream.str());
        stream.str("");
        //std::cout<<"response****"<<response;
        evbuffer_add(outputbuf,response.c_str(),response.size());
    }else
    {

    }




    evbuffer* tmp=evbuffer_new();
    evbuffer_add_buffer(tmp,inputbuf);
    evbuffer_free(tmp);




}

void BaseServer::write_callback(bufferevent *pBufferEvent, void *user_data)
{

}





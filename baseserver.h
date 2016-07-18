#ifndef BASESERVER_H
#define BASESERVER_H

#include "util.h"
#include "tools.h"

class BaseServer
{
public:
    BaseServer();
    void run(const char *address,unsigned short port);
    static void listener_callback(struct evconnlistener *listener,
                                   int fd,
                                   struct sockaddr *sa,
                                   int socklen,
                                   void *arg);
    static void read_callback(struct bufferevent *pBufferEvent, void *user_data);
    static void write_callback(struct bufferevent *pBufferEvent, void *user_data);
    //static void event_callback(struct bufferevent *pBufferEvent, void *user_data);

private:
    //struct event_base *m_base;
    struct evconnlistener *m_listener;
    struct sockaddr_in m_sin;
};

#endif // BASESERVER_H

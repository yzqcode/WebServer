#include "baseserver.h"

int main(int argc, char *argv[])
{
    BaseServer* b = new BaseServer();
    b->run("0.0.0.0",80);
}

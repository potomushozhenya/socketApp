#ifndef PROG2_H
#define PROG2_H
#include <netinet/in.h>
class Server{
    struct sockaddr_in addr;
    int sock, listener;
public:
    Server();
    void recieve();
};
class Prog2:Server{
public:
    Prog2();
};
#endif
#ifndef PROG1_H
#define PROG1_H
#include <mutex>
#include <condition_variable>
#include <netinet/in.h>
class Client{
    struct sockaddr_in addr;
    int sock;
public:
    Client();
    void createConnection();
    int sendData(char *data);
    bool established;
};
class Prog1:Client{
    std::mutex m;
    std::condition_variable data_recieved;
    char *data;
    int dataSize;
public:
    Prog1();
    void t1();
    void t2();
};
#endif
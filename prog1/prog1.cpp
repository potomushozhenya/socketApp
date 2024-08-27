#include "prog1.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <charconv>
#include <thread>
#include <cstring>

Client::Client(){
    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(3425);
    this->addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    this->established = false;
    createConnection();
}
void Client::createConnection(){
    if (this->established)
    {
        close(this->sock);
    }
    this->sock = socket(AF_INET, SOCK_STREAM, 0);
    if(this->sock < 0)
    {
        close(this->sock);
        perror("socket");
        exit(1);
    }
    while(connect(this->sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {   
        sleep(2);
    }
    this->established = true;
}
int Client::sendData(char *data){
    if (this->established && (connect(this->sock, (struct sockaddr *)&addr, sizeof(addr)) < 0))
    {
        createConnection();
    }
    
    return send(this->sock, data, sizeof(data), 0);
}
void Prog1::t1(){
    std::string line;
    while (getline(std::cin, line)){
        std::unique_lock<std::mutex> lock(this->m);
        this->dataSize = line.size();
        char* ptr = new char[this->dataSize];
        this->data = ptr;
        strcpy(ptr, line.data());
        bool isOk = true;
        int even=0;
        if (this->dataSize > 64)
        {
            isOk = false;
        }
        if (isOk)
        {
            for (int i = 0; i < this->dataSize; ++i)
            {
                if (!('0' <= data[i] && data[i] <= '9')){
                    isOk = false;
                    break;
                } else{
                    if ((data[i] - '0')%2==0){
                        ++even;
                    }
                }
            }
        }
        if (isOk){
            std::sort(this->data,this->data + sizeof(char)*(this->dataSize), std::greater<>());
            if (even!=0){
                int newSize = this->dataSize + even;
                char* newData = new char[newSize];
                int c=0;
                for (int i = 0; i < this->dataSize; ++i)
                {
                    if ((data[i] - '0')%2==0){
                        newData[c]='K';
                        newData[c+1]='B';
                        c+=2;
                    } else{
                        newData[c]=this->data[i];
                        ++c;
                    }
                }
                this->data=newData;
                this->dataSize=newSize;
            }
            this->data_recieved.notify_one();
        }
    }
}
void Prog1::t2(){
    while (1){
        std::unique_lock<std::mutex> lock(this->m);
    this->data_recieved.wait(lock, [&] {
      return this->data!=nullptr;
    });
    int c=0;
    for (int i = 0; i < this->dataSize+2; ++i)
    {
        if ('0' <= this->data[i] && this->data[i] <= '9'){
            c+=(this->data[i] - '0');
        }
        std::cout<<this->data[i];
    }
    std::cout<<std::endl;
    this->data=nullptr;
    int len = std::to_string(c).length();
    char* buf = new char[len];
    std::to_chars(buf, buf+len, c);
    sendData(buf);
    delete []buf;
    }
}
Prog1::Prog1():Client(){
    this->data=nullptr;
    std::thread th1(&Prog1::t1, this);
    std::thread th2(&Prog1::t2, this);
    th1.join();
    th2.join();
}
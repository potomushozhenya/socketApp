#include "prog2.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>

Server::Server(){
    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(3425);
    this->addr.sin_addr.s_addr = htonl(INADDR_ANY);
    this->listener = socket(AF_INET, SOCK_STREAM, 0);
    if(this->listener < 0)
    {
        close(this->listener);
        perror("socket");
        exit(1);
    }
    int opt = 1;
    setsockopt(this->listener, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
    bind(this->listener, (struct sockaddr *)&addr, sizeof(addr));
    listen(listener, 4);
}
void Server::recieve(){
    int bytes_read;
    while(1)
    {
        sock = accept(this->listener, NULL, NULL);
        if(sock < 0)
        {
            close(sock);
            perror("accept");
            exit(3);
        }

        while(1)
        {
            char buf[1024];
            bytes_read = recv(sock, buf, 1024, 0);
            if(bytes_read <= 0){
                break;
            } else{
                int i;
                int c = 0;
                while (buf[c]!='\0')
                {
                    ++c;
                }
                sscanf(buf, "%d", &i);
                if (c>2 && i%32==0){
                    std::cout<<"Data recieved"<<std::endl;
                } else{
                    std::cout<<"Error"<<std::endl;
                }
            }
            
        }
    }
}
Prog2::Prog2():Server(){
    this->recieve();
}
#include "server.h"

using namespace std;

Server::Server(int listenport){
    socklen_t adrlen;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    ServerConfig(&servaddr, listenport);
    Reuse(listenfd);
    Bind(listenfd, &servaddr);
    Listen(listenfd,CONNECTIONS);

    connectfd = Accept(listenfd, (struct sockaddr*)&adr, &adrlen);
}

Server::Server(int listenport, int status){

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    connectfd = listenfd;
    ServerConfig(&servaddr, listenport);
    Reuse(listenfd);
    Bind(listenfd, &servaddr);
    Listen(listenfd,CONNECTIONS);
}

int Server::GetSocket(){
    return connectfd;
}

int Server::GetListenFd(){
    return listenfd;
}

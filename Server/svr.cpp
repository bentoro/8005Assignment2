#include "../Socketwrappers/server.h"
#include "../Socketwrappers/client.h"
#include "../Socketwrappers/socketwrappers.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>

#define BUFLEN 1024

void NewData(int fd);
void NewConnection(int socket, fd_set* fd, int* maxfd);

int main(int argc, char *argv[]){
    int maxfd, n, i;
    fd_set read,master;

    Server *server = new Server(7005, 0);
    cout << server->GetSocket() << endl;
    FD_ZERO(&read);
    maxfd = server->GetSocket();

    FD_SET(server->GetListenFd(), &read);

    while(1){
        memcpy(&master, &read, sizeof(read));
        //no timeout set
        n = select(maxfd+1,&master, NULL, NULL, NULL);

        if(n < 0){
            perror("Select error");
            break;
        }

        //file descriptor is ready
        for(i = 0; i <= maxfd; i++){
            //check which file descriptor is ready
            if(FD_ISSET(i, &master)){
                if(i == (server->GetSocket())){
                    NewConnection(server->GetSocket(),&master, &maxfd);

                } else {
                //incoming data on socket
                NewData(i);


                }
            }
        }
    }

    return 0;
}
void CheckSet(fd_set* fd,int newfd, int* maxfd){
    FD_SET(newfd, fd);
    if(newfd > *maxfd){
        *maxfd = newfd;
    }
}


void NewConnection(int socket, fd_set* fd, int* maxfd){
    struct sockaddr_in addr;
    socklen_t addrlen;
    int newfd;
    printf("Listening socket");
    //add all the new clients trying to connect
    if ((newfd = accept(socket, (struct sockaddr*)&addr, &addrlen)) < 0){
        printf("Accept error");
    }
    printf("Adding a new client");
    FD_SET(newfd, fd);
    if(newfd > *maxfd){
        *maxfd = newfd;
    }
}

void NewData(int fd){
  FILE *fp;
  char buffer[BUFLEN];
  int bytesread, write;

  if((fp = fopen("result","w")) == NULL){
    perror("File doesn't exist");
  }
  memset(buffer, '\0', BUFLEN);
  while((bytesread = read(fd, buffer,sizeof(buffer))) < 0){
    if((write = fwrite(buffer, 1, bytesread, fp)) < 0){
      perror("Write failed");
    }
  }
  cout << buffer << endl;
  fclose(fp);
}

/*void SendData(int fd){
  char buffer[BUFLEN];
  int read, sent;

  if(fp = fopen(filename,"rb")) == NULL){
    perror("File doesn't exist");
  }
  memset(buffer, '\0', BUFLEN);
  while((read = fread(buffer, 1, sizeof(buffer), fp))){
    sent = send(fd, buffer, sizeof(buffer),0);
    memset(buffer,'\0',BUFLEN);
  }
  fclose(fp);
}*/

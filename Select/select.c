#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include "select.h"

void createSelectFd(fd_set* fd, int newfd, int* maxfd){
  //add new fd to set, if new fd is larger update
  FD_SET(newfd, fd);
  if(newfd > *maxfd){
      *maxfd = newfd;
  }
}

void waitForSelectEvent(fd_set* fd, int* maxfd){
  int n;
  n = select(maxfd+1,&fd, NULL, NULL, NULL);
        if(n < 0){
            perror("Select error");
        }
}
//not sure what to do with this?
void SingleSelectReadInstance(const int sock, unsigned char *buffer, const size_t bufSize){
  int maxfd;
  FD_ZERO(&read);
  maxfd = sock;
  FD_SET(sock, &read);

  waitForSelectEvent(read, maxfd);
  for(i = 0; i <= maxfd; i++){
            //check which file descriptor is ready
            if(FD_ISSET(i, &read)){
                if(i == (sock)){

                } else {

                }
            }
        }
}

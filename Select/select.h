#ifndef select_h
#define select_h

#include <sys/select.h>

#define MAX_SELECT 20

void createSelectFd(fd_set* fd, int newfd, int* maxfd);
void SingleSelectReadInstance(const int sock, unsigned char *buffer, const size_t bufSize);
void waitForSelectEvent(fd_set* fd, int* maxfd);

#endif

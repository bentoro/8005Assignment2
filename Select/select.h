#ifndef select_h
#define select_h

#include <sys/select.h>

#define MAX_SELECT 20

int createSelectFd(void);
void addSelectSocket();

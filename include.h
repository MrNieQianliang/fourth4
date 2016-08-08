#include<iostream>
#include<errno.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/epoll.h>
#include<netinet/in.h>
#include<sys/resource.h>
#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<cstring>
#include<fstream>

#include"readConWriLog.h"

#define MAXEPOLLSIZE 10000

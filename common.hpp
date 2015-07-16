#ifndef __COMMON_H

#define __COMMON_H

#include <sys/time.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include <getopt.h>
#include <netdb.h>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <stdint.h>

#define RECVBUFLEN 640

using namespace std;

long minInterArrival(long* interArrival, int num);
long maxInterArrival(long* interArrival, int num);
long meanInterArrival(long* interArrival, int num);


#endif

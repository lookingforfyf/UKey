#ifndef _LGN_INCLUDES_H
#define _LGN_INCLUDES_H


#ifdef WIN32
#else // linux
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
//#include <errno.h>
#include <time.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iconv.h>

#if (LGN_SUPPORT_MULTITHREAD)
#include <pthread.h>
#endif

#define LGN_ALG_STATIC_LOAD 1

#endif


#include "lgnbase.h"
#include "lgncoll.h"
#include "lgnsync.h"
#include "lgnfile.h"
#include "lgnencoding.h"
#include "lgnsock.h"
#include "lgngsmem.h"
#if (LGN_SUPPORT_MULTITHREAD)
#include "lgnthread.h"
#endif


#endif // #ifndef _LGN_INCLUDES_H

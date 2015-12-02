/*
 * Custom.h
 *
 * Includes and Defines
 *
 ******************************************************/

#ifndef _custom_h_
#define _custom_h_

/* Normal headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <ctype.h>

/* System headers */
#include <sys/signal.h>
#include <sys/wait.h>

/* Socket headers */
#include <sys/socket.h>
#include <arpa/inet.h>

/* Function Prototypes */
void strip_special (char *str);
void sig_child (int signo);
/* End of Prototypes */

#endif /* End of custom.h */

/*
 * Handle Clients - hdl_client.h
 *
 * Header file for handling clients
 *
 **********************************************/

#ifndef _hdl_client_h_
#define _hdl_client_h_

#include "custom.h"

/* List of defines */
#define WELCOME ">> Welcome <<\n"
#define SEEYOU ">> Goodbye <<\n"
#define SERVER_KILLED ">> Server Killed <<\n"
#define MAGICKEY "SEC_CODE_007"

/* Function Prototypes */
void close_error (char *message, int ret_val, int *sockfd);
int handle_commands (int *sockfd);
int handle_socket (int *sockfd);
/* End of prototypes */

#endif /* End of hdl_client.h */

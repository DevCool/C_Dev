/*
* serv.c
*
* Program utilising sockets - A
* server program.
*
* by Philip R. Simonson
*********************************************************/

/* Headers */

#include "hdl_client.h"
#include "custom.h"

/*
* Function to print usage
*/
void
print_usage (const char *progname)
{
	printf ("Usage:\n\t%s -p [port]\n"
			"-h for more help.\n", progname);
}

/*
* Function to print help
*/
void
print_help ()
{
	printf ("Server 2 - v0.02\n"
		"================================\n"
		"List of options:\n\n"
		"-h           = prints this help message\n"
		"-p [portnum] = change default port number\n"
		"================================\n"
		"End of help!\n\n");
}

/*
* Remove special chars from string function
*/
void
strip_special (char *str)
{
	char result[512];
	int i = 0;

	bzero (result, sizeof (result));
	while (str[i] != '\0')
    {
		if (isalnum (str[i]))
	{
			result[i]=str[i];
		}
      else if ((str[i] == '_') || (str[i] == ' ') || (str[i] == '\"') ||
			(str[i] == ';') || (str[i] == '$') || (str[i] == '%') ||
			(str[i] == '@') || (str[i] == '!') || (str[i] == '#')||
			      (str[i] == '^') || (str[i] == '&') || (str[i] == '*')||
			      (str[i] == '(') || (str[i] == ')') || (str[i] == '-')||
			      (str[i] == '=') || (str[i] == '{') || (str[i] == '}') ||
			      (str[i] == '.'))
	{
			result[i]=str[i];
		}
		i++;
	}

  result[i] = '\0';
  strcpy (str, result);
}

/*
* Handle the child process
*/
void
sig_child (int signo)
{
	pid_t pid;
	int stat;

	pid = wait (&stat);
	if (pid == 0);
}

/*
* Entry Point for program below
*/
int
main (int argc, char *argv[])
{
	int server_socket, client_socket, done, pid;
	socklen_t client_len;
	struct sockaddr_in server, client;
	char client_message[128] = {0};
	short port = 5555;
	int c;

	opterr = 0;
	
	puts ("Custom Server 2\nUse telnet to access it...\n");
	
	while ((c = getopt (argc, argv, "hp:")) != -1)
	{
		switch (c)
		{
		long lnum = 0;
		char *end = 0;

		case 'h':
			print_help();
			return (0);
		break;
		case 'p':
		{
			lnum = strtol (optarg, &end, 10);
			if (end == optarg) {
				close_error ("Cannot convert string to number!", 1, &server_socket);
			}
			if ((lnum == LONG_MAX) || (lnum == LONG_MIN)) {
				close_error ("Error: number out of range!", 2, &server_socket);
			}
			if ((lnum > INT_MAX) || (lnum < INT_MIN)) {
				close_error ("Error: number out of range!", 2, &server_socket);
			}
			port = (short) lnum;
		}
		break;
		default:
			print_usage("server");
			abort();
		}
	}

	/* Prepare the sockaddr_in structure */
	memset (&server, 0, sizeof (server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl (INADDR_ANY);
	server.sin_port = htons (port);

	/* Create the socket */
	server_socket = socket (AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1) {
		close_error ("Cannot create socket", 1, &server_socket);
	}

	puts ("Socket created!");

	/* Bind socket */
	if (bind (server_socket, (struct sockaddr *) &server, sizeof (server)) < 0) {
		close_error("Error: Bind failed!",1,&server_socket);
	}
	puts ("Bind successful!");

	/* Listen for connections */
	listen (server_socket, 5);
	signal (SIGCHLD, sig_child);

	/* Accept any incoming connection */
	puts ("Waiting for incoming connections...");
	done = 0;
	client_len = sizeof (client);

	while (!done) {
		client_socket = accept (server_socket, (struct sockaddr*) &client, &client_len);
		bzero (client_message, sizeof (client_message));
		if (client_socket < 0) {
			puts ("Couldn't accept client.");
		}

		  if(client_socket != 0) {
			    puts ("Client connected!");
			    if (system ("aplay -q ./data/clientc.wav") < 0) {
				      puts ("Cannot play WAV file!");
			    }
		  }

		  pid = fork();
		  if (pid < 0) {
			close_error ("Error on fork()", 1, &client_socket);
		  }

		  if (pid == 0) {
			write (client_socket, "Welcome to the echo server!\n", 28);
			handle_socket (&client_socket);
			if (client_socket != 0) {
				close (client_socket);
				puts ("Client disconnected!");
				if (system ("aplay -q ./data/clientd.wav") < 0) {
						puts ("Cannot play WAV file!");
				}
				fflush (stdout);
			}
			exit (0);
		  }
		  else {
			close (client_socket);
		  }
	}

	close (server_socket);
	return (0);
}


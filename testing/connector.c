/************************************************************
 * Program to get HTTP header info.
 * by Philip "5n4k3" Simonson
 ************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define HTTP_LOCATION 512

struct _HTTPHEADERstruct {
    int result;
    char loc[HTTP_LOCATION];
    char *info;
};
typedef struct _HTTPHEADERstruct HTTPHEADER;

void timer(int sec);
void destroy_headerinfo(HTTPHEADER *header);
void get_httpheader(HTTPHEADER *header, char *data, size_t size);
void get_headerinfo(HTTPHEADER *header);

#define HTTP_REQUEST "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n"

int main(int argc, char **argv) {
    HTTPHEADER header;
    struct addrinfo hints, *servinfo;
    int sockfd;
    char request[BUFSIZ];
    char data[BUFSIZ];
    int bytes, c;

    if(argc != 3) {
        fprintf(stderr, "Usage: %s <hostname> <urlpath>\n", argv[0]);
        return 0;
    }
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if(getaddrinfo(argv[1], "80", &hints, &servinfo) < 0) {
        fprintf(stderr, "Error: getting host by addr.\n");
        return 1;
    }
    if((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        fprintf(stderr, "Error: Cannot create socket.\n");
        return 1;
    }
    if(connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
        fprintf(stderr, "Error: Cannot connect to host %s.\n", argv[1]);
        close(sockfd);
        return 1;
    }
    puts("Connected.");

    memset(request, 0, sizeof(request));
    snprintf(request, sizeof(request), HTTP_REQUEST, argv[2], argv[1]);
    bytes = send(sockfd, request, strlen(request), 0);
    if(bytes < 0) {
        fprintf(stderr, "Error: Couldn't send request.\n");
        close(sockfd);
        return 1;
    } else
        puts("Request sent.");

    memset(data, 0, sizeof(data));
    bytes = recv(sockfd, data, sizeof(data), 0);
    if(bytes < 0) {
        fprintf(stderr, "Error: Couldn't receive data.\n");
        close(sockfd);
        return 1;
    } else
        puts("Request received.");

    printf("Did you want to see the received transmission (Y/N)? ");
    scanf("%c", &c);
    fflush(stdin);
    if(c == 'y' || c == 'Y') {
        get_httpheader(&header, data, sizeof(data));
        printf("Domain requested data below...\n\n%s\n\nProcessing data...\n",
                header.info);
        get_headerinfo(&header);
        printf("Response: %d\nLocation: %s\n\n", header.result,
                header.loc);
        destroy_headerinfo(&header);
    } else {
        puts("You didn't want to see the requested data?");
    }
    
    puts("Disconnected.");
    close(sockfd);
    return 0;
}

void destroy_headerinfo(HTTPHEADER *header) {
    if(header == NULL)
        return;
    free(header->info);
    memset(header->loc, 0, sizeof(HTTP_LOCATION));
    header->result = 0;
}

void get_httpheader(HTTPHEADER *header, char *data, size_t size) {
    char *pos;
    size_t len;

    if(data == NULL)
        return;
    memset(header, 0, sizeof(HTTPHEADER));
    data[size] = 0;
    pos = strstr(data, "\r\n\r\n");
    if(pos == NULL)
        return;
    header->info = malloc((pos-data)+1);
    memcpy(header->info, data, pos-data);
    header->info[pos-data] = 0;
}

void get_headerinfo(HTTPHEADER *header) {
    sscanf(header->info, "HTTP/1.1 %d %*[^\r]\r\nLocation: %s\r\n",
            &header->result, header->loc);
}

void timer(int sec) {
    clock_t start = clock();

    while(1) {
        int secs = (clock()-start) / CLOCKS_PER_SEC;
        if(secs >= 5)
            break;
    }
}

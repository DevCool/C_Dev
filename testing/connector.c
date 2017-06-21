/************************************************************
 * Program to get HTTP header info.
 * by Philip "5n4k3" Simonson
 ************************************************************
 */


/* standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>

/* linux network headers */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define CHUNK_SIZE 256
#define HTTP_LOCATION 512

/* My HTTP header struct */
struct _HTTPHEADERstruct {
    int info_size;
    int result;
    char loc[HTTP_LOCATION];
    char *data;
    char *info;
};
typedef struct _HTTPHEADERstruct HTTPHEADER;

/* My function prototypes */
char *get_httpdata(int sockfd, size_t *total);
void destroy_headerinfo(HTTPHEADER *header);
void get_httpheader(HTTPHEADER *header, char *data, size_t size);
void get_headerinfo(HTTPHEADER *header);
void timer(int sec);


/* The request string you have to send to a HTTP server */
#define HTTP_REQUEST "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n"

/* main() - entry point for program.
 */
int main(int argc, char **argv) {
    HTTPHEADER header;
    struct addrinfo hints, *servinfo;
    int sockfd;
    char request[BUFSIZ];
    char *data;
    size_t total_bytes;
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

    if((data = get_httpdata(sockfd, &total_bytes)) == NULL) {
        close(sockfd);
        return 1;
    }

    printf("Did you want to see the received transmission (Y/N)? ");
    scanf("%c", &c);
    fflush(stdin);
    if(c == 'y' || c == 'Y') {
        get_httpheader(&header, data, total_bytes);
        printf("Domain requested data below...\n\n%s\n\nProcessing data...\n",
                header.info);
        get_headerinfo(&header);
        printf("Response: %d\nLocation: %s\n\n", header.result,
                header.loc);
        printf("Data below...\n%s\n", header.data);
        destroy_headerinfo(&header);
    } else {
        puts("You didn't want to see the requested data?");
    }
    
    puts("Disconnected.");
    free(data);
    close(sockfd);
    return 0;
}

/* get_httpdata() - function to get website data.
 */
char *get_httpdata(int sockfd, size_t *total) {
    char buffer[CHUNK_SIZE];
    char *data = NULL;
    size_t size = 0;
    size_t total_bytes = 0;
    size_t bytes;

    while((bytes = recv(sockfd, buffer, CHUNK_SIZE, 0)) > 0) {
        if(total_bytes >= size) {
            size += CHUNK_SIZE;
            data = realloc(data, size);
            if(data == NULL)
                return NULL;
        }
        memcpy(&data[total_bytes], buffer, bytes);
        total_bytes += bytes;
    }

    if(bytes == 0) {
        printf("Transfer of %u bytes received successfully.\n", total_bytes);
        *total = total_bytes;
        return data;
    } else {
        fprintf(stderr, "Transfer of %u bytes failed.\n", bytes);
        free(data);
    }
    return NULL;
}

/* destroy_headerinfo() - destroys the http header info structure.
 */
void destroy_headerinfo(HTTPHEADER *header) {
    if(header == NULL)
        return;
    free(header->info);
    free(header->data);
    memset(header->loc, 0, sizeof(HTTP_LOCATION));
    header->result = 0;
    header->info_size = 0;
}

/* get_httpheader() - strips the header out of the data.
 */
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
    header->info_size = pos-data+2;
    header->data = malloc((size-(pos-data))+1);
    memcpy(header->data, &data[header->info_size], size-(pos-data));
    header->data[(size-(pos-data))+1] = 0;
}

/* get_headerinfo() - strips out the http response and location if
 * one exists.
 */
void get_headerinfo(HTTPHEADER *header) {
    char *tok;
    char found = 0;

    tok = strtok(header->info, "\r\n");
    if(strncmp(tok, "HTTP/", 5) == 0)
        sscanf(tok, "HTTP/1.1 %d %*[^\r]\r\n", &header->result);

    while(tok != NULL) {
        tok = strtok(NULL, "\r\n");
        if(tok == NULL)
            return;
        if(strncmp(tok, "Location:", 9) == 0) {
            sscanf(tok, "Location: %s\r\n", header->loc);
            found = 1;
        }
    }

    if(!found)
        snprintf(header->loc, 5, "None");
}

/* timer() - function for a simple timer.
 */
void timer(int sec) {
    clock_t start = clock();

    while(1) {
        int secs = (clock()-start) / CLOCKS_PER_SEC;
        if(secs >= 5)
            break;
    }
}

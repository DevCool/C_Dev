/************************************************************
 * Program     : HTTP Downloader
 * Description : Program to get HTTP header info.
 * Version     : 0.04
 ************************************************************
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
#define HTTP_INFOSIZE 2048
#define HTTP_LOCATION 512
#define HTTP_CTYPE 256
#define HTTP_PROTO 6

/* My Found enumeration */
enum _HTTPFOUND {
    FOUND_LOC,
    FOUND_CTYPE,
    FOUND_COUNT
};

/* My HTTP header struct */
struct _HTTPHEADERstruct {
    int info_size;
    float version;
    int result;
    char proto[HTTP_PROTO];
    char loc[HTTP_LOCATION];
    char ctype[HTTP_CTYPE];
    char *data;
    char *info;
};
typedef struct _HTTPHEADERstruct HTTPHEADER;

/* My http function prototypes */
int handle_redirect(HTTPHEADER *header);
char *get_httpdata(int sockfd, size_t *total);
size_t http_download(int sockfd, const char *uri, const char *domain);

/* My info header function prototypes */
HTTPHEADER setup_headerinfo(void);
void destroy_headerinfo(HTTPHEADER *header);
void get_httpheader(HTTPHEADER *header, char *data, size_t size);
void get_headerinfo(HTTPHEADER *header);

/* My misc function prototypes */
int create_conn(const char *domain);
int get_filename(const char *path, char *fname, char *ext);
void timer(int sec);
void clear_filebuffer(FILE *fp);
size_t file_download(int sockfd, FILE *fout);


/* The request string you have to send to a HTTP server */
#define HTTP_REQUEST "GET %s HTTP/1.0\r\nHost: %s\r\n\r\n"

/* main() - entry point for program.
 */
int main(int argc, char **argv) {
    HTTPHEADER header;
    struct addrinfo hints, *servinfo;
    int sockfd, newsockfd;
    char request[BUFSIZ];
    char *data;
    size_t total_bytes;
    int bytes, c;
    int res;

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
    freeaddrinfo(servinfo);
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

    header = setup_headerinfo();
    if((data = get_httpdata(sockfd, &total_bytes)) == NULL) {
        fprintf(stderr, "Cannot get website data.\n");
        close(sockfd);
        return 1;
    }

    get_httpheader(&header, data, total_bytes);
    get_headerinfo(&header);
#if !defined(NDEBUG)
    printf("Website Info...\n===================\n%s\n===================\n",
            header.info);
    printf("Version: %.1f\nResult: %d\nLocation: %s\n",
            header.version, header.result, header.loc);
#endif

    while((header.result == 301 || header.result == 302)
            && header.result != 404) {
        if((sockfd = handle_redirect(&header)) < 0) {
            free(data);
            destroy_headerinfo(&header);
            return 1;
        } else {
            char domain[BUFSIZ];
            char uripath[BUFSIZ];
            /* clear variables for new location storage */
            memset(request, 0, sizeof(request));
            memset(uripath, 0, sizeof(uripath));
            free(data);
            sscanf(header.loc, "http://%[^/]%s", domain, uripath);
            destroy_headerinfo(&header);
            snprintf(request, sizeof(request), HTTP_REQUEST, uripath, domain);
            bytes = send(sockfd, request, strlen(request), 0);
            if(bytes < 0) {
                fprintf(stderr, "Cannot send HTTP request to site.\n");
                close(sockfd);
                return 1;
            }
            data = get_httpdata(sockfd, &total_bytes);
            if(data == NULL) {
                fprintf(stderr, "Cannot get website data.\n");
                close(sockfd);
                return 1;
            } else {
                header = setup_headerinfo();
                get_httpheader(&header, data, total_bytes);
                get_headerinfo(&header);

#if !defined(NDEBUG)
                printf("Website Info...\n====================\n%s\n"\
                        "=====================\n", header.info);
                printf("Version: %.1f\nResult: %d\nLocation: %s\n",
                        header.version, header.result, header.loc);
#endif
            }
        }
    }

    printf("Did you want to see the received transmission (Y/N)? ");
    scanf("%c", &c);
    clear_filebuffer(stdin);
    if(c == 'y' || c == 'Y') {
        printf("Domain requested data below...\n\n%s\n\nProcessing data...\n",
                header.info);
        printf("Response: %d\nLocation: %s\n\n", header.result,
                header.loc);
    } else {
        puts("You didn't want to see the requested data?");
    }
  
    if(header.result != 404) {
        printf("Did you want to download the file (Y/N)? ");
        scanf("%c", &c);
        clear_filebuffer(stdin);
        if(c == 'y' || c == 'Y') {
            total_bytes = http_download(sockfd, argv[2], argv[1]);
        }
    }
    puts("Disconnected.");
    destroy_headerinfo(&header);
    if(data != NULL)
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
            size += bytes;
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

/* handle_redirect() - handles HTTP redirection.
 */
int handle_redirect(HTTPHEADER *header) {
    struct addrinfo hints, *servinfo = NULL;
    char domain[256];
    char uripath[1024];
    int sockfd, res;

    memset(&hints, 0, sizeof(hints));
    memset(domain, 0, sizeof(domain));
    memset(uripath, 0, sizeof(uripath));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    
    if((res = sscanf(header->loc, "http://%[^/]%s", domain, uripath)) != 2) {
        fprintf(stderr, "Error: Could not seperate domain and uripath.\n");
        return -1;
    }
    if(getaddrinfo(domain, "80", &hints, &servinfo) < 0) {
        fprintf(stderr, "Could not get addr info.\n");
        return -1;
    }
    if((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        fprintf(stderr, "Could not open a new socket.\n");
        return -1;
    }
    if(connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
        fprintf(stderr, "Could not connect to new server.\n");
        return -1;
    }
    freeaddrinfo(servinfo);

    puts("Connected.");
    return sockfd;
}

/* create_conn() - creates a new connection to the requested host.
 */
int create_conn(const char *domain) {
    struct addrinfo hints, *servinfo;
    int sockfd;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if(getaddrinfo(domain, "80", &hints, &servinfo) < 0) {
        fprintf(stderr, "Error: Cannot get address information.\n");
        return -1;
    }
    if((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        fprintf(stderr, "Error: Could not create a socket.\n");
        return -1;
    }
    if(connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
        fprintf(stderr, "Error: Could not connect to server.\n"\
                "Host might be unreachable...\n");
        return -1;
    }
    freeaddrinfo(servinfo);
    puts("Connected");
    return sockfd;
}

/* get_filename() - gets filename from a full path.
 */
int get_filename(const char *path, char *fname, char *ext) {
    char found = 0;
    char *tmp, *dot;
    const char *end;

    end = path;
    while(*end++ != 0);
    if((tmp = strrchr(path, '/')) != NULL)
        found = 1;
    if((dot = strrchr(path, '.')) != NULL) {
        if(found)
            strncpy(fname, &tmp[1], dot-tmp-1);
        else
            strncpy(fname, path, dot-path);
        strncpy(ext, &(*(dot+1)), end-dot);
    } else {
        return 1;
    }

    return 0;
}

/* http_download() - content-type not text/plain or text/html then
 * download the content.
 */
size_t http_download(int sockfd, const char *uri, const char *domain) {
    FILE *file = NULL;
    char filename[261];
    char fname[256];
    char ext[5];
    char request[512];
    size_t total_bytes;
    int bytes, res;

    memset(filename, 0, sizeof(filename));
    memset(fname, 0, sizeof(fname));
    memset(ext, 0, sizeof(ext));
    memset(request, 0, sizeof(request));
    res = get_filename(uri, fname, ext);
    if(res != 0) {
        fprintf(stderr, "Error: Could not get file name.\n");
        return -1;
    }
    printf("Filename: %s.%s\n", fname, ext);
    snprintf(filename, sizeof(filename), "%s.%s", fname, ext);
    snprintf(request, sizeof(request), HTTP_REQUEST, uri, domain);

    if((sockfd = create_conn(domain)) < 0) {
        fprintf(stderr, "Error: Could not reconnect to host.\n");
        return -1;
    }
    bytes = send(sockfd, request, strlen(request), 0);
    if(bytes < 0) {
        fprintf(stderr, "Error: Sending request string.\n");
        return -1;
    }

    if((file = fopen(filename, "wb")) == NULL) {
        fprintf(stderr, "Error: Cannot open file for writing.\n");
        return -1;
    }
    total_bytes = file_download(sockfd, file);
    if(total_bytes < 0) {
        fprintf(stderr, "Error: Could not download the file.\n");
        fclose(file);
        return -1;
    } else {
        printf("Total bytes downloaded: %lu\nFile download was succesful!\n",
            total_bytes);
    }
    fclose(file);
    return total_bytes;
}

/* setup_headerinfo() - initializes the http header info structure.
 */
HTTPHEADER setup_headerinfo(void) {
    HTTPHEADER header;
    memset(&header, 0, sizeof(HTTPHEADER));
    header.info = NULL;
    header.data = NULL;
    strncpy(header.proto, "http", 5);
    return header;
}

/* destroy_headerinfo() - destroys the http header info structure.
 */
void destroy_headerinfo(HTTPHEADER *header) {
    if(header == NULL)
        return;
    if(header->info != NULL)
        free(header->info);
    if(header->data != NULL)
        free(header->data);
    memset(header, 0, sizeof(HTTPHEADER));
}

/* get_httpheader() - strips the header out of the data.
 */
void get_httpheader(HTTPHEADER *header, char *data, size_t size) {
    char *info = NULL;

    if(data == NULL)
        return;
    data[size] = 0;
    info = strstr(data, "\r\n\r\n");
    header->info_size = info-data;
    header->info = malloc(header->info_size+1);
    memcpy(header->info, data, header->info_size);
    header->info[header->info_size+1] = 0;
    header->data = malloc((size-header->info_size)+1);
    memcpy(header->data, &data[header->info_size],
            size-header->info_size);
    header->data[(size-header->info_size)+1] = 0;
}

/* get_headerinfo() - strips out the http response and location if
 * one exists.
 */
void get_headerinfo(HTTPHEADER *header) {
    char info[header->info_size];
    char *tok = NULL;
    char found[FOUND_COUNT];

    strncpy(info, header->info, sizeof(info));
    tok = strtok(info, "\r\n");
    if(strncmp(tok, "HTTP/", 5) == 0)
        sscanf(tok, "HTTP/%f %d %*[^\r]\r\n",
                &header->version, &header->result);

    while(tok != NULL) {
        if(strncmp(tok, "Location:", 9) == 0) {
            if(sscanf(tok, "Location: %s\r\n", header->loc) != 1)
                found[FOUND_LOC] = 0;
            else
                found[FOUND_LOC] = 1;
        }
        if(strncmp(tok, "Content-Type:", 13) == 0) {
            if(sscanf(tok, "Content-Type: %s\r\n", header->ctype) != 1)
                found[FOUND_CTYPE] = 0;
            else
                found[FOUND_CTYPE] = 1;
        }
        tok = strtok(NULL, "\r\n");
    }

    if(!found[FOUND_LOC])
        snprintf(header->loc, HTTP_LOCATION, "None");
    if(!found[FOUND_CTYPE])
        snprintf(header->ctype, HTTP_CTYPE, "Not Found");
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

/* clear_filebuffer() - function to clear input buffer.
 */
void clear_filebuffer(FILE *fp) {
    int c;
    while((c = getc(stdin)) != EOF && c != 0x0A);
}

/* file_download() - function to download a file.
 */
size_t file_download(int sockfd, FILE *fout) {
    char data[CHUNK_SIZE];
    int bytesRead, bytesWritten;
    static size_t total_bytes = 0;

    bytesRead = read(sockfd, data, sizeof(data));
    if(bytesRead > 0)
        bytesWritten = fwrite(data, 1, bytesRead, fout);
    if(bytesWritten == bytesRead)
        return total_bytes;
    else
        total_bytes += bytesWritten;
    file_download(sockfd, fout);
}

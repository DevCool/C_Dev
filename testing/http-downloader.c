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
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>

/* linux network headers */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define ERROR(M, ...) {\
    fprintf(stderr, "[ERROR] : M\n", ##__VA_ARGS__);\
    goto error; errno = 0; exit(errno); }

#define CHUNK_SIZE 512
#define HTTP_LOCATION 512
#define HTTP_CTYPE 256
#define HTTP_PROTO 6
#define HTTP_URLSIZE 512
#define HTTP_INFOSIZE 2048

/* My Found enumeration */
enum _HTTPFOUND {
    FOUND_LOC,
    FOUND_CTYPE,
    FOUND_DSIZE,
    FOUND_COUNT
};

/* My HTTP header struct */
struct _HTTPHEADERstruct {
    size_t info_size;
    size_t data_size;
    float version;
    int result;
    unsigned char set;
    char info[HTTP_INFOSIZE];
    char domain[HTTP_URLSIZE];
    char uri[HTTP_URLSIZE];
    char proto[HTTP_PROTO];
    char loc[HTTP_LOCATION];
    char ctype[HTTP_CTYPE];
};
typedef struct _HTTPHEADERstruct HTTPHEADER;

/* My http function prototypes */
int handle_redirect(HTTPHEADER *header);
size_t get_httpdata(int sockfd, char *data, size_t total_bytes);
size_t http_download(HTTPHEADER *header, const char *uri, const char *domain);

/* My info header function prototypes */
HTTPHEADER setup_headerinfo(void);
void destroy_headerinfo(HTTPHEADER *header);
void get_httpheader(HTTPHEADER *header, char *data, size_t data_size);
void get_headerinfo(HTTPHEADER *header);
int get_urlinfo(HTTPHEADER *header);

/* My misc function prototypes */
int create_conn(const char *domain);
int get_filename(const char *path, char *fname, char *ext);
void timer(int sec);
void clear_filebuffer(FILE *fp);
size_t file_download(int sockfd, HTTPHEADER *header, FILE *fout);
size_t file_copy(FILE *fin, HTTPHEADER *header, FILE *fout);


/* The request string you have to send to a HTTP server */
#define HTTP_REQUEST "GET %s HTTP/1.0\r\nHost: %s\r\n\r\n"

/* main() - entry point for program.
 */
int main(int argc, char **argv) {
    HTTPHEADER header;
    struct addrinfo hints, *servinfo;
    int sockfd, newsockfd;
    char request[BUFSIZ];
    char data[HTTP_INFOSIZE];
    size_t total_bytes;
    int bytes, c;
    int res;

    if(argc != 3) {
        fprintf(stderr, "Usage: %s <hostname> <uripath>\n", argv[0]);
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
        ERROR("Cannot connect to host.");
    }
    freeaddrinfo(servinfo);
    puts("Connected.");

    memset(request, 0, sizeof(request));
    snprintf(request, sizeof(request), HTTP_REQUEST, argv[2], argv[1]);
    bytes = send(sockfd, request, strlen(request), 0);
    if(bytes < 0) {
        ERROR("Cannot send request.");
    } else {
        puts("Request sent.");
    }

    header = setup_headerinfo();
    if((total_bytes = get_httpdata(sockfd, data, HTTP_INFOSIZE)) < 0)
        ERROR("Cannot get website data.");

    get_httpheader(&header, data, total_bytes);
    get_headerinfo(&header);
#if !defined(NDEBUG)
    printf("Website Info...\n===================\n%s\n===================\n",
            header.info);
    printf("Version: %.1f\nResult: %d\nLocation: %s\n",
            header.version, header.result, header.loc);
#endif
    if(header.result == 404)
        ERROR("Uri path not found on server.");

    while(header.result == 301 || header.result == 302) {
        sockfd = handle_redirect(&header);
        if(sockfd < 0) {
            ERROR("Socket was not created.");
        } else {
            if((total_bytes = get_httpdata(sockfd, data, HTTP_INFOSIZE)) < 0) {
                ERROR("Cannot get website data.");
            } else {
                get_httpheader(&header, data, total_bytes);
                get_headerinfo(&header);

#if !defined(NDEBUG)
                printf("Website Info...\n====================\n%s\n"\
                        "=====================\n", header.info);
                printf("Version: %.1f\nResult: %d\nLocation: %s\n",
                        header.version, header.result, header.loc);
#endif
                if(header.result == 404) {
                    puts("Uri path not found on server.");
                    break;
                }
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
    close(sockfd);
    puts("Disconnected.");
   
    if(header.result != 404) {
        printf("Did you want to download the file (Y/N)? ");
        scanf("%c", &c);
        clear_filebuffer(stdin);
        if(c == 'y' || c == 'Y') {
            if(strncmp(header.loc, "None", 4) == 0) {
                total_bytes = http_download(&header, argv[2], argv[1]);
            } else {
                get_urlinfo(&header);
#if !defined(NDEBUG)
                printf("Result: %d\nLocation: %s\n=======\n"
                        "Url     : http://%s%s\n", header.result,
                        header.loc, header.domain, header.uri);
#endif
                total_bytes = http_download(&header, header.uri, header.domain);
            }
        }
    }
    if(header.set == 1)
        destroy_headerinfo(&header);
    return EXIT_SUCCESS;

error:
    if(header.set == 1)
        destroy_headerinfo(&header);
    if(sockfd > 0)
        close(sockfd);
    return EXIT_FAILURE;
}

/* get_httpdata() - function to get website data.
 */
size_t get_httpdata(int sockfd, char *data, size_t data_size) {
    char buffer[CHUNK_SIZE];
    size_t size = 0;
    size_t total_bytes = 0;
    size_t bytes;

    while((bytes = recv(sockfd, buffer, CHUNK_SIZE, 0)) > 0) {
        memcpy(&data[total_bytes], buffer, bytes);
        total_bytes += bytes;
        if(total_bytes >= HTTP_INFOSIZE)
            break;
    }

    if(bytes == 0) {
        printf("Transfer of %u bytes received successfully.\n", total_bytes);
        return total_bytes;
    } else {
        fprintf(stderr, "Transfer of %u bytes failed.\n", bytes);
    }
    return -1;
}

/* handle_redirect() - handles HTTP redirection.
 */
int handle_redirect(HTTPHEADER *header) {
    char request[1024];
    int sockfd, res;

    memset(request, 0, sizeof(request));

    if(get_urlinfo(header) < 0)
        return -1;
    sockfd = create_conn(header->domain);
    if(sockfd < 0)
        return -1;
    snprintf(request, sizeof(request), HTTP_REQUEST, header->uri, header->domain);
    res = send(sockfd, request, strlen(request), 0);
    if(res < 0) {
        puts("Failed to send redirection request.");
        close(sockfd);
        return -1;
    }
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
size_t http_download(HTTPHEADER *header, const char *uri, const char *domain) {
    FILE *file = NULL, *tmp = NULL;
    char filename[261];
    char fname[256];
    char ext[5];
    char request[512];
    size_t total_bytes;
    int bytes, res, sockfd;

    memset(filename, 0, sizeof(filename));
    memset(fname, 0, sizeof(fname));
    memset(ext, 0, sizeof(ext));
    memset(request, 0, sizeof(request));
    res = get_filename(uri, fname, ext);
    if(res != 0) {
        fprintf(stderr, "Error: Could not get file name.\n");
        return -1;
    }
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

    if((tmp = fopen("tmpfile.dat", "wb")) == NULL) {
        fprintf(stderr, "Error: Cannot open file for writing.\n");
        return -1;
    }
    total_bytes = file_download(sockfd, header, tmp);
    if(total_bytes < 0) {
        fprintf(stderr, "Error: Could not download the file.\n");
        fclose(file);
        return -1;
    } else {
        printf("Total bytes downloaded: %lu\nFile download was succesful!\n",
            total_bytes);
    }
    close(sockfd);
    puts("Disconnected!");
    if((tmp = freopen("tmpfile.dat", "rb", tmp)) == NULL) {
        fprintf(stderr, "Error: cannot open tmp file for reading.\n");
        return -1;
    }
    if((file = fopen(filename, "wb")) == NULL) {
        fprintf(stderr, "Error: Cannot open %s for writing.\n", filename);
        fclose(tmp);
        return -1;
    }
    total_bytes = file_copy(tmp, header, file);
    if(total_bytes < 0) {
        fprintf(stderr, "Error: Could not write the file.\n");
        fclose(tmp);
        fclose(file);
        return -1;
    }
    fclose(tmp);
    fclose(file);
    remove("tmpfile.dat");
    return total_bytes;
}

/* setup_headerinfo() - initializes the http header info structure.
 */
HTTPHEADER setup_headerinfo(void) {
    HTTPHEADER header;
    memset(&header, 0, sizeof(HTTPHEADER));
    header.info_size = 0;
    header.data_size = 0;
    header.set = 0;
    strncpy(header.proto, "http", 5);
    return header;
}

/* destroy_headerinfo() - destroys the http header info structure.
 */
void destroy_headerinfo(HTTPHEADER *header) {
    if(header == NULL)
        return;
    memset(header, 0, sizeof(HTTPHEADER));
}

/* get_httpheader() - strips the header out of the data.
 */
void get_httpheader(HTTPHEADER *header, char *data, size_t data_size) {
    char *tmp = NULL, *info = data;

    tmp = strstr(data, "\r\n\r\n");
    data[tmp-info] = 0;
    header->info_size = strlen(data);
    memcpy(header->info, data, tmp-info);
    header->set = 1;
}

/* get_headerinfo() - strips out the http response and location if
 * one exists.
 */
void get_headerinfo(HTTPHEADER *header) {
    char info[header->info_size];
    char *tok = NULL;
    char found[FOUND_COUNT] = {0};

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
        if(strncmp(tok, "Content-Length:", 15) == 0) {
            if(sscanf(tok, "Content-Length: %lu\r\n", &header->data_size) != 1)
                found[FOUND_DSIZE] = 0;
            else
                found[FOUND_DSIZE] = 1;
        }
        tok = strtok(NULL, "\r\n");
    }

    if(!found[FOUND_LOC])
        snprintf(header->loc, HTTP_LOCATION, "None");
    if(!found[FOUND_CTYPE])
        snprintf(header->ctype, HTTP_CTYPE, "Not Found");
    if(!found[FOUND_DSIZE])
        header->data_size = 0;
}

/* get_urlinfo() - function to get the url from header location.
 */
int get_urlinfo(HTTPHEADER *header) {
    if(sscanf(header->loc, "http://%[^/]%s", header->domain, header->uri) != 2) {
        fprintf(stderr, "Error: Couldn't extract uri and domain from url.\n");
        return -1;
    }
    return 0;
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
size_t file_download(int sockfd, HTTPHEADER *header, FILE *fout) {
    char data[CHUNK_SIZE];
    int bytesRead, bytesWritten;
    static size_t total_bytes = 0;
    clock_t start = clock();

    while((bytesRead = read(sockfd, data, sizeof(data))) > 0) {
        bytesWritten = fwrite(data, 1, bytesRead, fout);
        if(bytesWritten == bytesRead)
            total_bytes += bytesWritten;
        if(((clock()-start) / CLOCKS_PER_SEC) >= 5) {
            printf("Total bytes downloaded: %lu/%lu\n", total_bytes,
                    header->data_size);
            start = clock();
        }
    }
    if(bytesRead == 0)
        return total_bytes;
    return -1;
}

/* file_copy() - function to copy a file without HTTP header info.
 */
size_t file_copy(FILE *fin, HTTPHEADER *header, FILE *fout) {
    char data[CHUNK_SIZE];
    int bytesRead, bytesWritten;
    size_t total_bytes = 0;

    fseek(fin, header->info_size+4, SEEK_SET);
    while((bytesRead = fread(data, 1, sizeof(data), fin)) > 0) {
        bytesWritten = fwrite(data, 1, bytesRead, fout);
        if(bytesWritten == bytesRead)
            total_bytes += bytesWritten;
    }
    if(bytesRead == 0)
        return total_bytes;
    return -1;
}

#ifndef transfer_h
#define transfer_h

int download(const char *hostname, const char *filename);
int upload(const char *hostname, const char *filename);
int hdl_download(int *sockfd, struct sockaddr_in *client, const char *filename);
int hdl_upload(int *sockfd, struct sockaddr_in *client, const char *filename);

#endif

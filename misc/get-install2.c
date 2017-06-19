/*
 *************************************************************
 * get-install.c - Program to get a specific url and download
 * that whatever the URL contains.
 * by 5n4k3
 *************************************************************
 */

#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* function prototypes */
int dl_url(const char *host, char *path);
int get_line(char *s, int size);
char* strip_line(char *s, int size);

/* main() - entry point for all programs.
 */
int main(int argc, char **argv) {
	char buffer[BUFSIZ];
	char data[2][BUFSIZ];
	char *tmp[BUFSIZ];
	char *msgs[2] = {"hostname","url path"};
	int i;

	for(i = 0; i < 2; i++) {
		printf("Enter %s: ", msgs[i]);
		get_line(data[i], sizeof(data[i]));
	}
	for(i = 0; i < 2; i++) {
		tmp[i] = strip_line(data[i], BUFSIZ);
		printf("tmp %d: %s\n", i, tmp[i]);
	}
	return dl_url(tmp[0], tmp[1]); /* return success */
}

/* get_line() - gets a string from the user.
 */
int get_line(char *s, int size) {
	int c, i, j;

	j = i = 0;
	while((c = getchar()) != EOF && c != 0x0A)
		if(i < size-2) {
			if(c == 0x08) {	/* backspace found */
				*(s+i) = 0;
				--i;
				--j;
				continue;
			}
			*(s+i) = c;
			++i;
			++j;
		}
	if(c == 0x0A) {
		*(s+i) = c;
		++i;
		++j;
	}
	*(s+j) = 0;
	++j;
	return i;	/* return length of string */
}

/* strip_line() - strips off the line endings.
 */
char *strip_line(char *s, int size) {
	char *tmp = s;
	if((tmp = strchr(s, '\n')) != NULL)
		*tmp = 0x00;
	else {
		fprintf(stderr, "Error: Input to long for buffer.\n");
		return NULL;
	}
	return s;	/* return new char array */
}

/* defined HTTP header */
#define HTTP_HEADER "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n"

struct BUFFERSTRUCT {
	char *data;
	size_t size;
};

/* writebuf() - writes data to BUFFERSTRUCT
 */
static size_t writebuf(void *ptr, size_t size, size_t nmemb, void *data) {
	size_t realsize = size * nmemb;
	struct BUFFERSTRUCT *mem = (struct BUFFERSTRUCT *)data;
	mem->data = realloc(mem->data, mem->size + realsize);
	if(mem->data) {
		memcpy(&(mem->data[mem->size]), ptr, realsize);
		mem->size += realsize;
	}
	return realsize;
}

/* dl_url() - downloads a file from a URL.
 */
int dl_url(const char *host, char *path) {
	FILE *file;
	CURL *myHandle;
	CURLcode result;
	struct BUFFERSTRUCT output;
	char url[BUFSIZ];
	char fname[BUFSIZ];
	char *tmp, *savtok;
	int bytes;

	/* init libcurl */
	bytes = 0;
	output.data = NULL;
	output.size = 0;
	curl_global_init(CURL_GLOBAL_ALL);
	puts("Curl initializing...");
	if((myHandle = curl_easy_init()) == NULL) {
		fprintf(stderr, "Error: Could not init cURL.\n");
		return 1;
	}
	puts("Initialized!");

	/* get remote file */
	memset(url, 0, sizeof(url));
	if(strchr(path, '/') != 0)
		snprintf(url, sizeof(url), "http://%s%s", host, path);
	else
		snprintf(url, sizeof(url), "http://%s/%s", host, path);
	curl_easy_setopt(myHandle, CURLOPT_WRITEFUNCTION, writebuf);
	curl_easy_setopt(myHandle, CURLOPT_WRITEDATA, (void *)&output);
	curl_easy_setopt(myHandle, CURLOPT_URL, url);
	curl_easy_setopt(myHandle, CURLOPT_FOLLOWLOCATION, 1L);
	result = curl_easy_perform(myHandle);
	curl_easy_cleanup(myHandle);

	/* write output to a file */
	memset(fname, 0, sizeof(fname));
	tmp = strtok(path, "/");
	while(tmp != NULL) {
		savtok = tmp;
		tmp = strtok(NULL, "/");
	}
	if(savtok != NULL)
		snprintf(fname, sizeof(fname), "%s", savtok);
	else
		snprintf(fname, sizeof(fname), "data.dat");
	if((file = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Cannot open output file for writing.\n");
		return 1;
	}
	if((bytes = fwrite(output.data, 1, output.size, file)) != output.size) {
		fprintf(stderr, "Error: Failed to write %ul bytes of data.\n", bytes);
		return 1;
	} else {
		puts("File transfer complete.");
	}
	fclose(file);

	if(output.data) {
		free(output.data);
		output.data = 0;
		output.size = 0;
	}
	return 0;	/* return success */
}

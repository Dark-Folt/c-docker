#ifndef DF_CURL_LIB_H
#define DF_CURL_LIB_H

#include <stdio.h>
#include <curl/curl.h>
#include <curl/header.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define DOCKER_SOKET_PATH "/var/run/docker.sock"

typedef struct {
    char *data;
    size_t size;
}Response;

char *docker_get(const char *endpoint);
void list_containers();

#endif
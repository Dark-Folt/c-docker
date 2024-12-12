#include "../include/lib.h"
#include <curl/curl.h>
#include <curl/easy.h>
#include <json-c/json_object.h>
#include <json-c/json_types.h>
#include <json-c/json_util.h>
#include <stdio.h>
#include <string.h>
#include <json-c/json.h>


/**
 * @brief 
 * Callback for writing received data
 * 
 * @param ptr : point to the delivered data 
 * @param size : always 1 that represent the size of one element which is a byte 
 * @param nmemb : size of that data
 * @param response : ptr of user struct that we would store data
 * @return size_t 
 */
static size_t
write_callback(void *data, size_t size, size_t nmemb, Response *response)
{
    size_t total_size = size * nmemb;

    // Cast void ptr to Response ptr 
    // Response *response = (Response *)userdata;

    char *new_data = realloc(response->data, response->size + total_size + 1);
    if (NULL == new_data) {
        fprintf(stderr, "[Realloc]: Out of memory on %d\n", __LINE__);
        return 0;
    }
    response->data = new_data;
    // Copy delivered data
    memcpy(response->data + response->size, data, total_size);
    response->size += total_size;

    // End by null char
    response->data[response->size] = '\0';
    return total_size;
}

char *
docker_get(const char *endpoint)
{
	CURL *curl;
	CURLcode result;
    Response response = {
        .data = NULL,
        .size = 0
    };

	// Init libcurl
	curl = curl_easy_init();
	if (NULL == curl) {
		fprintf(stderr, "[Error]: Init curl\n");
        return NULL;
	} 

    printf("Endpoint: %s\n", endpoint);

	// Access to docker API
	curl_easy_setopt(curl, CURLOPT_UNIX_SOCKET_PATH, DOCKER_SOKET_PATH);
	curl_easy_setopt(curl, CURLOPT_URL, endpoint);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

	// Perform GET request
	result = curl_easy_perform(curl);
	if (CURLHE_OK != result) {
		fprintf(stderr, "[Error]: List all running containers failed\n");
	}

	// free curl
	curl_easy_cleanup(curl);
    
    return response.data;
}

void parse_containers(const char *json_response) {
    json_object *json;

    json = json_object_new_string(json_response);

    json_object_to_fd(stdout, json, JSON_C_TO_STRING_COLOR);

}


void list_containers() {
    const char *endpoint = "http://localhost/containers/json";
    char *response = docker_get(endpoint);
    if (response) {
        parse_containers(response);
        free(response);
    }
}

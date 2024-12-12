#include "../include/lib.h"

int main(void)
{
	const char *endpoint = "http://localhost/containers/json"; 
	char *response = docker_get(endpoint);
	printf("Response: %ld\n", strlen(response));

	free(response);
	return 0;
}

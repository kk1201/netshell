#include <stdio.h>
#include "common.h"

int main(int argc, char** argv)
{
	printf("Hello client\n");

	int socket = connect_socket("localhost");
	if (socket <= 0) {
		fprintf(stderr, "Could not connect to socket, error %d\n", socket);
		cleanup_socket(socket);
		return 1;
	}

	char data[] = "Hello!";
	send_socket(socket, data, sizeof(data));
	cleanup_socket(socket);
}
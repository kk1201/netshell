#ifndef COMMON_H
#define COMMON_H

#define COMM_PORT "4268"

int get_tcp_socket( void );
int connect_socket(char* server_address);
int listen_socket(char* port);
void cleanup_socket(int socket);
int send_socket(int socket, void* data, size_t data_size);
int recv_info(int socket, void* out, size_t out_size);

#endif

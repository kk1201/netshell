#ifndef COMMON_H
#define COMMON_H

#define COMM_PORT "4268"

int get_tcp_socket( void );
int connect_socket(char*);
int listen_socket(char* port);
int recv_info(int socket, void* out);
int get_info_example(char* address);

#endif

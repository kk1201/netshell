#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "common.h"

#pragma comment(lib, "Ws2_32.lib")

static void get_msg_text(DWORD, char**);

int connect_socket(char* server_address)
{
    int i_status;
    int sockfd = INVALID_SOCKET;
    int i_addrlen;
    char* nc_error;
    DWORD dw_error;
    WSADATA s_wsaData;
    struct addrinfo s_hints;
    struct addrinfo* ps_res;
    struct addrinfo* ps_address;
    struct addrinfo* ps_servinfo;
    char ac_ipstr[INET6_ADDRSTRLEN] = { 0 };
    char ac_portstr[64];

    i_status = WSAStartup(MAKEWORD(2, 2), &s_wsaData);

    if (i_status != 0) {
        dw_error = (DWORD)i_status;
        get_msg_text(dw_error, &nc_error);
        fprintf(stderr, __FUNCTION__": WSAStartup failed with code %d.\n", i_status);
        fprintf(stderr, "%s\n", nc_error);
        LocalFree(nc_error);
        
        return -2;
    }

    memset(&s_hints, 0, sizeof(s_hints));

    s_hints.ai_family = AF_UNSPEC;
    s_hints.ai_socktype = SOCK_STREAM;

    i_status = getaddrinfo(server_address, COMM_PORT, &s_hints, &ps_servinfo);
    if (i_status != 0) {
        dw_error = (DWORD)WSAGetLastError();
        get_msg_text(dw_error, &nc_error);
        fprintf(stderr, __FUNCTION__": getaddrinfo failed with code %ld.\n", dw_error);
        fprintf(stderr, "%s\n", nc_error);
        LocalFree(nc_error);
        WSACleanup();

        return -4;
    }

    for (ps_address = ps_servinfo;
        ps_address != NULL;
        ps_address = ps_address->ai_next)
    {
        if (ps_address->ai_family == AF_INET6) continue;

        sockfd = socket(ps_address->ai_family, ps_address->ai_socktype,
            ps_address->ai_protocol);
        if (sockfd == INVALID_SOCKET)
        {
            dw_error = (DWORD)WSAGetLastError();
            get_msg_text(dw_error,
                &nc_error);
            fprintf(stderr, __FUNCTION__": socket failed with code %ld.\n", dw_error);
            fprintf(stderr, "%s\n", nc_error);
            LocalFree(nc_error);

            continue;
        }

        inet_ntop(ps_address->ai_family, &((struct sockaddr_in*)ps_address->ai_addr)->sin_addr, ac_ipstr, sizeof(ac_ipstr));
        snprintf(ac_portstr, sizeof(ac_portstr), "%d", ntohs(((struct sockaddr_in*)ps_address->ai_addr)->sin_port));
        printf("Trying connection to %s:%s socket %d\n", ac_ipstr, ac_portstr, sockfd);
        

        i_addrlen = (int)ps_address->ai_addrlen;
        i_status = connect(sockfd, ps_address->ai_addr, ps_address->ai_addrlen);

        if (i_status == SOCKET_ERROR)
        {
            dw_error = (DWORD)WSAGetLastError();
            get_msg_text(dw_error,
                &nc_error);
            fprintf(stderr, __FUNCTION__": connect failed with code %ld.\n", dw_error);
            fprintf(stderr, "%s\n", nc_error);
            LocalFree(nc_error);
            closesocket(sockfd);

            continue;
        }

        return sockfd;
    }

    return -1;
}

void cleanup_socket(int sockfd)
{
    if (sockfd > 0) {
        shutdown(sockfd, SD_SEND);
        closesocket(sockfd);
    }

    WSACleanup();
}

int send_socket(int sockfd, void* data, size_t data_size)
{
    int i_status = 0;
    if ((i_status = send(sockfd, data, data_size, 0)) <= 0) {
        fprintf(stderr, "Send failed with %d\n", i_status);
    }
    else {
        printf("Sent with %d bytes\n", i_status);
    }

    return i_status;
}
static void get_msg_text(DWORD dw_error, char** pnc_msg)
{
    DWORD dw_flags;
    dw_flags = FORMAT_MESSAGE_ALLOCATE_BUFFER
        | FORMAT_MESSAGE_FROM_SYSTEM
        | FORMAT_MESSAGE_IGNORE_INSERTS;

    FormatMessage(dw_flags, NULL, dw_error, LANG_SYSTEM_DEFAULT, (LPTSTR)pnc_msg, 0, NULL);
}
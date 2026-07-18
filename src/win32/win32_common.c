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

void get_msg_text(DWORD, char**);

int get_info_example(char* address)
{
    struct addrinfo* ps_address;
    void* pv_address;
    char* nc_error;
    DWORD dw_error;
    struct addrinfo s_hints;
    char ac_ipstr[INET6_ADDRSTRLEN];
    struct sockaddr_in* ps_ipv4;
    struct sockaddr_in6* ps_ipv6;
    char* pc_ipver;
    struct addrinfo* ps_res;
    int i_status;
    WSADATA s_wsaData;

    i_status = WSAStartup(MAKEWORD(2, 2), &s_wsaData);

    if (i_status != 0)
    {
        dw_error = (DWORD)i_status;
        get_msg_text(dw_error, &nc_error);
        fprintf(stderr, "WSAStartup failed with code %d.\n", i_status);
        fprintf(stderr, "%s\n", nc_error);
        LocalFree(nc_error);

        return 2;
    }

    if (LOBYTE(s_wsaData.wVersion) < 2 ||
        HIBYTE(s_wsaData.wVersion) < 2)
    {
        fprintf(stderr, "Version 2.2 of Winsock is not available.\n");
        WSACleanup();

        return 3;
    }

    memset(&s_hints, 0, sizeof(s_hints));

    s_hints.ai_family = AF_UNSPEC;
    s_hints.ai_socktype = SOCK_STREAM;

    i_status = getaddrinfo(address, NULL, &s_hints, &ps_res);

    if (i_status != 0)
    {
        dw_error = (DWORD)WSAGetLastError();
        get_msg_text(dw_error,
            &nc_error);
        fprintf(stderr, "getaddrinfo failed with code %ld.\n", dw_error);
        fprintf(stderr, "%s\n", nc_error);
        LocalFree(nc_error);
        WSACleanup();

        return 4;
    }

    printf("IP addresses for %s:\n\n", address);

    for (ps_address = ps_res; ps_address != NULL; ps_address = ps_address->ai_next)
    {
        /* Get the pointer to the address itself (different fields in IPv4 and IPv6): */
        if (ps_address->ai_family == AF_INET) // IPv4
        {
            ps_ipv4 = (struct sockaddr_in*)ps_address->ai_addr;
            pv_address = &(ps_ipv4->sin_addr);
            pc_ipver = "IPv4";
        }
        else // IPv6
        {
            ps_ipv6 = (struct sockaddr_in6*)ps_address->ai_addr;
            pv_address = &(ps_ipv6->sin6_addr);
            pc_ipver = "IPv6";
        }
        /* Convert the IP to a string and print it:                                   */
        inet_ntop(ps_address->ai_family, pv_address, ac_ipstr, sizeof(ac_ipstr));

        printf("  %s: %s\n", pc_ipver, ac_ipstr);
    }

    inet_ntop(ps_res->ai_family, &((struct sockaddr_in*)(ps_res->ai_addr))->sin_addr, ac_ipstr, sizeof(ac_ipstr));
    printf("Hostnames for %s:\n\n", ac_ipstr);
    CHAR nodeBuffer[NI_MAXHOST] = { 0 };
    getnameinfo(ps_res->ai_addr, ps_res->ai_addrlen, nodeBuffer, sizeof(nodeBuffer), NULL, 0, 0);
    printf("%s\n", nodeBuffer);

    WSACleanup();

    freeaddrinfo(ps_res);

    return 0;
}

void get_msg_text(DWORD dw_error, char** pnc_msg)
{
    DWORD dw_flags;
    dw_flags = FORMAT_MESSAGE_ALLOCATE_BUFFER
        | FORMAT_MESSAGE_FROM_SYSTEM
        | FORMAT_MESSAGE_IGNORE_INSERTS;

    FormatMessage(dw_flags, NULL, dw_error, LANG_SYSTEM_DEFAULT, (LPTSTR)pnc_msg, 0, NULL);
}
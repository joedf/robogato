#define _WIN32_WINNT 0x0501

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#pragma comment(lib, "Ws2_32.lib")

#define WINSOCK_REQUEST_VERSION MAKEWORD(2,2)

#define SEPERATOR "__________________________"
#define DEFAULT_PORT "6667"
#define DEFAULT_BUFLEN 512
#define BOT_VERBOSE 1

struct addrinfo *sck_resolve(const char *server, const char *port);
SOCKET get_socket(struct addrinfo *ptr);
int sck_connect(SOCKET *sck, struct addrinfo *ptr);
int sck_send(SOCKET *sck, char *sendbuf);
int sck_sendf(SOCKET *sck, const char *fmt, ...);
int sck_close(SOCKET *sck);
int sck_disconnect(SOCKET *sck);
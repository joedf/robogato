#include "main.h"

struct addrinfo *sck_resolve(const char *server, const char *port) {
	struct addrinfo hints;
	struct addrinfo *pAddr = NULL;
	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	
	int err = getaddrinfo(server, port, &hints, &pAddr);
	if (err != 0) {
		printf("getaddrinfo failed: %d\n", err);
		WSACleanup();
		return NULL;
	}
	return pAddr;
}

SOCKET get_socket(struct addrinfo *ptr) {
	SOCKET ConnectSocket = INVALID_SOCKET;
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	if (ConnectSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}
	return ConnectSocket;
}

int sck_connect(SOCKET *sck, struct addrinfo *ptr) {
	int err = connect( *sck, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (err == SOCKET_ERROR) {
		closesocket(*sck);
		printf("Unable to connect to server!\n");
		WSACleanup();
		return -1;
	}
	return err;
}

int sck_send(SOCKET *sck, char *sendbuf) {
	int err = send(*sck, sendbuf, (int) strlen(sendbuf), 0);
	if (BOT_VERBOSE)
		printf("[POSTED]-> %s",sendbuf);
	if (err == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(*sck);
		WSACleanup();
		return 0;
	}
	return err;
}

int sck_sendf(SOCKET *sck, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	char s[DEFAULT_BUFLEN];
	vsprintf(s,fmt,args);
	va_end(args);
	return sck_send(sck,s);
}

int sck_close(SOCKET *sck) {
	int err = shutdown(*sck, SD_SEND);
	if (err == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(*sck);
		WSACleanup();
		return -1;
	}
	return err;
}

int sck_disconnect(SOCKET *sck) {
	int err = shutdown(*sck, SD_SEND);
	if (err == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(*sck);
		WSACleanup();
		return -1;
	}
	return err;
}

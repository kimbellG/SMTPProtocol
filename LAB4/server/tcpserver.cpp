#include "tcpserver.h"

#include <iostream>

#include "ch.h"

TCPSocket::TCPSocket(const char *ip, const int port) : Server(ip, port, SOCK_STREAM, "tcpsock.log") {
	listen(s, 5);
	std::cout << "Server activate. Start listening..." << std::endl;
}

TCPSocket::TCPSocket(SOCKET sock) : Server(sock) {}

TCPSocket::~TCPSocket() {};

int TCPSocket::send(const char *buf, const int szbuf, const char *toip, const int toport) {
	int total = 0;
	int n;

	while (total < szbuf) {
		n = ::send(s, buf + total, szbuf - total, 0);
		if (n == SOCKET_ERROR) {
			log_t(check("send").c_str());
			break;
		} else
			total += n;
	}

	return (n == SOCKET_ERROR) ? SOCKET_ERROR : total;
}

int TCPSocket::recv(char *buf, const int szbuf, sockaddr_in *from) {
	int n;
	while (!(n = ::recv(s, buf, szbuf, 0)));

	if (n < 0)
		log_t(check("recv").c_str());

	return n;
}

SOCKET TCPSocket::accept() {
	SOCKET clsocket;
	sockaddr_in claddr;
	int szcladdr = sizeof (claddr);
	clsocket = ::accept(s, (sockaddr *) &claddr, &szcladdr);
	if (clsocket == INVALID_SOCKET)
		log_t(check("accept").c_str());

	return (clsocket == INVALID_SOCKET) ? INVALID_SOCKET : clsocket;
}

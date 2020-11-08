#include "client.h"

#include "../../errcheck.h"

Client::Client(const char *ip, const int port) {
	s = socket(AF_INET, SOCK_STREAM, 0);
	errcheck(!(s == INVALID_SOCKET), (char *)"Starting process of sending", (char *)"Socket has been failed with error code: ", WSAGetLastError());
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	szaddr = sizeof (addr);
}

void Client::connection() {
	errcheck(!(connect(s, (sockaddr *) &addr, szaddr) == SOCKET_ERROR), "Connect successfull!",
			 "Connect has been failed with error code: ", WSAGetLastError());
}

int Client::sendstr(const char *buf) {
	int numbyte = send(s, buf, strlen(buf), 0);
	errcheck(!(numbyte == SOCKET_ERROR),
			 "Send has been successfull", "Send has been failed with error code: ", WSAGetLastError(), false);

	return numbyte;
}

int Client::getstr(char *buf) {
	int endstr = recv(s, buf, 100, 0);
	errcheck(!(endstr == SOCKET_ERROR),
			"Data is accept.", "Recv has been failed with error code: ", WSAGetLastError(), false);

	buf[endstr] = '\0';
	return endstr;
}

void Client::close() {
	closesocket(s);
}



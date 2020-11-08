#include <iostream>
#include <cstring>
#include <ctime>

#include "../../errcheck.h"

#include "smtpserver.h"

using namespace std;

DWORD WINAPI clproccesing(LPVOID clsocket) {
	TCPSocket cl(*((SOCKET *) clsocket));
	SMTPServer client;

	char buf[1024];
	std::string answer;

	clock_t lastdata;
	clock_t current;

	while (true) {
		int num = cl.recv(buf, kbyte);

		if (!num)
			continue;

		buf[num] = '\0';


		if (!strcmp(buf, "QUIT\r\n"))
			break;


		current = clock();
		if (!strcmp(buf, "DATA\r\n")) {
			if ((current - lastdata) / CLOCKS_PER_SEC > 15) {
				lastdata = clock();
				answer = client.proccesing_recv(buf, (*((SOCKET *) clsocket)));
			} else
				answer = "550 — The frequency of sending messages is limited. 15 seconds one message";
		} else
			answer = client.proccesing_recv(buf, (*((SOCKET *) clsocket)));

		if (cl.send(answer.c_str(), answer.size()) == SOCKET_ERROR)
			break;
	}

	cl.close();
	cout << "Break connetion." << endl;
	return 0;
}

int main() {
	WORD wverreq;
	WSADATA wsadata;
	wverreq = MAKEWORD(2, 2);
	WSAStartup(wverreq, &wsadata);

	TCPSocket network("127.0.0.1", 25);

	while (true) {
		SOCKET clsocket = network.accept();
		DWORD thID;
		if (clsocket != INVALID_SOCKET)
			errcheck((CreateThread(NULL, NULL, clproccesing, &clsocket, NULL, &thID)), "New client. Start proccesing...", "Thread not started with error code: ", GetLastError());
	}

	WSACleanup();
	return 0;
}

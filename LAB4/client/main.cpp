/*
 * main.cpp
 *
 *  Created on: 6 сент. 2020 г.
 *      Author: filip
 */

#include <iostream>
#include <winsock2.h>
#include <cstring>
#include <string>

#include "client.h"
#include "../../errcheck.h"

using namespace std;

int main() {
	WORD wreqver;
	WSADATA wsadata;
	wreqver = MAKEWORD(2,2);
	int errcode = WSAStartup(wreqver, &wsadata);
	errcheck(!errcode, (char *)"Init WSA done.", (char *)"WSAStart has been failed with errrocode: ", errcode, false);

	Client cl("127.0.0.1", 25);
	cl.connection();

	string command = "HELO 127.0.0.1";

	while (command != "QUIR\r\n") {
		cl.sendstr(command.c_str());

		char result[1024];
		cl.getstr(result);
		cout << result << endl;

		if (command == "DATA\r\n") {
			while (command != ".\r\n") {
				cout << "Input message: ";
				std::getline(cin, command);
				command.push_back('\r');
				command.push_back('\n');
				cl.sendstr(command.c_str());
			}
		}

		cout << "Input string: ";
		std::getline(cin, command);
		command.push_back('\r');
		command.push_back('\n');
	}

	cl.close();
	WSACleanup();
	return 0;
}

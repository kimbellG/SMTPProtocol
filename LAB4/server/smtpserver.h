#ifndef SMTPSERVER_H
#define SMTPSERVER_H


#include <vector>
#include <string>
#include <fstream>

#include "tcpserver.h"

class SMTPServer {
	std::vector<std::string> mailboxes;
	std::ofstream log;

	long long incomming;
	long long outgoing;
	std::string ip;

	std::string helo(std::string &domain);
	std::string rcptto(std::string &out);
	std::string mailfrom(std::string &in);
	std::string data(int sock);
	std::string invalid();
	std::string quit();

	static std::string &deletestr(std::string &str, const int end, int start = 0);
public:
	SMTPServer();
	~SMTPServer();

	std::string proccesing_recv(const char *str, int sock_fordata = -1);
};

#endif // SMTPSERVER_H

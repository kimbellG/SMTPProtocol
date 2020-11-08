#include "smtpserver.h"

#include <iostream>
#include <fstream>
#include <ctime>

using std::string;

SMTPServer::~SMTPServer() {}

SMTPServer::SMTPServer() : log("allusers.log") {
	std::ifstream in("mailboxes.txt");

	incomming = -1;
	outgoing = -1;
	ip = "";

	if (!in)
		return;

	string tmp;
	while (std::getline(in, tmp, ':')) {
		mailboxes.push_back(tmp);
		std::getline(in, tmp);
	}
}

std::string SMTPServer::proccesing_recv(const char *str, int sock) {
	std::string mainstr(str);

	if (mainstr == "")
		return "";
	mainstr.pop_back();
	mainstr.pop_back();

	std::string answer;
	log << mainstr << std::endl;

	std::string command;
	size_t i = 0;

	if (mainstr == "QUIT")
		return quit();
	else if (mainstr == "DATA")
		return data(sock);


	for (; i < 4; i++)
		command.push_back(mainstr[i]);
	mainstr = deletestr(mainstr, i);

	if (command == "HELO")
		answer = helo(mainstr);

	else if (command == "MAIL")
		answer = mailfrom(mainstr);

	else if (command == "RCPT")
		answer = rcptto(mainstr);
	else
		answer = invalid();

	log << answer << std::endl;

	return answer;
}

std::string SMTPServer::helo(std::string &domain) {
	ip = domain;
	log << "New client: " << domain << std::endl;

	return string("250 SMTP is ready");
}
std::string SMTPServer::mailfrom(std::string &in) {
	if (ip == "")
		return "503 bad sequence of commands, the mail server requires authentication.";

	string rightcommand;

	size_t i = 0;
	while (in[i] != ' ')
		rightcommand.push_back(in[i++]);
	in = deletestr(in, i);

	if (rightcommand != "FROM:")
		return invalid();

	for (size_t i = 0; i < mailboxes.size(); i++) {
		if (in == mailboxes[i]) {
			incomming = i;
			break;
		}
	}

	if (incomming == -1)
		return "555 bad mailbox";

	return string("250 OK");
}
std::string SMTPServer::invalid() {
	return string("501 Invalid syntax");
}
std::string SMTPServer::rcptto(std::string &str) {
	if (ip == "")
		return "503 bad sequence of commands, the mail server requires authentication.";

	string rightcommand;

	size_t i = 0;
	for (; str[i] != ' '; i++)
		rightcommand.push_back(str[i]);
	str = deletestr(str, i);

	if (rightcommand != "TO:")
		return invalid();

	for (size_t i = 0; i < mailboxes.size(); i++) {
		if (str == mailboxes[i]) {
			outgoing = i;
			break;
		}
	}

	if (outgoing == -1)
		return "555 bad mailbox";

	return "250 OK";
}
std::string SMTPServer::data(int sock) {
	if (ip == "" || incomming == -1 || outgoing == -1)
		return "503 bad sequence of commands, the mail server requires authentication.";


	TCPSocket network(sock);

	char buf[kbyte] = "354 Start mail input; end with <CLRF>.<CLRF>";
	string data;
	network.send(buf, kbyte);

	while (strcmp(buf, ".\r\n")) {
		int num = network.recv(buf, kbyte);
		buf[num] = '\0';
		data += buf;
		if (strcmp(buf, ".\r\n"))
			data.pop_back();
	}

	std::time_t current = std::time(NULL);
	string currtm = std::ctime(&current);
	currtm.pop_back();

	std::ofstream account("mailboxes/" + mailboxes[outgoing] + "_inbox.dat", std::ofstream::app | std::ofstream::out);
	account << mailboxes[incomming] << ':' << currtm << std::endl << data << std::endl;

	account.close();
	account.open("mailboxes/" + mailboxes[incomming] + "_shipped.dat", std::ofstream::app | std::ofstream::out);
	account << mailboxes[outgoing] << ':' << currtm << std::endl << data << std::endl;

	return "250 OK";
}
std::string SMTPServer::quit() {
	return "quit";
}

std::string &SMTPServer::deletestr(std::string &str, const int end, int start) {
	string::iterator curr = str.begin() + start;

	while (start++ <= end)
		str.erase(curr);

	return str;
}

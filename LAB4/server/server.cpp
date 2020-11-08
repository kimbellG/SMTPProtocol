#include "server.h"

#include "ch.h"

Server::Server(const char *ip, const int port, const int type,
			   const char *logfile) : log(logfile, std::ofstream::app | std::ofstream::out) {
	s = socket(AF_INET, type, 0);

	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = inet_addr(ip);

	szlocal = sizeof (local);

	if (bind(s, reinterpret_cast<sockaddr *>(&local), szlocal) < 0)
		check("Bind");
}

Server::Server(SOCKET sock) {
	s = sock;
}

Server::~Server() {}

SOCKET Server::accept() {
	return -1;
};

void Server::log_t(const char *message) {
	log << s << ": " << message << std::endl;
}

#ifndef CH_H
#define CH_H

#include <winsock2.h>
#include <iostream>
#include <cstdlib>
#include <cstring>

inline std::string check(const char *cause) {
	return std::string(cause) + ": " + std::to_string(WSAGetLastError());
}

#endif // CH_H

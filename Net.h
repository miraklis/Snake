#pragma once

#include "stdHeaders.h"
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

namespace Snake
{
	class Net
	{
	public:
		enum class NetStatus { Disconnected = 0, WaitingForClients, WaitingForServer, Connected};
	private:
		WSADATA wsaData;
		struct addrinfo *info, hints;
		const char* DEFAULT_PORT{ "7654" };
		SOCKET listenSocket;
		SOCKET clientSocket;
		NetStatus status;
	public:
		Net();
		~Net();
		void Send(char* sendBuffer);
		char* Rcv();
		void CreateServer(string ipAddress);
		void CreateClient(string ipAddress);
		NetStatus GetStatus() const;
	};
}

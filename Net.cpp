#include "Net.h"

namespace Snake
{
	Net::Net()
	{
		listenSocket = INVALID_SOCKET;
		clientSocket = INVALID_SOCKET;
		if(int result = WSAStartup(MAKEWORD(2, 2), &wsaData))
			cout << "WSAStartup failed " << endl << result << endl;
		status = NetStatus::Disconnected;
	}

	Net::~Net()
	{
		if(listenSocket != INVALID_SOCKET)
		{
			shutdown(listenSocket, SD_BOTH);
			closesocket(listenSocket);
		}
		if(clientSocket != INVALID_SOCKET)
		{
			shutdown(clientSocket, SD_BOTH);
			closesocket(clientSocket);
		}
		WSACleanup();
	}

	void Net::Send(char* sendBuffer)
	{
	}

	char* Net::Rcv()
	{
		return nullptr;
	}

	void Net::CreateServer(string ipAddress)
	{
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		int result = getaddrinfo(ipAddress.c_str(), DEFAULT_PORT, &hints, &info);
		if(result != 0)
		{
			cout << "GetAddrInfo failed " << endl << result << endl;
			WSACleanup();
			return;
		}
		listenSocket = INVALID_SOCKET;
		listenSocket = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
		if(listenSocket == INVALID_SOCKET)
		{
			cout << "Error at socket(" << WSAGetLastError() << ")" << endl;
			freeaddrinfo(info);
			WSACleanup();
			return;
		}
		result = ::bind(listenSocket, info->ai_addr, (int)info->ai_addrlen);
		if(result == SOCKET_ERROR)
		{
			cout << "Bind failed with error: " << WSAGetLastError() << endl;
			freeaddrinfo(info);
			closesocket(listenSocket);
			WSACleanup();
			return;
		}
		if(::listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
		{
			cout << "Listen failed with error: " << WSAGetLastError() << endl;
			closesocket(listenSocket);
			WSACleanup();
			return;
		}
		cout << "Listening for connections on port " << DEFAULT_PORT << "..." << endl;
		status = NetStatus::WaitingForClients;
		thread([&](){
			clientSocket = ::accept(listenSocket, NULL, NULL);
			if(clientSocket == INVALID_SOCKET) {
				cout << "Accept failed: " << WSAGetLastError() << endl;
				closesocket(listenSocket);
				WSACleanup();
			} else {
				cout << "Client connected" << endl;
				status = NetStatus::Connected;
			}
		}).detach();
	}

	void Net::CreateClient(string ipAddress)
	{
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		int result = getaddrinfo(ipAddress.c_str(), DEFAULT_PORT, &hints, &info);
		if(result != 0)
		{
			cout << "GetAddrInfo failed " << endl << result << endl;
			WSACleanup();
			return;
		}
		clientSocket = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
		if(clientSocket == INVALID_SOCKET)
		{
			cout << "Error at socket(" << WSAGetLastError() << ")" << endl;
			freeaddrinfo(info);
			WSACleanup();
			return;
		}
		status = NetStatus::WaitingForServer;
		result = connect(clientSocket, info->ai_addr, (int)info->ai_addrlen);
		if(result == SOCKET_ERROR)
		{
			closesocket(clientSocket);
			clientSocket = INVALID_SOCKET;
		}
		if(clientSocket == INVALID_SOCKET)
		{
			cout << "Unable to connect to server!" << endl;
			WSACleanup();
			return;
		}
		cout << "Succesfully connected to server " << ipAddress << endl;
		status = NetStatus::Connected;
	}

	Net::NetStatus Net::GetStatus() const
	{
		return status;
	}
}
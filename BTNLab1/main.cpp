#include <iostream>
#include <string>
#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")

int main()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "Could not start DLLs" << std::endl;
        return 0;
    }

    SOCKET ClientSocket;
    ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ClientSocket == INVALID_SOCKET) {
        std::cout << "Could not create socket" << std::endl;
        WSACleanup();
        return 0;
    }

    struct sockaddr_in SvrAddr;
    SvrAddr.sin_family = AF_INET;
    SvrAddr.sin_port = htons(27000);
    SvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //SvrAddr.sin_addr.s_addr = inet_addr("159.203.26.94"); // PART A
    if ((connect(ClientSocket, (struct sockaddr*) & SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR) {
        std::cout << "Failed to connect to server" << std::endl;
        closesocket(ClientSocket);
        WSACleanup();
        return 0;
    }

    while (1) // PART C
    {
        std::string TxBuffer;

        std::cout << "Enter a String to transmit" << std::endl;
        getline(std::cin, TxBuffer); // PART B
        send(ClientSocket, TxBuffer.c_str(), TxBuffer.length(), 0);

        char RxBuffer[128] = {};
        recv(ClientSocket, RxBuffer, sizeof(RxBuffer), 0);
        std::cout << "Response: " << RxBuffer << std::endl;

        if (TxBuffer == "[q]")
        {
            closesocket(ClientSocket);
            WSACleanup();
            return 0;
        }
        else if (TxBuffer == "[quit]")
        {
            closesocket(ClientSocket);
            WSACleanup();
            return 0;
        }

    }

    return 0;
}
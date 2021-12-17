#include "socket.h"
#include <string>
#include <iostream>
#include <windows.networking.sockets.h>
#include <stdlib.h>
#pragma comment(lib, "Ws2_32.lib")

bool Node::dlls_started = false;
int Node::num_nodes = 0;

void display(unsigned char byte) {
    for (int k = 0; k < 8; k++) {
        std::cout << (int)(byte >> 7);
        byte = byte << 1;
    }
    std::cout << std::endl;
}

unsigned char calculate_data_packet_crc(data_packet my_packet) {
    unsigned char byte, input, feedback, state = 0;
    int size = strlen(my_packet.data);
    for (int i = 0; i < size; i++) {
        byte = my_packet.data[i];
        for (int j = 0; j < 8; j++) {
            input = byte >> 7;
            feedback = ((state & 0x80) >> 7) ^
                ((state & 0x20) >> 5) ^ input;
            state = (state << 1) + feedback;
            byte = byte << 1;
        }
    }
    return state;
}

Node::Node() {
    start_dlls();
    num_nodes++;
    this->active_socket = INVALID_SOCKET;
    this->protocol = "tcp";
    this->ip = "127.0.0.1";
    this->port = 27000;
}

Node::Node(std::string ip, int port) {
    start_dlls();
    num_nodes++;
    this->active_socket = INVALID_SOCKET;
    this->ip = ip;
    this->port = port;
}

Node::~Node() {
    closesocket(this->active_socket);
    std::cout << "Closing socket" << std::endl;
    num_nodes--;
    if (num_nodes == 0) {
        WSACleanup();
    }
}

void Node::start_dlls() const {
    if (!dlls_started) {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cout << "Could not start DLLs" << std::endl;
            exit(EXIT_FAILURE);
        }
        else {
            dlls_started = true;
        }
    }
}

bool Node::create_socket() {
    if (this->protocol == "tcp") {
        this->active_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    }
    else {
        this->active_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    }

    if (this->active_socket == INVALID_SOCKET) {
        std::cout << "Could not create socket" << std::endl;
        return false;
    }
    else {
        return true;
    }
}

void Node::display_info() const {
    std::cout << "IP: " << this->ip << std::endl;
    std::cout << "port: " << this->port << std::endl;
    std::cout << "protocol: " << this->protocol << std::endl;
    std::cout << "dlls: " << dlls_started << std::endl;
}

Server_TCP::Server_TCP() : Node() {
    this->role = "server";
    this->client_socket = INVALID_SOCKET;
}

Server_TCP::Server_TCP(std::string ip, int port) : Node(ip, port) {
    this->protocol = "tcp";
    this->role = "server";
    this->client_socket = INVALID_SOCKET;
}

Server_TCP::~Server_TCP() {
    closesocket(this->client_socket);
    std::cout << "Closing client socket" << std::endl;
}

bool Server_TCP::bind_socket() {
    struct sockaddr_in SvrAddr;
    SvrAddr.sin_family = AF_INET;
    SvrAddr.sin_addr.s_addr = inet_addr(this->ip.c_str());
    SvrAddr.sin_port = htons(this->port);
    if (bind(this->active_socket, (struct sockaddr*) & SvrAddr,
        sizeof(SvrAddr)) == SOCKET_ERROR) {
        std::cout << "Could not bind socket to address" << std::endl;
        return false;
    }
    else {
        return true;
    }
}

bool Server_TCP::listen_for_connections() {
    if (listen(this->active_socket, 1) == SOCKET_ERROR) {
        std::cout << "Could not start to listen" << std::endl;
        return false;
    }
    else {
        return true;
    }
}

bool Server_TCP::accept_connection() {
    this->client_socket = accept(this->active_socket, NULL, NULL);
    if (this->client_socket == INVALID_SOCKET) {
        std::cout << "Failed to accept connection" << std::endl;
        return false;
    }
    else {
        return true;
    }
}

int Server_TCP::send_data_packet(data_packet my_packet)
{
    struct serialized_packet serialized = data_packet_serializer(my_packet);
    return send(this->active_socket, serialized.data, serialized.length, 0);
}

int Server_TCP::send_message(std::string message) {
    return send(this->client_socket, message.c_str(), message.length(), 0);
}

int Server_TCP::receive_message(std::string& message) {
    char RxBuffer[MAX_BUFFER_SIZE] = {};
    memset(RxBuffer, 0, MAX_BUFFER_SIZE);
    int num_bytes = recv(this->client_socket, RxBuffer, MAX_BUFFER_SIZE, 0);
    message = RxBuffer;
    return num_bytes;
}

int Server_TCP::receive_data_packet(data_packet& my_packet)
{
    char RxBuffer[MAX_BUFFER_SIZE] = {};
    memset(RxBuffer, 0, MAX_BUFFER_SIZE);
    int num_bytes = recv(this->client_socket, RxBuffer, MAX_BUFFER_SIZE, 0);
    my_packet = data_packet_deserializer(RxBuffer);
    return num_bytes;
}

void Server_TCP::close_connection() {
    closesocket(this->client_socket);
}

Client_TCP::Client_TCP() : Node() {
    this->protocol = "tcp";
    this->role = "client";
}

bool Client_TCP::connect_socket(std::string ip, int port) {
    struct sockaddr_in SvrAddr;
    SvrAddr.sin_family = AF_INET;
    SvrAddr.sin_port = htons(port);
    SvrAddr.sin_addr.s_addr = inet_addr(ip.c_str());
    if ((connect(this->active_socket, (struct sockaddr*) & SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR) {
        std::cout << "Failed to connect to server" << std::endl;
        return false;
    }
    else {
        return true;
    }
}

int Client_TCP::send_data_packet(data_packet my_packet)
{
    struct serialized_packet serialized = data_packet_serializer(my_packet);
    return send(this->active_socket, serialized.data, serialized.length, 0);
}

int Client_TCP::send_message(std::string message) {
    return send(this->active_socket, message.c_str(), message.length(), 0);
}


int Client_TCP::receive_message(std::string& message) {
    char RxBuffer[MAX_BUFFER_SIZE] = {};
    memset(RxBuffer, 0, MAX_BUFFER_SIZE);
    int num_bytes = recv(this->active_socket, RxBuffer, MAX_BUFFER_SIZE, 0);
    message = RxBuffer;
    return num_bytes;
}

int Client_TCP::receive_data_packet(data_packet& my_packet)
{
    char RxBuffer[MAX_BUFFER_SIZE] = {};
    memset(RxBuffer, 0, MAX_BUFFER_SIZE);
    int num_bytes = recv(this->active_socket, RxBuffer, MAX_BUFFER_SIZE, 0);
    my_packet = data_packet_deserializer(RxBuffer);
    return num_bytes;
}

data_packet create_packet()
{
    data_packet my_packet;
    std::cout << "Enter a seq for Packet: ";
    std::cin >> my_packet.seq;

    my_packet.ack = 1;
    my_packet.flag = 1;
    my_packet.rst = 1;

    std::cout << "Enter a length for Packet: ";
    std::cin >> my_packet.length;

    my_packet.data = new char[my_packet.length];
    for (int i = 0; i < my_packet.length; i++)
    {
        std::cout << "Enter data " << i << ": ";
        std::cin >> my_packet.data[i];
    }

    return my_packet;
}

serialized_packet data_packet_serializer(data_packet my_packet)
{
    int packet_size = sizeof(int) + sizeof(unsigned char) + sizeof(unsigned int) + my_packet.length * sizeof(char) + sizeof(unsigned int);

    char* my_serialized_packet = new char[packet_size];

    char* auxptr = my_serialized_packet;
    memcpy(auxptr, &my_packet.seq, (sizeof(int) + sizeof(unsigned char)));
    auxptr += sizeof(int) + sizeof(unsigned char);
    memcpy(auxptr, &my_packet.length, sizeof(unsigned int));
    auxptr += sizeof(unsigned int);
    memcpy(auxptr, my_packet.data, my_packet.length * sizeof(char));
    auxptr += my_packet.length * sizeof(char);
    memcpy(auxptr, &my_packet.crc, sizeof(unsigned char));
    auxptr += sizeof(unsigned char);

    serialized_packet output;
    output.length = packet_size;
    output.data = my_serialized_packet;
    return output;
}

data_packet data_packet_deserializer(char* serialized_packet)
{
    data_packet de_packet;
    char* auxptr = serialized_packet;
    memcpy(&de_packet.seq, auxptr, 1 * sizeof(int) + 1 * sizeof(unsigned char));
    auxptr += 1 * sizeof(int) + 1 * sizeof(unsigned char);
    memcpy(&de_packet.length, auxptr, 1 * sizeof(unsigned int));
    auxptr += 1 * sizeof(unsigned int);
    de_packet.data = new char[de_packet.length];
    memcpy(de_packet.data, auxptr, de_packet.length * sizeof(char));
    auxptr += de_packet.length * sizeof(char);
    memcpy(&de_packet.crc, auxptr, sizeof(unsigned char));
    auxptr += 1 * sizeof(unsigned char);

    return de_packet;
}

bool check_crc(data_packet my_packet)
{

    if (calculate_data_packet_crc(my_packet) == my_packet.crc)
    {
        return true;
    }
    else
    {
        return false;
    }
}
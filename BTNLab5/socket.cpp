#include <string>;
#include <windows.networking.sockets.h>
#include <iostream>
#include "socket.h"

using namespace std;

Node::Node()
{
	cout << "Base default constructor Node()" << endl;
}

Node::Node(std::string ip, int port)
{
	cout << "Base default constructor Node() with 2 args: " << ip << endl;
	cout << "Base default constructor Node() with 2 args: " << port << endl;
}

Node::~Node()
{
	cout << "Base default destructor ~Node()" << endl;
}

void Node::start_dlls()
{
	cout << "Node::start_dlls()" << endl;
}

bool Node::create_socket()
{
	cout << "Node::create_socket()" << endl;

	return true;
}

void Node::display_info()
{
	cout << "Node::display_info()" << endl;
}

Server_TCP::Server_TCP()
{
	cout << "Derived default constructor Server_TCP()" << endl;
}

Server_TCP::Server_TCP(std::string ip, int port)
{
	cout << "Derived default constructor Server_TCP with 2 args: " << ip << endl;
	cout << "Derived default constructor Server_TCP with 2 args: " << port << endl;
}

Server_TCP::~Server_TCP()
{
	cout << "Derived default destructor ~Server_TCP()" << endl;
}

bool Server_TCP::bind_socket()
{
	cout << "Server_TCP::bind_socket()" << endl;
	
	return true;
}

bool Server_TCP::listen_for_connections()
{
	cout << "Server_TCP::listen_for_connections()" << endl;

	return true;
}

bool Server_TCP::accept_connection()
{
	cout << "Server_TCP::accepts_connections()" << endl;

	return true;
}

int Server_TCP::send_message(std::string msg)
{
	cout << "Server_TCP::send_messages(): " << msg << endl;

	return 1;
}

int Server_TCP::receive_message(std::string& msg)
{
	cout << "Server_TCP::receive_message(): " << msg << endl;

	return 1;
}

void Server_TCP::close_connection()
{
	cout << "Server_TCP::close_connection()" << endl;
}

Client_TCP::Client_TCP()
{
	cout << "Derived default constructor Client_TCP()" << endl;
}

bool Client_TCP::connect_socket(std::string ip, int port)
{
	cout << "Client_TCP::connect_socket(): " << ip << endl;
	cout << "Client_TCP::connect_socket(): " << port << endl;

	return true;
}

int Client_TCP::send_message(std::string msg)
{
	cout << "Client_TCP::send_message(): " << msg << endl;

	return 1;
}

int Client_TCP::receive_message(std::string& msg)
{
	cout << "Client_TCP::recieve_message(): " << msg << endl;

	return 1;
}

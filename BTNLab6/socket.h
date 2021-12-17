#ifndef SOCKET_H
#define SOCKET_H
#include <string>
#include <windows.networking.sockets.h>

#define MAX_BUFFER_SIZE 512

class Node
{
protected:

	static bool dlls_started;
	static int num_nodes;
	std::string ip;
	std::string role;
	std::string protocol;
	int port;
	SOCKET active_socket;

public:

	Node();
	Node(std::string ip, int port);
	~Node();
	void start_dlls();
	bool create_socket();
	void display_info() const;
};

class Server_TCP : public Node
{
private:

	SOCKET client_socket;

public:

	Server_TCP();
	Server_TCP(std::string ip, int port);
	~Server_TCP();
	bool bind_socket();
	bool listen_for_connections();
	bool accept_connection();
	int send_message(std::string msg);
	int receive_message(std::string& msg);
	void close_connection();
};

class Client_TCP : public Node
{
public:

	Client_TCP();
	bool connect_socket(std::string ip, int port);
	int send_message(std::string msg);
	int receive_message(std::string& msg);
};

class Server_UDP : public Node
{
public:
	Server_UDP();
	Server_UDP(std::string ip, int port);
	bool bind_socket();
	int send_message(std::string msg, std::string ip, int port);
	sockaddr_in receive_message(std::string& msg);
};

class Client_UDP : public Node
{
public:
	Client_UDP();
	int send_message(std::string msg, std::string ip, int port);
	sockaddr_in receive_message(std::string& msg);
};

#endif
#ifndef SERVER_HPP
#define SERVER_HPP

#include"head.hpp"
#include<list>

class client;

class	server {
private:
	std::string			_passwd;
	int					_port;
	int					_server_sock;
	sockaddr_in			_server_addr;
	socklen_t			_addr_len;
	std::list<client*>	_clients;
public:
	// Orthedox Form
	server();
	server(const server& other);
	server& operator=(const server& other);
	~server();

	// Server Setup
	void	launch(std::string	passwd, std::string	port);
};

#endif
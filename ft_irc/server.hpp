#ifndef SERVER_HPP
#define SERVER_HPP

#include"head.hpp"
#include<list>
#include <poll.h>
#include <sstream>
#include <algorithm>

#define IRC true

class client;

class	server {
private:
	std::string			_passwd;
	int					_port;
	int					_server_sock;
	sockaddr_in			_server_addr;
	socklen_t			_addr_len;
	std::list<client*>	_clients;
	int 				_activity;
public:
	// Orthedox Form
	server();
	server(const server& other);
	server& operator=(const server& other);
	~server();

	// Server Setup
	void	launch(std::string	passwd, std::string	port);
	int 	open_socket();
	void 	setpoll(int);
	void 	check_requ(std::string , client *);
	void	sendMessage(client	*from , client *to, const std::string& msg) const;
};

#endif
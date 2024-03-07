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
	int					_port;
	std::string			_passwd;
	int					_server_sock;
	sockaddr_in			_server_addr;
	socklen_t			_addr_len;
	std::list<client*>	_clients;
	int 				_activity;
	int ncl;

	void	new_client(std::stringstream& iss);
	void	handleMsg(std::stringstream& iss);

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
	void 	check_requ( std::string );
	void	sendMessage(client	*from , client *to, const std::string& msg) const;
};

#endif
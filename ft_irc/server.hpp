#ifndef SERVER_HPP
#define SERVER_HPP

#include"head.hpp"
#include<list>
#include <vector>
#include <poll.h>
#include <sstream>
#include <algorithm>
#include <map>
#define IRC true
class client;
class channel;

class	server {
private:
	int					_port;
	std::string			_passwd;
	int					_server_sock;
	sockaddr_in			_server_addr;
	socklen_t			_addr_len;
	std::vector<client*>	_clients;
	std::vector<channel*>	_channels;
	int 				_activity;
    size_t nfds;
	
	void				new_client(std::string& , int);
	void				handleMsg(std::string& , int);
	client*				getClient(int fd);
	client*				getClient(std::string nick);
	bool				validPASS(std::stringstream &iss);
	std::string 		msg;

public:
	socklen_t		_c_addr_len;
	sockaddr_in		_client_addr;
	// Orthedox Form
	server();
	server(const server& other);
	server& operator=(const server& other);
	~server();

	// Server Setup
	void	launch(std::string	passwd, std::string	port);
	int 	open_socket();
	void 	setpoll(int);
	void 	check_requ( std::string , int);
	void	sendMessage(client	*from , client *to, const std::string& msg) const;
	///
	std::string 	creatPong(std::string &, client *, std::string ) ;
	bool validUser(std::string user);
	bool	chackIfChannelExists(const std::string name) const;
	channel*	getChannel(std::string name);
};

#endif
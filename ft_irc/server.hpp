#ifndef SERVER_HPP
#define SERVER_HPP

#include"head.hpp"
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include<list>
#include <vector>
#include <poll.h>
#include <sstream>
#include <algorithm>
#include <map>
#include <netdb.h>
#define IRC true
#include "bot.hpp"
class bot;
class client;
class channel;

class	server : public bot {
private:
	int					_port;
	std::string			_passwd;
	int					_server_sock;
	sockaddr_in			_server_addr;
	socklen_t			_addr_len;
	std::vector<client*>	_clients;
	std::vector<channel*>	_channels;
	int 				_activity;
    size_t 				nfds;
	std::string			_token;
	int					_id;

	
	bool				validPASS(std::stringstream &iss);
	std::string 		msg;

	client				*_server;
public:

	void				new_client(std::string& , int);
	void				handleMsg(std::string& , int);
	client*				getClient(int fd);
	client*				getClient(std::string nick);
	
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
	void	listofclients(std::vector<struct pollfd> &fds);
	///
	std::string 	creatPong(std::string &, client *, std::string ) ;
	bool validUser(std::string user);
	bool	chackIfChannelExists(const std::string name) const;
	channel*	getChannel(std::string name);
	bool	GenerateServerData(const std::string &port);

	///commands
	void who( client *, std::stringstream& );
	void user( client *, std::stringstream& );
	void nick( client *, std::stringstream& );
	void join( client *, std::stringstream& );
	void privmsg( client *, std::stringstream& );
	void topic( client *, std::stringstream& );
	void invite( client *, std::stringstream& );
	void mode( client *, std::stringstream& );
	void kick( client *, std::stringstream& );


	//Methods 
	channel*	 createNewChannel(std::string name, client* creator);

	//bot
	void bot(client *cl, std::stringstream &os);
};
#endif
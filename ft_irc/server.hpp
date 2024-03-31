#pragma once

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
#include <algorithm>
#include "bot.hpp"
#define IRC true
#include "bot.hpp"
class bot;
class client;
class channel;

class	server : public bot{
private:
	// data
	int					_port;
	std::string			_passwd;
	int					_server_sock;
	sockaddr_in			_server_addr;
	socklen_t			_addr_len;

	int 				_activity;
    size_t 				nfds;

	int					_id;
	std::string			_token;
	bool				_quit;
	

	std::vector<client*>	_clients;
	std::vector<channel*>	_channels;

	client				*_server;


public:
	// Public attribues
	socklen_t		_c_addr_len;
	sockaddr_in		_client_addr;

	// Orthedox Form
	server();
	~server();

	// Getters
	client*			getClient(int fd);
	client*			getClient(std::string nick);
	channel*		getChannel(std::string name);
	
	// Creation
	void			new_client(std::string& , int);
	channel*		createNewChannel(std::string name, client* creator);

	// Checkers
	bool			validPASS(std::stringstream &iss);
	void 			check_requ( std::string , int);
	bool			validUser(std::string user);
	bool			chackIfChannelExists(const std::string name) const;

	// Server Setup
	void			launch(std::string	passwd, std::string	port);
	int 			open_socket();
	void 			setpoll(int);
	void			listofclients(std::vector<struct pollfd> &fds);

	// Communication
	void			handleMsg(std::string& , int);
	void			sendMessage(client	*from , client *to, const std::string& msg) const;
	void			broadcast( const std::string& message );
	void			quiteMessege(int fd);


	/// Commands
	void 	who( client *, std::stringstream& );
	void 	join( client *, std::stringstream& );
	void 	privmsg( client *, std::stringstream& );
	void 	topic( client *, std::stringstream& );
	void 	invite( client *, std::stringstream& );
	void 	mode( client *, std::stringstream& );
	void 	part( client *, std::stringstream& );
	void 	kick( client *, std::stringstream& );
	void 	user( client *, std::stringstream& );
	void 	nick( client  *, std::stringstream& );
	void	Emit( client *, std::stringstream & );


	void	removeclient(client  *cl);
	void	removeChannel(channel  *ch);

};

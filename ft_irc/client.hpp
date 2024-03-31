#ifndef CLIENT_HPP
#define CLIENT_HPP

#include<iostream>
#include"head.hpp"
#include "server.hpp"
class server;

#define MAX_CLIENT 75
#define BUFFER_SIZE 4096


class	client {
private:
	bool			_active;
	std::string		_username;
	std::string		_nickname;
	std::string		_ipaddress;
	int				_client_sock;
	char 	_buff[BUFFER_SIZE];
public:

	// Orthedox Form
	client();
	client(int socket);
	client(int socket, int id, std::string & ip);
	bool operator==(const client& other);
	~client();
	
	// Setters
	void	setUsername( std::string Username );
	void	setNickname( std::string Nickname );
	void	setIpAddress( std::string IpAddress );
	void	setClientsock(int socket);
	void 	setActive(bool);

	// Getters
	std::string		getUsername() const ;
	std::string		getNickname() const ;
	int				getClientsock() const ;
	sockaddr_in 	getClientaddr() const ;
	bool 			getActive() const;
	std::string		getIpaddress() const ;
	std::string 	getForm() const ;

	// Methods
	void	printClient() const;
};


#endif
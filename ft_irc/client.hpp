#ifndef CLIENT_HPP
#define CLIENT_HPP

#include<iostream>
#include"head.hpp"

class	client {
private:
	std::string		_username;
	std::string		_nickname;
	int				_client_sock;
	char			_buff[BUFFER_SIZE];
public:

	socklen_t		_addr_len;
	sockaddr_in		_client_addr;

	// Orthedox Form
	client();
	client(int socket);
	client(const client& other);
	client operator=(const client& other);
	client operator==(const client& other);
	~client();
	
	// Setters
	void	setUsername( std::string& Username );
	void	setNickname( std::string& Nickname );
	void	setClientsock(int socket);

	// Getters
	std::string		getUsername() const ;
	std::string		getNickname() const ;
	int				getClientsock() const ;
	sockaddr_in 	getClientaddr() const ;

};

#endif
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include<iostream>
#include"head.hpp"

#define MAX_CLIENT 256
#define BUFFER_SIZE 1024
class	client {
private:
	bool			_active;
	std::string		_username;
	std::string		_nickname;
	std::string		_ipaddress;
	int				_client_sock;
	char			_buff[BUFFER_SIZE];
public:

	socklen_t		_addr_len;
	sockaddr_in		_client_addr;

	// Orthedox Form
	client();
	client(int socket);
	client(const client& other);
	client &operator=(const client& other);
	bool operator==(const client& other);
	~client();
	
	// Setters
	void	setUsername( std::string& Username );
	void	setNickname( std::string& Nickname );
	void	setIpAddress( std::string& IpAddress );
	void	setClientsock(int socket);
	void 	setActive(bool);

	// Getters
	std::string		getUsername() const ;
	std::string		getNickname() const ;
	int				getClientsock() const ;
	sockaddr_in 	getClientaddr() const ;

	// Methods
	void	printClient() const;
};


#endif
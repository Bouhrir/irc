#include "client.hpp"

// Orthedox Form
client::client() : _username("username"), _nickname("nickname") {
	_client_sock = 0;
}

client::client(int socket) : _username("username"), _nickname("nickname") {
	_client_sock = socket;
	if (socket == -1)
		throw	std::runtime_error("Failed accepting a connection : " + std::string(strerror(errno)));
}

client::client(const client& other) {

}

client client::operator=(const client& other) {

}

client client::operator==(const client& other) {
	return (this->_username == other._username);
}

client::~client() {

}

// Setters
void	client::setUsername( std::string& Username ) {
	_username = Username;
}
void	client::setNickname( std::string& Nickname ) {
	_nickname = Nickname;
}
void	client::setClientsock(int socket) {
	_client_sock = socket;
	if (socket == -1)
		throw	std::runtime_error("Failed accepting a connection : " + std::string(strerror(errno)));
}

// Getters
std::string	client::getUsername() const {
	return (_username);
}

std::string	client::getNickname() const {
	return (_nickname);
}

int				client::getClientsock() const {
	return (_client_sock);
}

sockaddr_in 	client::getClientaddr() const {

}

#include "client.hpp"

// Orthedox Form
client::client() : _username("user" + std::to_string(rand() % 1000)), _nickname("nick" + std::to_string(rand() % 1000)){
	_client_sock = 0;
}


client::client(int socket) : _username("user" + std::to_string(rand() % 1000)), _nickname("nick" + std::to_string(rand() % 1000)) {
	_client_sock = socket;
	if (socket == -1)
		throw	std::runtime_error("Failed accepting a connection : " + std::string(strerror(errno)));
}

client::client(int socket, int id,  std::string & ip) : _username("user" + std::to_string(id)), _nickname("nick" + std::to_string(id)) , _ipaddress(ip) {
	_client_sock = socket;
	if (socket == -1)
		throw	std::runtime_error("Failed accepting a connection : " + std::string(strerror(errno)));
}

bool client::operator==(const client& other) {
	return (this->_nickname == other._nickname);
}

client::~client() {

}

// Setters
void	client::setUsername( std::string Username ) {
	_username = Username;
}
void	client::setNickname( std::string Nickname ) {
	_nickname = Nickname;
}
void	client::setIpAddress( std::string IpAddress ) {
	_ipaddress = IpAddress;
}
void	client::setClientsock(int socket) {
	_client_sock = socket;
	if (socket == -1)
		throw	std::runtime_error("Failed accepting a connection : " + std::string(strerror(errno)));
}
void 	client::setActive(bool set){
	_active = set;
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

std::string		client::getIpaddress() const {
	return _ipaddress;
}
// sockaddr_in 	client::getClientaddr() const {
// 	return _client_addr;
// }

bool client::getActive() const {
	return _active;
}

std::string client::getForm() const {
    return  this->getNickname() + '!' + this->getUsername() + '@' + this->getIpaddress();
}

// methods

void client::printClient() const {
    std::cout << "\033[1;36mClient Information\033[0m" << std::endl;
    std::cout << "Client fd: " <<  getClientsock() << std::endl;
    std::cout << "Username: " << _username << std::endl;
    std::cout << "Nickname: " << _nickname << std::endl;
    std::cout << "IP Address: " << _ipaddress << std::endl;

}

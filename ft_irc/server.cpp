#include "server.hpp"
#include "client.hpp"

static	int	my_atoi(std::string& str) {
    size_t	i = 0;
    int		result = 0;

    // Skip leading whitespaces
    while (i < str.size() && std::isspace(str[i])) {
        ++i;
    }

    // Check for sign
    if (i < str.size() && (str[i] == '+' || str[i] == '-')) {
		throw	std::invalid_argument("Don't use any signs in the Port");
    }

    // Process digits
    while (i < str.size() && std::isdigit(str[i])) {
        int digit = str[i] - '0';

        // Check for overflow
        if (result > INT_MAX / 10 || (result == INT_MAX / 10 && digit > INT_MAX % 10)) {
            throw std::out_of_range("Integer overflow");
        }

        result = result * 10 + digit;
        ++i;
    }
	return (result);
}

// Orthedox Form
server::server() : _port(0), _passwd("password") {
	_server_sock = 0;
	_server_addr.sin_family = AF_INET;
	_server_addr.sin_port = htons(_port);
	_server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	_addr_len = sizeof(_server_addr);
}

server::server(const server& __unused other) {

}

server& server::operator=(const server& __unused other) {
	return *this;
}

server::~server() {

}

void server::setpoll(int act){
	activity = act;
	if (activity == -1)
		throw std::runtime_error("Failed in poll(): " + std::string(strerror(errno)));
}

void printascii(std::string ss){
	std::cout << "ascii => ";
	for (size_t i = 0; i < ss.length(); i++){
		if (static_cast<int>(ss.c_str()[i]) == 10)
			std::cout << "\\n";
		else if (static_cast<int>(ss.c_str()[i]) == 13)
			std::cout << "\\r";
		else
			std::cout << static_cast<char>(ss.c_str()[i]);
	}
	std::cout << std::endl;
}

void server::check_requ(std::string str, client *Client){
	std::stringstream iss(str);
	std::string token;

	while (std::getline(iss, token, '\n')){
		if (!std::strncmp(token.c_str(), "USER", 4)){
			std::stringstream ss(token);
			std::string str;
			std::getline(ss, str, ' ');std::getline(ss, str, ' ');
			// printascii(str);
			if (str.empty())
				throw std::runtime_error("Error: invalid username");
			std::list<client *>::iterator it = _clients.begin();
			for (; it != _clients.end();++it){
				if ((*it)->getUsername() == str)
					throw std::runtime_error("Error: this username already exists");
			}
			Client->setUsername(str);

		}
		else if (!std::strncmp(token.c_str(), "NICK", 4)){
			std::stringstream ss(token);
			std::string str;
			std::getline(ss, str, ' '); std::getline(ss, str, '\r');

			if (str.empty())
				throw std::runtime_error("Error: invalid nickname");
			
			Client->setNickname(str);
		}
		else if (!std::strncmp(token.c_str(), "PASS", 4)){
			std::stringstream ss(token);
			std::string str;
			std::getline(ss, str, ' '); std::getline(ss, str, '\r');
		
			if (str != this->_passwd)
				throw std::runtime_error("Error: invalid password");
			Client->setActive(true);
		}
	}
	_clients.push_back(Client);
	// std::list<client *>::iterator i = _clients.begin();
	// for (; i != _clients.end(); ++i)
	// 	std::cout << "list = " << (*i)->getUsername() << std::endl;
}
// Server Setup
void	server::launch(std::string	passwd, std::string	port) {
	_port = my_atoi(port);
	_passwd = passwd;
	_server_sock = open_socket();

	_server_addr.sin_family = AF_INET;
	_server_addr.sin_port = htons(_port);
	_server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int enable = 1;
	if (setsockopt(_server_sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0)
		throw std::runtime_error("Failed in setting socket option: " + std::string(strerror(errno)));

	if (bind(_server_sock, (struct sockaddr *)&_server_addr, sizeof(_server_addr)) == -1) {
		throw	std::runtime_error("Failed to bind server's socket: " + std::string(strerror(errno)));
	}

	if (listen(_server_sock, MAX_CLIENT) == -1) {
		throw	std::runtime_error("Failed in listening to server's socket: " + std::string(strerror(errno)));
	}

	std::cout << "\033[1;32mThe server is listening on the port ==> \033[0m" << "\033[1;41m" << _port << "\033[0m" <<  std::endl;
	
	struct pollfd fds[MAX_CLIENT + 1];
	
	fds[0].fd = _server_sock;
	fds[0].events = POLLIN;
	size_t nfds = 1;

	while (IRC){
			//multiplexing
			setpoll(poll(fds, nfds, 5000));

			client *c = new client();
			if (fds[0].revents & POLLIN){
				c->setClientsock(accept(_server_sock, reinterpret_cast<sockaddr *>(&c->_client_addr), &c->_addr_len));

				fds[nfds].fd = c->getClientsock();
				fds[nfds].events = POLLIN | POLLOUT;
				++nfds;
			}
			//check request
			for (int i = 1; i < nfds ; ++i){
				if (fds[i].revents & POLLIN){
					std::cout << "client fd: " << fds[i].fd << std::endl;
					std::cout << "IP => " << inet_ntoa(c->_client_addr.sin_addr) << std::endl;
					char buff[100];
					int read = recv(fds[i].fd, buff, sizeof(buff), 0);
					if (read == -1)
						throw std::runtime_error("hbas hna");
					buff[read] = '\0';
					std::cout << buff;
					check_requ(buff, c);
				}
			}
	}

	
}


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
	_activity = act;
	if (_activity == -1)
		throw std::runtime_error("Failed in poll(): " + std::string(strerror(errno)));
}

void printascii(std::string ss){
	for (size_t i = 0; i < ss.length(); i++){
		if (static_cast<int>(ss.c_str()[i]) == 10)
			std::cout << "\\n";
		else if (static_cast<int>(ss.c_str()[i]) == 13)
			std::cout << "\\r";
		else {
			std::cout << static_cast<int>(ss.c_str()[i]);
		}
	}

	std::cout << " --  " << ss << std::endl;
}

void server::check_requ(std::string str, client *Client){
	std::stringstream iss(str);
	std::string token;
	std::string tmp;

	while (std::getline(iss, token, '\n')) {
		std::istringstream iss(token);
		std::string command;
		iss >> command;
		std::cout << "command " << command << std::endl;
		if (!std::strncmp(command.c_str(), "USER", 4)){
    		std::string username, nickname, ipAddress;
    		iss >> username >> nickname >> ipAddress;

    		// Remove the last colon (:) from the ipAddress
    		ipAddress = ipAddress.substr(0, ipAddress.size() - 1);

    		// Set's the client's object with the parsed information
		std::cout << "username && address" << username << "  " << ipAddress << std::endl;
    		Client->setUsername(username);
    		Client->setIpAddress(ipAddress);
		}
		else if (!std::strncmp(command.c_str(), "NICK", 4)){
    		std::string nickname;
			iss >> nickname;

    		// nickname = nickname.substr(0, nickname.size() - 1);
			Client->setNickname(str);
		}
		else if (!std::strncmp(command.c_str(), "PASS", 4)){
			std::string passwd;
			iss >> passwd;

			if (passwd != _passwd)
				throw std::runtime_error("Error: invalid password");
			Client->setActive(true);
		}
	}
	int count = 0;
	// Client->printClient();
	_clients.push_back(Client);
	// std::list<client *>::iterator i = _clients.begin();
	// for (; i !=  _clients.end(); ++i, ++count) {
	// 	std::cout << "index = " << count << std::endl;
	// 	(*i)->printClient();
	// }
}

void	printClient(client *c) {

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
			setpoll(poll(fds, nfds, -1));

			client *c;
			if (fds[0].revents & POLLIN){
				c = new client();

				c->setClientsock(accept(_server_sock, reinterpret_cast<sockaddr *>(&c->_client_addr), &c->_addr_len));
					// std::cerr << "Client IP address: " << inet_ntoa(c->_client_addr.sin_addr) << std::endl;
					// std::cerr << "Actual size of clientAddr structure: " << c->_addr_len << std::endl;
				fds[nfds].fd = c->getClientsock();
				fds[nfds].events = POLLIN | POLLOUT;
				++nfds;
			}
			//check request
			for (int i = 1; i < nfds ; ++i){
				if (fds[i].revents & POLLIN){
					std::cout << "client fd: " << fds[i].fd << std::endl;
					char buff[1024];
					int read = recv(fds[i].fd, buff, sizeof(buff), 0);
					if (read == -1)
						throw std::runtime_error("hbas hna");
					buff[read] = '\0';
					// std::cerr << buff << std::endl;
					check_requ(buff, c);
				}
			}
	}

	
}


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

	std::cout << ss << " " << std::endl;
}

bool validCAPLS(const std::string& message) {
	if (!message.compare(0, 6, "CAP LS")) {
		return true;
	}
    return false;
}

void	server::new_client(std::stringstream& iss) {
	std::string	token;
	client *Client = new client();

	while (std::getline(iss, token, '\n')) {
		std::istringstream iss(token);
		std::string command;
		iss >> command;
		if (!std::strncmp(command.c_str(), "USER", 4)){
			std::string username, nickname, ipAddress;
			iss >> username >> nickname >> ipAddress;

			Client->setUsername(username);
			Client->setIpAddress(ipAddress);
		}
		else if (!std::strncmp(command.c_str(), "NICK", 4)){
			std::string nickname;
			iss >> nickname;

			Client->setNickname(nickname);
		}
		else if (!std::strncmp(command.c_str(), "PASS", 4)){
			std::string passwd;
			iss >> passwd;

			if (passwd != _passwd)
				throw std::runtime_error("Error: invalid password");
			Client->setActive(true);
		}
	}
	_clients.push_back(Client);
}

void	server::handleMsg(std::stringstream& iss) {
	std::string	token;
	// client *Client = client::getClient();

	while (std::getline(iss, token, '\n')){
		if (!token.compare(0, 7, "PRIVMSG")){
			std::stringstream ss(token);
			std::string prvmsg, user, msg;
			ss >> prvmsg >> user >> msg;
			std::cout << msg << std::endl;

		}

	}
 

}

void server::check_requ(std::string str){
	std::stringstream iss(str);
	std::string token;
	if (validCAPLS(str))
		new_client(iss);
	else
		handleMsg(iss);

}


int	check_new_client(std::string buff) {
	std::istringstream  iss(buff);

	return 1;
}

// Server Setup
void	server::launch(std::string	passwd, std::string	port) {
	_port = my_atoi(port);
	_passwd = passwd;
	_server_sock = open_socket();

	_server_addr.sin_family = AF_INET;
	_server_addr.sin_port = htons(_port);
	_server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	ncl = 0;
	int enable = 1;
	if (setsockopt(_server_sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0)
		throw std::runtime_error("Failed in setting socket option: " + std::string(strerror(errno)));

	if (bind(_server_sock, (struct sockaddr *)&_server_addr, sizeof(_server_addr)) == -1) {
		throw	std::runtime_error("Failed to bind server's socket: " + std::string(strerror(errno)));
	}

	if (listen(_server_sock, MAX_CLIENT) == -1) {
		throw	std::runtime_error("Failed in listening to server's socket: " + std::string(strerror(errno)));
	}

	std::cout << "\033[1;42mThe server is listening on the port\033[0m ==> " << "\033[1;41m" << _port << "\033[0m" <<  std::endl;
	
	std::vector<struct pollfd> fds(MAX_CLIENT + 1);
	
    // Add the server socket to the fds vector
    fds[0].fd = _server_sock;
    fds[0].events = POLLIN;
    size_t nfds = 1;

    while (IRC) {
        // multiplexing
        setpoll(poll(&fds[0], nfds, -1));

			if (fds[0].revents & POLLIN){
				
				socklen_t		_addr_len;
				sockaddr_in		_client_addr;
				int clientSocket = accept(_server_sock, reinterpret_cast<sockaddr *>(&_client_addr), &_addr_len);
				if (clientSocket == -1)
					throw	std::runtime_error("Failed accepting a connection : " + std::string(strerror(errno)));

				fds[nfds].fd = clientSocket;
				fds[nfds].events = POLLIN | POLLOUT;
				++nfds;
			}
			//check request
			for (size_t i = 1; i < nfds; ++i){
				if (fds[i].revents & POLLIN){
					char buff[1024];
					
					int read = recv(fds[i].fd, buff, sizeof(buff), 0);
					if (read == -1)
						throw std::runtime_error("Failed receving data" + std::string(strerror(errno)));
					buff[read] = '\0';
					check_requ(buff);
					if (fds[i].revents & (POLLHUP | POLLERR)){
						//del user if disconnected
						if (i > 0){
							std::cerr << "<fd=" << fds[i].fd << "> " << ": disconnected" << std::endl;
							close(fds[i].fd);
							fds.erase(fds.begin() + i);
						}
					}
				}
			}
	}
}

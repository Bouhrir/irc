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

server::server(const server& other) {

}

server& server::operator=(const server& other) {

}

server::~server() {

}

// Server Setup
void	server::launch(std::string	passwd, std::string	port) {
	_port = my_atoi(port);
	_passwd = passwd;
	_server_sock = open_socket();

	_server_addr.sin_family = AF_INET;
	_server_addr.sin_port = htons(_port);
	_server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(_server_sock, (struct sockaddr *)&_server_addr, sizeof(_server_addr)) == -1) {
		throw	std::runtime_error("Failed to bind server's socket: " + std::string(strerror(errno)));
	}

	if (listen(_server_sock, 256) == -1) {
		throw	std::runtime_error("Failed in listening to server's socket: " + std::string(strerror(errno)));
	}

	std::cout << "The server is listening on the port ==> " << _port << std::endl;

	client test;

	test.setClientsock(accept(_server_sock, (sockaddr *)&test._client_addr, &test._addr_len));
	
}


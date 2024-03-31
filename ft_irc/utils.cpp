#include "server.hpp"

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

static std::string getipmachine() {
    char hostname[256];
    struct hostent *host_entry;
    struct in_addr **addr_list;
    int i;

    // Get the hostname
    if (gethostname(hostname, sizeof(hostname)) == -1) {
        perror("Error getting hostname");
        return "127.0.0.1";
    }

    // Get host entry
    if ((host_entry = gethostbyname(hostname)) == NULL) {
        perror("Error getting host entry");
        return "127.0.0.1";
    }

    // Retrieve IP addresses
    addr_list = (struct in_addr **)host_entry->h_addr_list;
	close(5);
    return inet_ntoa(*addr_list[0]);
}

// Server Setup
int	server::open_socket() {
	int	sock;

	sock = socket(AF_INET, TCP, IPPROTO_TCP);
	if (sock == -1) {
		throw	std::runtime_error("Failed to open server's socket: " + std::string(strerror(errno)));
	}

	if (fcntl(sock, F_SETFL, O_NONBLOCK) == -1) {
		throw	std::runtime_error("Failed to set server's socket as non-blocking: " + std::string(strerror(errno)));
	}
	return (sock);
}

void server::setpoll(int act){
	_activity = act;
	if (_activity <= 0)
		throw std::runtime_error("Failed in poll(): " + std::string(strerror(errno)));
}

void server::listofclients(std::vector<struct pollfd> &fds) {
	for (size_t i = 1; i < fds.size(); ++i){
		std::string all;
		if (fds[i].revents & POLLIN) {
			std::string tmp;
			char buff[BUFFER_SIZE];
			int read;
			while (true) {
				read = recv(fds[i].fd, buff, sizeof(buff) -1, 0);
				if (read > 0 && errno != EPIPE){
					buff[read] = '\0';
					tmp = buff;
					all = all + tmp;
				}
				else if (read <= 0 || errno == EPIPE)
					break;
			}
			check_requ(all, fds[i].fd);
		}
		if ((fds[i].revents & (POLLHUP | POLLERR)) || _quit) {
				//del user if disconnected
				if (i > 0) {
					std::cerr << "\033[1;41m<fd=" << fds[i].fd << "> IP " <<  inet_ntoa(_client_addr.sin_addr) << ": disconnected\033[0m" << std::endl;
					quiteMessege(fds[i].fd);
					std::vector<struct pollfd>::iterator it = fds.begin();
					std::advance(it, i);
					fds.erase(it);
					_quit = false;
				}
			}
	}
}

void	server::launch(std::string	passwd, std::string	port) {
	_port = my_atoi(port);
	_passwd = passwd;
	_server_sock = open_socket();
	_server = new client(_server_sock);
	_server->setIpAddress(getipmachine()); //anaaaaa 4adi ldaaar

	_quit = false;

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

	std::cout << "\033[1;42mSERVER IP CONNECTED: \033[0m ==> " << "\033[1;41m"  << _server->getIpaddress() << "\033[0m" << "\n\n";
	sleep(1);
	std::cout << "\033[1;42mTHE SERVER IS LISTENING ON THE PORT\033[0m ==> " << "\033[1;41m" << _port << "\033[0m" <<  "\n\n";
	std::vector<struct pollfd> fds;
    // Add the server socket to the fds vector
	struct pollfd srv;

    srv.fd = _server_sock;
   	srv.events = POLLIN;
	fds.push_back(srv);
	signal(SIGPIPE, SIG_IGN);
	// bot = new client();
	nfds = 1;
    while (IRC) {
        // multiplexing
        setpoll(poll(&fds[0], fds.size(), -1));
		//accept new clients
		if (fds[0].revents & POLLIN){
			struct pollfd cl;
			_c_addr_len = sizeof(_client_addr);
			int clientSocket = accept(_server_sock, (struct sockaddr *)(&_client_addr), &_c_addr_len);
			if (clientSocket == -1)
				throw	std::runtime_error("Failed accepting a connection : " + std::string(strerror(errno)));
			fcntl(clientSocket, F_SETFL, O_NONBLOCK) ;
			std::cout << "\033[1;42mNEW CLIENT CONNECTED: \033[0m ==> \033[1;41m" << inet_ntoa(_client_addr.sin_addr) << "\033[0m\n";
			cl.fd = clientSocket;
			cl.events = POLLIN | POLLOUT;
			fds.push_back(cl);
			srand(time(NULL));
			_id = rand() % 1000;
			++nfds;
		}
		
		//check request
		listofclients(fds);
	}
}

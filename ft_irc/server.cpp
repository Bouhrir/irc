#include "server.hpp"
#include "client.hpp"
#include "head.hpp"
#include "stdio.h"
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
server::server() : _port(0), _passwd("password"), nfds(1) {
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

bool server::validPASS(std::stringstream &iss) {
	std::string token;
	while (std::getline(iss, token, '\n')){
		std::stringstream inc(token);
		std::string pass;
		inc >> pass;
		if (!pass.compare(0, 4,"PASS")){
			std::string passwd;
			inc >> passwd;
			if (passwd == _passwd){
				return true;
			}
		}
	}
	return false;
}
void	server::new_client(std::string& str, int fd) {
	std::string	token;
	std::string ip(inet_ntoa(_client_addr.sin_addr));
	client *Client = new client(fd, _id, ip);
	std::stringstream ss(str);
	while (std::getline(ss, token, '\n')) {
		
		std::istringstream iss(token);
		std::string command;
		iss >> command;
		if (!std::strncmp(command.c_str(), "PASS", 4)){
			std::string passwd;
			iss >> passwd;

			if (passwd != _passwd)
				throw std::runtime_error("Error: invalid password");
			Client->setActive(true);
		}
		else if (!std::strncmp(command.c_str(), "USER", 4) && Client->getActive()){
			std::string username, nickname, ipAddress;
			iss >> username >> nickname >> ipAddress;

			Client->setUsername(username);
			Client->setIpAddress(ipAddress);
		}
		else if (!std::strncmp(command.c_str(), "NICK", 4) && Client->getActive()){
			std::string nickname;
			iss >> nickname;

			Client->setNickname(nickname);
		}
		usleep(100);
	}
	std::cout << "----newclient----\n";
	if (Client)
		Client->printClient();
	_clients.push_back(Client);
}

void	join(client *c, channel& chan) {
	chan.addMember(c);
}

bool	server::chackIfChannelExists(const std::string name) const {
	for (size_t i = 0; i < _channels.size(); ++i) {
		if (_channels[i]->getName() == name)
			return true;
	}
	return false;
}

channel*	server::getChannel(std::string name) {
	for (size_t i = 0; i < _channels.size(); ++i) {
		if (_channels[i]->getName() == name)
			return _channels[i];
	}
	return NULL;
}

// :obouhrir!oussama@88ABE6.25BF1D.D03F86.88C9BD.IP PRIVMSG obouhrir :slama sahbi
// :obouhrir!oussama@88ABE6.25BF1D.D03F86.88C9BD.IP JOIN #general * :realnam
std::string server::creatPong(std::string &token, client *c, std::string check){
	// std::map<std::string, std::string> args;
	std::stringstream iss(token);
	std::string pong, skip, target;
	iss >> skip >> target;
	if (check == "prvmsg")
		pong = ':' + c->getForm() + " PRIVMSG " + target + " :" + msg + "\r\n"; 
	else if (check == "join") {
		if (chackIfChannelExists(target)) {
			puts("deja");
			channel* chan = getChannel(target);
			chan->addMember(c);
		} else {
			puts("makinax");
			channel *newChan = new channel(target, _server);
			_channels.push_back(newChan);
			newChan->addMember(c);
		}
	}
	return pong;
}

bool server::validUser(std::string nick){
	std::vector<client *>::iterator it = _clients.begin();
	for (; it != _clients.end();++it){
		if ((*it)->getNickname() == nick)
			return true;
	}
	return false;
}

client*	server::getClient(int fd) {
	for  (size_t i = 0; i < _clients.size(); ++i) {
		if (_clients[i]->getClientsock() == fd){
			return _clients[i];
		}
	}
	return NULL;
}

client*	server::getClient(std::string nick) {
	for  (size_t i = 0; i < _clients.size(); ++i) {
		if (_clients[i]->getNickname() == nick){
			return _clients[i];
		}
	}
	return NULL;
}

void server::who(client *Client , std::stringstream& os){
	std::cout << "who\n";
	std::string chan;

	os >> chan;
	channel	*ch = getChannel(chan);
	if (!ch) {
		sendMessage(_server, Client, ":localhost " + ERR_NOSUCHCHANNEL(Client->getNickname(), chan));
	} else {
		ch->RPL_who(Client);
	}
}
void server::user(client *Client , std::stringstream& os){
	std::cout << "user\n";

}
void server::nick(client *Client , std::stringstream& os){
	std::cout << "nick\n";

}
void server::join(client *Client , std::stringstream& os){
	std::cout << "join\n";

	std::string chan, buffer;
	os >> chan;

	size_t pos = chan.find('#');
	if (pos != std::string::npos){
		channel *ch = getChannel(chan);
		if (!ch){
			ch = new channel(chan, _server);
			_channels.push_back(ch);
			ch->setModes("t");
			ch->addOperator(Client);
			ch->addMember(Client);

		} else {
			ch->addMember(Client);
		}
	}
	else {
		buffer = ":localhost " +  ERR_NOSUCHCHANNEL(Client->getNickname(), chan);
		send(Client->getClientsock(), buffer.c_str(), buffer.size(), 0);
	}

}
void server::privmsg(client *Client , std::stringstream& os){
	std::cout << "privmsg\n";
	std::string buffer;
	std::stringstream ss(_token);
	std::string skip, nick;
	ss >> skip >> nick;
	if (validUser(nick))
	{
		client *c = getClient(nick);
		size_t pos = _token.find(':');
		if (pos != std::string::npos){
			msg = _token.substr(pos + 1);
			std::string pong = creatPong(_token, Client, "prvmsg");
			send(c->getClientsock(), pong.c_str(), pong.length(), 0);
			std::cout << pong << std::endl;
		}
	}
	else{
		buffer = ":" + _server->getIpaddress() + " " + ERR_NOSUCHNICK(Client->getNickname(), nick);
		send(Client->getClientsock(), buffer.c_str(), buffer.size(), 0);
	}

}
void server::topic(client *Client , std::stringstream& os){
	std::cout << "topic\n";

}
void server::invite(client *Client , std::stringstream& os){
	std::cout << "invite\n";

}
void server::mode(client *Client , std::stringstream& os){
	std::cout << "mode\n";
	std::string chan, modes;
	os >> chan >> modes;
	channel	*ch = getChannel(chan);
	if (!ch) {
		sendMessage(_server, Client, ":localhost " + ERR_NOSUCHCHANNEL(Client->getNickname(), chan));
	} else {
		if  (!modes.empty()) {
			ch->setModes(modes);
		} else {
			ch->RPL_list(Client);
			ch->RPL_mode(Client);
		}
	}

}

void server::kick(client *Client , std::stringstream& os){
	std::cout << "kick\n";

}
//PART #gen :Leaving
//: 403 obouhrir #ggggg :No such channel
void server::part(client *Client, std::stringstream& os){
	std::cout << "part\n";
	std::string chan;
	os >> chan;
	std::cout << chan << std::endl;
	channel *cha = getChannel(chan);
	if (!cha){
		sendMessage(_server, Client, ":localhost " + ERR_NOSUCHCHANNEL(Client->getNickname(), chan));
	}
	else
	{
		////
	}
}

void	server::handleMsg(std::string& str, int fdClient) {
	std::stringstream iss(str);
	client *Client = getClient(fdClient);
	std::cout << "----msg----\n";
	std::string arr[] = {"WHO" ,"USER" ,"NICK" ,"JOIN" ,"PRIVMSG" ,"TOPIC" ,"INVITE" ,"MODE"  ,"KICK", "PART"};

	void (server::*env[10])(client *, std::stringstream&) = {&server::who, &server::user, &server::nick, &server::join , &server::privmsg, &server::topic, &server::invite, &server::mode, &server::mode, &server::part};
	int i = 0;
	while (std::getline(iss, _token, '\n')){
		std::stringstream os(_token);
		std::string cmd;
		os >> cmd;
		for(; i < 10; ++i){
			if (cmd == arr[i])
				break;
		}
		switch(i){
			case 0:
				(this->*(env[0]))(Client, os);
				break;
			case 1:
				(this->*(env[1]))(Client, os);
				break;
			case 2:
				(this->*(env[2]))(Client, os);
				break;
			case 3:
				(this->*(env[3]))(Client, os);
				break;
			case 4:
				(this->*(env[4]))(Client, os);
				break;
			case 5:
				(this->*(env[5]))(Client, os);
				break;
			case 6:
				(this->*(env[6]))(Client, os);
				break;
			case 7:
				(this->*(env[7]))(Client, os);
				break;
			case 8:
				(this->*(env[8]))(Client, os);
				break;
			case 9:
				(this->*(env[9]))(Client, os);
			default:
				break;
		}

	}
}


void server::check_requ(std::string str, int fd){
	std::stringstream iss(str);


	if (validPASS(iss)){
		new_client(str, fd);
	}
	else
		handleMsg(str, fd);
}


int	check_new_client(std::string buff) {
	std::istringstream  iss(buff);

	return 1;
}
void server::listofclients(std::vector<struct pollfd> &fds){
	for (size_t i = 1; i < fds.size(); ++i){
		if (fds[i].revents & POLLIN){
			char buff[BUFFER_SIZE];
			int read;
			read = recv(fds[i].fd, buff, sizeof(buff), 0);
			if (read == -1)
				throw std::runtime_error("Failed receving data" + std::string(strerror(errno)));
			buff[read] = '\0';
			check_requ(buff, fds[i].fd);
			if (fds[i].revents & (POLLHUP | POLLERR)){
				//del user if disconnected
				if (i > 0){
					std::cerr << "<fd=" << fds[i].fd << "> IP " <<  inet_ntoa(_client_addr.sin_addr) << ": disconnected" << std::endl;
					close(fds[i].fd);
					fds.erase(fds.begin() + i);
				}
			}
		}
	}
}

std::string getipmachine() {
    char hostname[256];
    struct hostent *host_entry;
    struct in_addr **addr_list;
    int i;

    // Get the hostname
    if (gethostname(hostname, sizeof(hostname)) == -1) {
        perror("Error getting hostname");
        return 0;
    }

    // Get host entry
    if ((host_entry = gethostbyname(hostname)) == NULL) {
        perror("Error getting host entry");
        return 0;
    }

    // Retrieve IP addresses
    addr_list = (struct in_addr **)host_entry->h_addr_list;
    for (i = 0; addr_list[i] != NULL; i++) {
        printf("IP Address %d: %s\n", i + 1, inet_ntoa(*addr_list[i]));
    }

    return inet_ntoa(*addr_list[0]);
}
// Server Setup
void	server::launch(std::string	passwd, std::string	port) {
	_port = my_atoi(port);
	_passwd = passwd;
	_server_sock = open_socket();
	_server = new client(_server_sock);
	_server->setIpAddress(getipmachine()); //anaaaaa 4adi ldaaar


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

	std::cout << "\033[1;42mThe server is listening on the port\033[0m ==> " << "\033[1;41m" << _port << "\033[0m" <<  std::endl;
	std::vector<struct pollfd> fds(MAX_CLIENT + 1);
    // Add the server socket to the fds vector
    fds[0].fd = _server_sock;
    fds[0].events = POLLIN;
    while (IRC) {
        // multiplexing
        setpoll(poll(&fds[0], nfds, -1));
		//accept new clients
		if (fds[0].revents & POLLIN){
			
			int clientSocket = accept(_server_sock, reinterpret_cast<sockaddr *>(&_client_addr), &_c_addr_len);
			if (clientSocket == -1)
				throw	std::runtime_error("Failed accepting a connection : " + std::string(strerror(errno)));

			fds[nfds].fd = clientSocket;
			fds[nfds].events = POLLIN | POLLOUT;
			srand(time(NULL));
			_id = rand() % 1000;
			++nfds;
		}
		//check request
		listofclients(fds);
	}
}

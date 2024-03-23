#include "server.hpp"
#include "client.hpp"
#include "head.hpp"


// std::map<int, std::string> clients;

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
			std::cout << static_cast<char>(ss.c_str()[i]);
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
		}
		else if (!std::strncmp(command.c_str(), "NICK", 4) && Client->getActive()){
			std::string nickname;
			iss >> nickname;

			Client->setNickname(nickname);
		}
		// usleep(100);
	}
	std::cout << "----newclient----\n";
	if (Client)
		Client->printClient();
	std::cout << "----ENDclient----\n";
	_clients.push_back(Client);
}

// void	join(client *c, channel& chan) {
// 	chan.addMember(c);
// }

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

channel*	server::createNewChannel(std::string name, client* creator) {
	channel *newChan = new channel(creator, name, _server);
	std::string tmp;

	newChan->addMember(creator, tmp);
	return newChan;
}

void showcase(std::stringstream &str){
	std::string cmd, target, message;

	str >> cmd >> target >> message ;
	std::cout << "COMMAND: " << cmd << std::endl; 
	std::cout << "TARGET: " << target << std::endl; 
	std::cout << "MESSAGE: " << message << std::endl; 
}

void server::who(client *Client , std::stringstream& os){
	std::cout << "who\n";
	std::string chan;

	os >> chan;
	channel	*ch = getChannel(chan);
	if (!ch) {
		sendMessage(_server, Client, ":" + _server->getIpaddress() + ERR_NOSUCHCHANNEL(Client->getNickname(), chan));
	} else {
		ch->RPL_who(Client);
	}
}

void server::join(client *Client , std::stringstream& os){
	std::cout << "join\n";
	std::string chan, buffer, pass;

	os >> chan >> pass;
	if (chan[0] == '#'){
		channel *ch = getChannel(chan);
		if (!ch){
			ch = createNewChannel(chan, Client);
			_channels.push_back(ch);
		} else {
			ch->addMember(Client, pass);
		}
	}
	else {
		buffer = ":" + _server->getIpaddress() +  ERR_NOSUCHCHANNEL(Client->getNickname(), chan);
		send(Client->getClientsock(), buffer.c_str(), buffer.size(), 0);
	}

}

void server::privmsg(client *Client , std::stringstream& os){
	std::cout << "privmsg\n";
	std::string buffer, nickORchan, msg;

	os >> nickORchan >> msg;
	std::cout << nickORchan << std::endl;
	if  (nickORchan[0] == '#'){
		// Channel message
		channel* ch = getChannel(nickORchan);
		if (ch) {
			size_t pos = _token.find(':');
			if (pos != std::string::npos)
				msg = _token.substr(pos + 1);
			ch->RPL_privmsg(Client, msg);
		} else {
			buffer = ":" + _server->getIpaddress() + ERR_NOSUCHCHANNEL(Client->getNickname(), nickORchan);
			send(Client->getClientsock(), buffer.c_str(), buffer.size(), 0);
		}
	} else {
		// Private Message
		client *c = getClient(nickORchan);
		if (c) {
			size_t pos = _token.find(':');
			if (pos != std::string::npos)
				msg = _token.substr(pos + 1);
			buffer =  ':' + Client->getForm() + " PRIVMSG " + c->getNickname() + " :" + msg + "\r\n"; 
			sendMessage(_server, c, buffer);
		} else {
			buffer = ":" + _server->getIpaddress() + ERR_NOSUCHNICK(Client->getNickname(), nickORchan);
			send(Client->getClientsock(), buffer.c_str(), buffer.size(), 0);
		}
	}
}

void server::topic(client *Client , std::stringstream& os){
	std::cout << "topic\n";
	std::string chan, topic, buffer;

	os >> chan >> topic;
	channel *ch = getChannel(chan);
	if (ch) {
		ch->RPL_topic(Client, topic);
	} else {
		buffer = ":" + _server->getIpaddress() + ERR_NOSUCHCHANNEL(Client->getNickname(), chan);
		send(Client->getClientsock(), buffer.c_str(), buffer.size(), 0);	
	}
}

void server::invite(client *Client , std::stringstream& os){
	std::cout << "invite\n";
	std::string nick, chan, buffer;

	os >> nick >> chan;
	client *c = getClient(nick);
	channel *ch = getChannel(chan);
	if (c) {
		if (ch) {
			ch->RPL_invite(Client, c);
		} else {
			buffer =  ":" + _server->getIpaddress() + ERR_NOSUCHCHANNEL(Client->getNickname(), chan);
			send(Client->getClientsock(), buffer.c_str(), buffer.size(), 0);			
		}
	} else {
		buffer =  ":" + _server->getIpaddress() + ERR_NOSUCHNICK(Client->getNickname(), nick);
		send(Client->getClientsock(), buffer.c_str(), buffer.size(), 0);
	}
}

void server::mode(client *Client , std::stringstream& os){
	std::cout << "mode\n";
	std::string chan, modes, param;

	os >> chan >> modes >> param;
	channel	*ch = getChannel(chan);
	if (ch) {
		if  (!modes.empty()) {
			// SET MODE
			if (ch->isOperator(Client)) {
				ch->validModes(Client, modes, param);
			} else {
				sendMessage(_server, Client, ":" + _server->getIpaddress() + ERR_CHANOPRIVSNEEDED(Client->getNickname(), chan));
			}
		} else {
			// VEIW MODE
			if (!ch->getTopic().empty())
				ch->RPL_topic(Client, "fda");
			ch->RPL_list(Client);
			ch->RPL_mode(Client);
		}
	} else {
		sendMessage(_server, Client,  ": " + _server->getIpaddress() + ERR_NOSUCHCHANNEL(Client->getNickname(), chan));
	}
}
 //PART #chan :Leavin
void server::part(client *Client , std::stringstream& os){
	std::cout << "part\n";
	std::string chan , reason,  response;

	os >> chan >> reason;
	channel *ch = getChannel(chan);
	if (ch) {
		if (!ch->isCreator(Client))
			ch->RPL_part(Client);
	} else {
		response =  ":" + _server->getIpaddress() + ERR_NOSUCHCHANNEL(Client->getNickname(), chan);
		send(Client->getClientsock(), response.c_str(), response.size(), 0);	
	}
}

void server::kick(client *Client , std::stringstream& os){
	std::cout << "kick\n";
	std::string chan, nick, reason, response;


	os >> chan >> nick >> reason;
	
	channel *ch = getChannel(chan);
	client *cl = getClient(nick);
	if (ch) {
		if (cl) {
			ch->RPL_kick(Client, cl, reason);
		} else {
			response =  ":" + _server->getIpaddress() + ERR_NOTONCHANNEL(nick, chan);
			send(Client->getClientsock(), response.c_str(), response.size(), 0);
		}
	} else {
		response =  ":" + _server->getIpaddress() + ERR_NOSUCHCHANNEL(Client->getNickname(), chan);
		send(Client->getClientsock(), response.c_str(), response.size(), 0);			
	}
}

void server::bot(client *Cl, std::stringstream &os){
	std::string mesg, pong;
	size_t pos = _token.find(' ');
	pong = _token.substr(pos + 1);
	msg = server::botstart(pong);
	send(Cl->getClientsock(), msg.c_str(), msg.size(), 0);
}

void	server::handleMsg(std::string& str, int fdClient) { 
	std::stringstream iss(str);
	client *Client = getClient(fdClient);

	std::cout << "----msg----\n";
	std::string arr[] = {"WHO" ,"JOIN" ,"PRIVMSG" ,"TOPIC" ,"INVITE" ,"MODE" , "bot", "KICK", "PART", "QUIT"};

	void (server::*env[9])(client *, std::stringstream&) = {&server::who, &server::join , &server::privmsg, &server::topic, &server::invite, &server::mode, &server::bot, &server::kick, &server::part};
	int i = 0;
	while (std::getline(iss, _token, '\n')){
		std::stringstream os(_token);
		std::string cmd;
		os >> cmd;
		for(; i < 10; ++i){
			if (cmd == arr[i])
			{
				// showcase(s);
				break;
			}
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
				_quit = true;
				break;
			default:
				break;
		}

	}
}


void server::check_requ(std::string str, int fd){
	std::stringstream iss(str);

	if (errno == EPIPE)
		return;
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

void	server::removeChannel(channel *ch) {
	std::string response;

	for (size_t i = 0; i < ch->_members.size(); ++i) {
		ch->RPL_part(ch->_members[i]);
	}

    std::vector<channel*>::iterator it = std::find(_channels.begin(), _channels.end(), ch);
    if (it != _channels.end()) {
        _channels.erase(it);
    }

}


void	server::removeclient(client  *cl) {
    std::vector<client*>::iterator it = std::find(_clients.begin(), _clients.end(), cl);
    if (it != _clients.end()) {
        _clients.erase(it);
    }
}

 //:_i7i!_@88ABE6.25BF1D.D03F86.88C9BD.IP QUIT :Quit: Page closed
void	server::quiteMessege(int fd) {
	std::string response;
	client	*cl = getClient(fd);
	std::cout << "dkhal hna?\n";
	for (size_t i = 0; i < _channels.size(); ++i) {
		if (_channels[i]->isCreator(cl)) { 
			removeChannel(_channels[i]);
		} else if (_channels[i]->isMember(cl)) {
			response = ":" + cl->getForm() + " QUIT :\r\n";
			_channels[i]->removeMember(cl);
			removeclient(cl);
			_channels[i]->sendToAllMembers(response);
		}
	}
	delete cl;
}

void server::listofclients(std::vector<struct pollfd> &fds) {
	for (size_t i = 1; i < fds.size(); ++i){
		std::string all;
		if (fds[i].revents & POLLIN) {
			char buff[BUFFER_SIZE];
			int read;
			while (true) {
				read = recv(fds[i].fd, buff, sizeof(buff) -1, 0);

				if (read > 0 && errno != EPIPE){
					buff[read] = '\0';
					all += buff;
				}
				else if (read <= 0 || errno == EPIPE) {
					// std::cout << "start" << std::endl;
					// std::cout << all << std::endl;
					// std::cout << "end" << std::endl;
					break;
				}
			}
			check_requ(all, fds[i].fd);
			if (fds[i].revents & (POLLHUP | POLLERR) || _quit) {
				//del user if disconnected
				if (i > 0) {
					std::cerr << "<fd=" << fds[i].fd << "> IP " <<  inet_ntoa(_client_addr.sin_addr) << ": disconnected" << std::endl;
					quiteMessege(fds[i].fd);
					close(fds[i].fd);
					fds.erase(fds.begin() + i);
					_quit = false;
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

	socklen_t TEST = sizeof(_server_addr);
	// getsockname(_server_sock, (sockaddr *)&_server_addr, &TEST);
	std::cout << "SERVER IP CONNECTED: " << _server->getIpaddress() << "\n";

	// _server->setIpAddress(inet_ntoa(_server_addr.sin_addr));
	std::cout << "\033[1;42mThe server is listening on the port\033[0m ==> " << "\033[1;41m" << _port << "\033[0m" <<  std::endl;
	std::vector<struct pollfd> fds(MAX_CLIENT + 1);
    // Add the server socket to the fds vector
    fds[0].fd = _server_sock;
    fds[0].events = POLLIN;
	signal(SIGPIPE, SIG_IGN);
    while (IRC) {
        // multiplexing
        setpoll(poll(&fds[0], nfds, -1));
		//accept new clients
		if (fds[0].revents & POLLIN){
			_c_addr_len = sizeof(_client_addr);
			int clientSocket = accept(_server_sock, (struct sockaddr *)(&_client_addr), &_c_addr_len);
			if (clientSocket == -1)
				throw	std::runtime_error("Failed accepting a connection : " + std::string(strerror(errno)));
			fcntl(clientSocket, F_SETFL, O_NONBLOCK) ;
			std::cout << "NEW CLIENT CONNECTED: " << inet_ntoa(_client_addr.sin_addr) << "\n";
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

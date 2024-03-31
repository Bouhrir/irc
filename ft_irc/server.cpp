#include "server.hpp"
#include "client.hpp"
#include "head.hpp"
#include "bot.hpp"
#include <algorithm>
#include <iterator>


// Orthedox Form
server::server() : _port(0), _passwd("password"), nfds(1) {
	_server_sock = 0;
	_server_addr.sin_family = AF_INET;
	_server_addr.sin_port = htons(_port);
	_server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	_addr_len = sizeof(_server_addr);
}

server::~server() {

}

// Getters
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

channel*	server::getChannel(std::string name) {
	for (size_t i = 0; i < _channels.size(); ++i) {
		if (_channels[i]->getName() == name)
			return _channels[i];
	}
	return NULL;
}

// Creation
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
			std::string username, nickname;
			iss >> username >> nickname;

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

channel*	server::createNewChannel(std::string name, client* creator) {
	channel *newChan = new channel(creator, name, _server);
	std::string tmp;

	newChan->addMember(creator, tmp);
	return newChan;
}

// Checkers
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

bool server::validUser(std::string nick){
	std::vector<client *>::iterator it = _clients.begin();
	for (; it != _clients.end();++it){
		if ((*it)->getNickname() == nick)
			return true;
	}
	return false;
}

bool	server::chackIfChannelExists(const std::string name) const {
	for (size_t i = 0; i < _channels.size(); ++i) {
		if (_channels[i]->getName() == name)
			return true;
	}
	return false;
}

// Communication
void	server::quiteMessege(int fd) {
	std::string response;
	client	*cl = getClient(fd);

	for (size_t i = 0; i < _channels.size(); ++i) {
		if (_channels[i]->isCreator(cl)) {
			removeChannel(_channels[i]);
			break ;
		} else if (_channels[i]->isMember(cl)) {
			response = ":" + cl->getForm() + " QUIT :\r\n";
			_channels[i]->sendToAllMembers(response);
			_channels[i]->removeMember(cl);
			break ;
		}
	}
	close(fd);
	removeclient(cl);
	delete cl;
}

void server::sendMessage(client	*from , client *to, const std::string& msg) const {
	std::string	err("Cannot send message: client is not active or socket is invalid");
    if (to->getClientsock() <= 0) {
		// If the recipient is not connected, sends the message to
        send(from->getClientsock(), err.c_str(), err.size(), 0);
    } else {
        send(to->getClientsock(), msg.c_str(), msg.size(), 0);
    }
	return ;
}

void	server::broadcast(const std::string& message) {
	for (size_t i = 0; i < _clients.size(); ++i) {
		sendMessage(_server, _clients[i], message);
	}
}

void	server::handleMsg(std::string& str, int fdClient) { 
	std::stringstream iss(str);
	client *Client = getClient(fdClient);

	if (!Client)
		return ;
	std::cout << "----msg----\n";
	std::string arr[] = {"WHO" ,"JOIN" ,"PRIVMSG" ,"TOPIC" ,"INVITE" ,"MODE" , "bot", "KICK", "PART", "NICK", "USER", "QUIT"};

	void (server::*env[11])(client *, std::stringstream&) = {&server::who, &server::join , &server::privmsg, &server::topic, &server::invite, &server::mode, &server::Emit, &server::kick, &server::part, &server::nick, &server::user};
	int i = 0;
	while (std::getline(iss, _token, '\n')){
		std::stringstream os(_token);
		std::string cmd;
		os >> cmd;
		for(; i < 12; ++i){
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
				break;
			case 10:
				(this->*(env[10]))(Client, os);
				break;
			case 11:
				_quit = true;
				break;
			default:
				break;
		}

	}
}


// Commands
void server::who(client *Client , std::stringstream& os){
	std::cout << "who\n";
	std::string chan;

	os >> chan;
	channel	*ch = getChannel(chan);
	if (!ch) {
		if (Client)
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
		if (Client){
			buffer = ":" + _server->getIpaddress() +  ERR_NOSUCHCHANNEL(Client->getNickname(), chan);
			send(Client->getClientsock(), buffer.c_str(), buffer.size(), 0);
		}
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
			if (Client){
				buffer = ":" + _server->getIpaddress() + ERR_NOSUCHCHANNEL(Client->getNickname(), nickORchan);
				send(Client->getClientsock(), buffer.c_str(), buffer.size(), 0);
			}
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
			if (Client){
				buffer = ":" + _server->getIpaddress() + ERR_NOSUCHNICK(Client->getNickname(), nickORchan);
				send(Client->getClientsock(), buffer.c_str(), buffer.size(), 0);
			}
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
		if (Client){

		buffer = ":" + _server->getIpaddress() + ERR_NOSUCHCHANNEL(Client->getNickname(), chan);
		send(Client->getClientsock(), buffer.c_str(), buffer.size(), 0);	
		}
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
		if (Client){
			buffer =  ":" + _server->getIpaddress() + ERR_NOSUCHNICK(Client->getNickname(), nick);
			send(Client->getClientsock(), buffer.c_str(), buffer.size(), 0);
		}
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
				if (Client)
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
		if (Client)
			sendMessage(_server, Client,  ": " + _server->getIpaddress() + ERR_NOSUCHCHANNEL(Client->getNickname(), chan));
	}
}

void server::part(client *Client , std::stringstream& os){
	std::cout << "part\n";
	std::string chan , reason,  response;

	os >> chan >> reason;
	channel *ch = getChannel(chan);
	if (ch) {
		if (!ch->isCreator(Client))
			ch->RPL_part(Client);
	} else {
		if (Client){
			response =  ":" + _server->getIpaddress() + ERR_NOSUCHCHANNEL(Client->getNickname(), chan);
			send(Client->getClientsock(), response.c_str(), response.size(), 0);	
		}
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
		if (Client){
			response =  ":" + _server->getIpaddress() + ERR_NOSUCHCHANNEL(Client->getNickname(), chan);
			send(Client->getClientsock(), response.c_str(), response.size(), 0);			
		}
	}
}

void	server::user( client *Client, std::stringstream& os) {
	std::cout << "user\n";
	std::string username, response;

	os >> username;
	if (!username.empty()) {
		Client->setUsername(username);
	} else {
		// Do Nothing
	}
}

void	server::nick( client  *Client, std::stringstream& os) {
	std::cout << "nick\n";
	std::string nick, response;

	os >> nick;

	if (!nick.empty()) {
		client *cl = getClient(nick);

		if (!cl) {
			if (nick[0] == '#') {
				response = ":" + _server->getIpaddress() + ERR_ERRONEUSNICKNAME(Client->getNickname(), nick);
				sendMessage(_server, Client, response);
			} else {
				response = ":"  + Client->getForm() + " NICK :" + nick + "\r\n";
				Client->setNickname(nick);
				broadcast(response);
			}
		} else {
			response = ":" + _server->getIpaddress() + ERR_NICKNAMEINUSE(Client->getNickname(), nick);
			sendMessage(_server, Client, response);
		}
	} else {
		response = ":" + _server->getIpaddress() + ERR_NONICKNAMEGIVEN(Client->getNickname());
		sendMessage(_server, Client, response);
	}
}

void server::Emit( client *Cl, std::stringstream &os){
	std::string chan, msg, response;
	bot tmp;

	response = tmp.botstart(os);
	send(Cl->getClientsock(), response.c_str(), response.size(), 0);
}

// Erasing
void	server::removeChannel(channel *ch) {
	std::string response;

	for (size_t i = 0; i < ch->_members.size(); ++i) {
		ch->RPL_part(ch->_members[i]);
	}

    std::vector<channel*>::iterator it = std::find(_channels.begin(), _channels.end(), ch);
    if (it != _channels.end()) {
        _channels.erase(it);
    }
	delete ch;
}

void	server::removeclient(client  *cl) {
    std::vector<client*>::iterator it = std::find(_clients.begin(), _clients.end(), cl);
    if (it != _clients.end()) {
        _clients.erase(it);
    }
}

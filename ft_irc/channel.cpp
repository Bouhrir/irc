#include"channel.hpp"

// Orthedox Form
channel::channel() {
	_name = "unnamed";
	srand(time(NULL));
	_creationTime = std::to_string(rand()); // creation time
	_inviteOnly = false;	// default is not invite only
	_hasPass = false;		// default has no password set
	_hasTopic = false;		// default the channel doesn't have a topic
	_maxUsers = 25;			// default number of users per channel
}

channel::channel(std::string name, client *srv) : _name(name) {
	_server = srv;
	srand(time(NULL));
	_creationTime = std::to_string(rand()); // creation time
	_inviteOnly = false;	// default is not invite only
	_hasPass = false;		// default has no password set
	_hasTopic = false;		// default the channel doesn't have a topic
	_maxUsers = 25;			// default number of users per channel

}

channel::channel(const channel& other) {
	*this = other;
}

channel& channel::operator=(const channel& other) {
	if (this != &other) {
		_name = other._name;
		_modes = other._modes;
		_creator = other._creator;
		_hasPass = other._hasPass;
		_hasTopic = other._hasTopic;
		_passwd = other._passwd;
		_topic = other._topic;
		_creationTime = other._creationTime;
		_maxUsers = other._maxUsers;
		_server = other._server;
		_members = other._members;
		_invited = other._invited;
		_operators = other._operators;
	}
	return *this;
}

channel::~channel() {
	// do nothing for now .
}



// Setters
void	channel::setName(std::string& Name) {
	_name = Name;
}
void	channel::setPasswd(std::string& Passwd) {
	_passwd = Passwd;
}
void	channel::setTopic(std::string& Topic) {
	_topic = Topic;
}
void	channel::addMember(client *cl) {
	_members.push_back(cl);
	RPL_join(cl);
	
}



// Getters
std::string	channel::getName() {
	return (_name);
}
std::string	channel::getTopic() {
	return (_topic);
}
std::string	channel::getPasswd() {
	return (_passwd);
}
client*		channel::getmember(int fd) {
	for (size_t i = 0; i < _members.size(); ++i) {
		if (fd == _members[i]->getClientsock())
			return  (_members[i]);
	}
	return NULL;
}
std::vector<client*>	 channel::getMembers() {
	return _members;
}



// Checkers
bool	channel::isInvited(client *cl) {
	for (size_t i = 0; i < _invited.size(); ++i) {
		if (cl == _invited[i])
			return true;
	}
	return false;
}
bool	channel::isOperator(client *cl) {
	for (size_t i = 0; i < _operators.size(); ++i) {
		if (cl == _operators[i])
			return true;
	}
	return false;
}
bool	channel::isMember(client *cl) {
	for (size_t i = 0; i < _members.size(); ++i) {
		if (cl == _members[i])
			return true;
	}
	return false;
}



//REPLIES

void	channel::who(client *cl, client *user) {
	std::string response;

	response += ":" + _server->getIpaddress() + " 352 " + cl->getNickname() + " " + _name + " ";
	response += user->getUsername() + " " + user->getUsername() + " " + user->getIpaddress() + " ";
	response += user->getNickname() + " H";
	if (isOperator(user))
		response += "@";
	response += ":0 realname\r\n";
	sendMessage(_server, cl,  response);
}

void	channel::RPL_who(client *cl) {
	for (size_t i = 0; i < _members.size(); ++i) {
		who(cl, _members[i]);
	}
	sendMessage(_server, cl, ":" + _server->getIpaddress() + RPL_ENDOFWHOIS(cl->getNickname(), _name));
}

void	channel::RPL_join(client *cl) {
	std::string response;

	response += ':' + cl->getNickname() + '!' + cl->getUsername() + '@' + cl->getIpaddress();
	response += + " JOIN " + _name + " * :realname\r\n";
	sendToAllMembers(response);
}

void	channel::RPL_mode(client *cl) {

}



// Methods
void channel::sendMessage(client	*from , client *to, const std::string& msg) const {
	std::string	err("Cannot send message: client is not active or socket is invalid");
    if (to->getClientsock() <= 0) {
		// If the recipient is not connected, sends the message to
        send(from->getClientsock(), err.c_str(), err.size(), 0);
    } else {
        send(to->getClientsock(), msg.c_str(), msg.size(), 0);
    }
	return ;
}

bool	check_operator(channel& chan, client	*cl) {
	std::vector<client*>::iterator it;

	it = std::find(chan._operators.begin(), chan._operators.end(), cl);
	if (it == chan._operators.end()) {
		return false;
	} else {
		return true;
	}
	return true;
}

// Methods
void	channel::sendChanInfo(client* cl) {
	std::string users;

	users = ":localhost 353 " +  cl->getNickname() + " = " + _name + " :";
	for (size_t i = 0; i < _members.size(); ++i) {
		users += _members[i]->getForm() + " ";
	}
	users += "\r\n";
	sendMessage(_server, cl, users);
	sendMessage(_server, cl, ":localhost " + RPL_ENDOFNAMES(cl->getNickname(), _name) + "\r\n");
	sendMessage(_server, cl, ":localhost " + RPL_CHANNELMODEIS(cl->getNickname(), _name, _modes) + "\r\n");
	sendMessage(_server, cl, ":localhost " + RPL_CREATIONTIME(cl->getNickname(), _name, _creationTime) + "\r\n");
	sendMessage(_server, cl, ":localhost " + RPL_ENDOFWHOIS(cl->getNickname(), _name) + "\r\n");
}

void	channel::sendToAllMembers(const std::string &msg) {
	for(size_t i = 0; i < _members.size(); ++i) {
		send( _members[i]->getClientsock(), msg.c_str(), msg.size(), 0);
	}
}

void	channel::kick(client *kicker, client *other) {
	std::vector<client*>::iterator it;

	if (check_operator(*this, kicker) == false)
		return (sendMessage(_server, kicker, "You don't have the right to do that."));

	it = std::find(_members.begin(), _members.end(), other);
	if (it == _invited.end()) {
		sendMessage(_server, kicker, "This user is not in this channel");
	} else {
		// removeMember(*it);
		sendMessage(_server, kicker, "User kicked from the channel");
		sendMessage(_server, other, "You have been kicked from the channel");
	}
}

void	channel::invite(client *inviter, client *other) {
	std::vector<client*>::iterator it;

	if (check_operator(*this, inviter) == false)
		return (sendMessage(_server, inviter, "You don't have the right to do that."));

	it = std::find(_invited.begin(), _invited.end(), other);
	if (it != _invited.end()) {
		sendMessage(_server, inviter, "This user is already invited in this channel.");
	} else {
		_invited.push_back(other);
		sendMessage(_server, other, "You have been invited to join the channel " + this->_name + ".");
	}
}

void	channel::topic(client *other) {
	if (check_operator(*this, other) == false)
		return (sendMessage(_server, other, "You don't have the right to do that."));
	// setTopic();
}

void	channel::mode() {

}


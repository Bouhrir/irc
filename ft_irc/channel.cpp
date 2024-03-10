#include"channel.hpp"

// Orthedox Form
channel::channel() {
	_name = "unnamed";
	_creationTime = std::time(NULL); // creation time
	_inviteOnly = false;	// default is not invite only
	_hasPass = false;		// default has no password set
	_hasTopic = false;		// default the channel doesn't have a topic
	_maxUsers = 25;			// default number of users per channel
}

channel::channel(client *creator, std::string name, client *srv) : _name(name) {
	srand(time(NULL));

	_creator = creator;
	_operators.push_back(creator);
	_server = srv;

	_creationTime = std::time(NULL); // creation time
	_inviteOnly = false;	// default is not invite only
	_hasPass = false;		// default has no password set
	_hasTopic = false;	
	_topicrestrictions = false;	// default the channel doesn't have a topic
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
void	channel::setName(std::string Name) {
	_name = Name;
}
void	channel::setPasswd(std::string Passwd) {

	_passwd = Passwd;
}
void	channel::setTopic(std::string Topic) {
	Topic.erase(0, 1); // remove the ":" at the beginning of the
	if (Topic.empty()) {
		_hasTopic = false;
		_topic.clear();
	} else {
		_topicTime = std::time(NULL);
		_hasTopic = true;
		_topic = Topic;
	}
}
void	channel::setMode(char Mode) {
	_modes +=  Mode;
}

void	channel::setMaxUsers(std::string maxUsers) {
	int  iMaxUser = atoi(maxUsers.c_str());
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
	if (_hasTopic)
		return (_topic);
	return ("");
}
std::string	channel::getPasswd() {
	if (_hasPass)
		return (_passwd);
	return ("");
}
client*		channel::getmember(int fd) {
	for (size_t i = 0; i < _members.size(); ++i) {
		if (fd == _members[i]->getClientsock())
			return  (_members[i]);
	}
	return NULL;
}
client*		channel::getmember(std::string name) {
	for (size_t i = 0; i < _members.size(); ++i) {
		if (name == _members[i]->getNickname())
			return  (_members[i]);
	}
	return NULL;
}


std::vector<client*>	 channel::getMembers() {
	return _members;
}

client*		channel::getOperator(int fd) {
	for (size_t i = 0; i < _operators.size(); ++i) {
		if (fd == _operators[i]->getClientsock())
			return  (_operators[i]);
	}
	return NULL;
}
client*		channel::getOperator(std::string name) {
	for (size_t i = 0; i < _operators.size(); ++i) {
		if (name == _operators[i]->getNickname())
			return  (_operators[i]);
	}
	return NULL;
}
std::vector<client*>	 channel::getOperators() {
	return _operators;
}
void	channel::addOperator(client *cl) {
	_operators.push_back(cl);
}

void	channel::addOperator(client *op, std::string name) {
	client *cl = getmember(name);
	std::string response;
	if (name.empty())
		return (sendMessage(_server, op, ":" + _server->getIpaddress() + ERR_NEEDMOREPARAMS(op->getNickname(), "MODE")));
	if (cl) {
		addOperator(cl);
		response = ":" + op->getForm() + " MODE " + _name +  " +o " + name;
		sendToAllMembers(response);
	} else {
		sendMessage(_server, op, ":" + _server->getIpaddress() + ERR_USERNOTINCHANNEL(op->getNickname(), name, _name));
	}
}

void	channel::removeOperator(client *cl) {
	for (std::vector<client*>::iterator it = _operators.begin(); it != _operators.end(); ++it) {
		if (*it == cl) {
			_operators.erase(it);
			break ;
		}
	}
}

void	channel::removeOperator(client *op, std::string name) {
	client *cl = getOperator(name);
	std::string response;

	if (name.empty())
		return (sendMessage(_server, op, ":" + _server->getIpaddress() + ERR_NEEDMOREPARAMS(op->getNickname(), "MODE")));
	if (cl) {
		removeOperator(cl);
		response = ":" + op->getForm() + " MODE " + _name +  " -o " + name;
		sendToAllMembers(response);
	} else {
		//	Do nothing
	}
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
	response += " :0 realname\r\n";
	sendMessage(_server, cl,  response);
}

void	channel::RPL_who(client *cl) {
	for (size_t i = 0; i < _members.size(); ++i) {
		who(cl, _members[i]);
	}
	sendMessage(_server, cl, ":" + cl->getIpaddress() + RPL_ENDOFWHOIS(cl->getNickname(), _name));
}

void	channel::RPL_join(client *cl) {
	std::string response;

	if (_inviteOnly) {
		if (isInvited(cl)) {
			response = ":" + cl->getForm() +  " JOIN " + _name + " * :realname\r\n";
			sendToAllMembers(response);
		} else {
			response = ':' + _server->getIpaddress() + ERR_INVITEONLYCHAN(cl->getNickname(), _name);
			sendMessage(_server, cl, response);
		}
	} else {
		response = ":" + cl->getForm() +  " JOIN " + _name + " * :realname\r\n";
		sendToAllMembers(response);
	}
}


void	channel::RPL_list(client *cl) {
	std::string response;

	response += ":" + _server->getIpaddress() + " 353 " + cl->getNickname() + " = ";
	response += _name + " :";
	for (size_t i = 0; i < _members.size(); ++i) {
		if (isOperator(_members[i]))
			response +=  "@";
		response += _members[i]->getNickname() + " ";
	}
	response += "\r\n";
	sendMessage(_server, cl, response);
}

void	channel::RPL_mode(client *cl) {
	sendMessage(_server, cl, ":" + _server->getIpaddress() + RPL_CHANNELMODEIS(cl->getNickname(), _name, _modes));
	sendMessage(_server, cl, ":" + _server->getIpaddress() + RPL_CREATIONTIME(cl->getNickname(), _name, std::to_string(_creationTime)));
}

void	channel::RPL_invite(client *inviter, client *invited) {
	std::string response;

	if (isOperator(inviter)) {
		if (!isMember(invited)) {
			_invited.push_back(invited);
			response = ":" + _server->getIpaddress() + RPL_INVITING(inviter->getNickname(), invited->getNickname(), _name);
			sendMessage(_server, inviter, response);
			response = ":" + inviter->getForm() + " INVITE " + invited->getNickname() + " " +  _name + "\r\n";
			sendMessage(_server, invited, response);
		} else {
			response = ":" + _server->getIpaddress() + ERR_USERONCHANNEL(inviter->getNickname(), invited->getNickname(), _name);
			sendMessage(_server, inviter, response);
		}
	} else {
		response = ":" + _server->getIpaddress() + ERR_CHANOPRIVSNEEDED(inviter->getNickname(), invited->getNickname());
		sendMessage(_server, inviter, response);
	}
}

void	channel::RPL_privmsg(client *cl, std::string& msg) {
	std::string response;

	response = ':' + cl->getForm() + " PRIVMSG " + _name + " :" + msg + "\r\n";

}

void	channel::RPL_topic(client *cl, std::string topic) {
	std::string response;

	// VIEW TOPIC
	if (topic.find(':') == std::string::npos) {
		if (isMember(cl)) {
			if (_hasTopic) {
				response = ":" + _server->getIpaddress() + RPL_TOPIC(cl->getNickname(), _name, _topic);
				sendMessage(_server, cl, response);
				response = ":" + _server->getIpaddress() + RPL_TOPICWHOTIME(cl->getNickname(), _name, _topicSetter->getNickname(), std::to_string(_topicTime));
				sendMessage(_server, cl, response);
			} else {
				response = ":" + _server->getIpaddress() + RPL_NOTOPIC(cl->getNickname(), _name);
				sendMessage(_server, cl, response);
			}
		} else {
			response = ":" + _server->getIpaddress() + ERR_NOTONCHANNEL(cl->getNickname(), _name);
			sendMessage(_server, cl, response);
		}
		return ;
	}

	// CHANGE TOPIC
	if (isOperator(cl)) {
		if (_modes.find('t') == std::string::npos) {
			_topicSetter = cl;
			setTopic(topic);
			response = ":" + cl->getForm() +  " TOPIC " + _name + " " + _topic + "\r\n";
			sendMessage(_server, cl, response);
		} else {
			response = ":" + _server->getIpaddress() + ERR_CHANOPRIVSNEEDED(cl->getNickname(), _name);
			sendMessage(_server, cl, response);
		}
	} else {
		response = ":" + _server->getIpaddress() + ERR_CHANOPRIVSNEEDED(cl->getNickname(), _name);
		sendMessage(_server, cl, response);
	}
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

void	channel::sendToAllMembers(const std::string &msg) {
	for(size_t i = 0; i < _members.size(); ++i) {
		send( _members[i]->getClientsock(), msg.c_str(), msg.size(), 0);
	}
}

void channel::validModes(client *cl, std::string& modes, std::string param) {
	std::string tmp;
	char	sign = modes[0];
	size_t i = 0;
	if (sign == '+')
		i++;
	if (sign != '-') {
		for (; i < modes.size(); ++i) {
			if (modes[i] == 'i' || modes[i] == 't' || modes[i] == 'o' || modes[i] == 'k' || modes[i] == 'l')
				addMode(cl, modes[i], param);
			else
				sendMessage(_server, cl, ":" + _server->getIpaddress() + ERR_UNKNOWNMODE(cl->getNickname(), modes[i]));
		}
	} else {
		for (i = 1; i < modes.size(); ++i) {
			if (modes[i] == 'i' || modes[i] == 't' || modes[i] == 'o' || modes[i] == 'k' || modes[i] == 'l')
				removeMode(cl, modes[i], param);
			else
				sendMessage(_server, cl, ":" + _server->getIpaddress() + ERR_UNKNOWNMODE(cl->getNickname(), modes[i]));
		}
	}
	modes = tmp;
}

void	channel::addMode(client *cl, char c, std::string param) {
	if (c == 't') {
		_topicrestrictions = true;
	} else if (c == 'i') {
		_inviteOnly = true;
	} else if (c == 'o') {
		addOperator(cl, param);
	} else if (c == 'k') {
		if (param.empty())
			return (sendMessage(_server, cl, ":" + _server->getIpaddress() + ERR_NEEDMOREPARAMS(cl->getNickname(), "MODE")));
		_hasPass = true;
		setPasswd(param);
	} else if (c == 'l') {
		if (param.empty())
			return (sendMessage(_server, cl, ":" + _server->getIpaddress() + ERR_NEEDMOREPARAMS(cl->getNickname(), "MODE")));
		setMaxUsers(param);
	}
	setMode(c);
}

void	channel::removeMode(client *cl, char c, std::string param) {
	if (c == 't') {
		_topicrestrictions = false;
	} else if (c == 'i') {
		_inviteOnly = false;
	} else if (c == 'o') {
		removeOperator(cl, param);
	} else if (c == 'k') {
		_hasPass = false;
	} else if (c == 'l') {
		setMaxUsers("25"); // Default to 25 users in a channel
	}
	_modes.erase(_modes.find(c), 1);
}


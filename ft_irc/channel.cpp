#include"channel.hpp"


// Orthedox Form
channel::channel() {
	_name = "unnamed";
	_users = 0;
	_creationTime = std::time(NULL); // creation time
	_inviteOnly = false;	// default is not invite only
	_hasPass = false;		// default has no password set
	_hasTopic = false;		// default the channel doesn't have a topic
	_userLimit = false;
	_maxUsers = 0;			// default number of users per channel
}

channel::channel(client *creator, std::string name, client *srv) : _name(name) {
	srand(time(NULL));

	_users = 0;
	_creator = creator;
	_operators.push_back(creator);
	_server = srv;

	_creationTime = std::time(NULL); // creation time
	_inviteOnly = false;	// default is not invite only
	_hasPass = false;		// default has no password set
	_hasTopic = false;	
	_topicrestrictions = false;	// default the channel doesn't have a topic
	_userLimit = false;
	_maxUsers = 0;			// default number of users per channel

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
		_userLimit =  other._userLimit;
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

void	channel::setMaxUsers(int maxUsers) {
	_maxUsers = maxUsers;
}

void	channel::addMember(client *cl, std::string &pass) {
	if (_hasPass) {
		if (!validPASS(pass)) 
			return (sendMessage(_server, cl, ":" + _server->getIpaddress() + ERR_INVALIDKEY(cl->getNickname(), pass)));
	}

	if (_userLimit) {
		if (_users + 1 > _maxUsers)
			return (sendMessage(_server, cl, ":" + _server->getIpaddress() + ERR_CHANNELISFULL(cl->getNickname(), getName())));
	}

	if (_inviteOnly == true) {
		if (isInvited(cl)) {
			_members.push_back(cl);
			_users++;
			RPL_join(cl);
		} else {
			sendMessage(_server, cl, ":" + _server->getIpaddress() + ERR_INVITEONLYCHAN(cl->getNickname(), getName()));
		}
	} else {
		_members.push_back(cl);
		_users++;
		RPL_join(cl);
	}
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
		response = ":" + op->getForm() + " MODE " + _name +  " +o " + name + "\r\n";
		sendToAllMembers(response);
	} else {
		sendMessage(_server, op, ":" + _server->getIpaddress() + ERR_USERNOTINCHANNEL(op->getNickname(), name, _name));
	}
}

void	channel::removeOperator(client *cl) {
    std::vector<client*>::iterator it = std::find(_operators.begin(), _operators.end(), cl);
    if (it != _operators.end()) {
        _operators.erase(it);
    }
}

void	channel::removeOperator(client *op, std::string name) {
	client *cl = getOperator(name);
	std::string response;

	if (cl == _creator) {
		response =  ":" + _server->getIpaddress() + ERR_INVALIDMODEPARAM(op->getNickname(), _name, "o", cl->getNickname(), "Can't Remove The Creator Of The Channel");
		return (sendMessage(_server, op, response));
	}
	if (name.empty())
		return (sendMessage(_server, op, ":" + _server->getIpaddress() + ERR_NEEDMOREPARAMS(op->getNickname(), "MODE")));
	if (cl) {
		removeOperator(cl);
		response = ":" + op->getForm() + " MODE " + _name +  " -o " + name + "\r\n";
		sendToAllMembers(response);
	} else {
		sendMessage(_server, op, ":" + _server->getIpaddress() + ERR_USERNOTINCHANNEL(op->getNickname(), name, _name));
	}
}

void	channel::removeMember(client *cl) {
    std::vector<client*>::iterator it = std::find(_members.begin(), _members.end(), cl);
    if (it != _members.end()) {
        _members.erase(it);
    }
	_users--;
}


void	channel::removeInvited(client *cl) {
    std::vector<client*>::iterator it = std::find(_invited.begin(), _invited.end(), cl);
    if (it != _invited.end()) {
        _invited.erase(it);
    }
}
// Checkers
bool	channel::isCreator(client *cl) {
	if (cl == _creator)
		return true;
	return false;
}
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
	sendMessage(_server, cl, ":" + _server->getIpaddress() + RPL_ENDOFWHOIS(cl->getNickname(), _name));
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
		if (cl){
			response = ":" + cl->getForm() +  " JOIN " + _name + " * :realname\r\n";
			sendToAllMembers(response);
		}
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
	response = ":" + _server->getIpaddress() + RPL_ENDOFNAMES(cl->getNickname(), _name);
	sendMessage(_server, cl, response);
}

void	channel::RPL_mode(client *cl) {
	sendMessage(_server, cl, ":" + _server->getIpaddress() + RPL_CHANNELMODEIS(cl->getNickname(), _name, _modes));
	sendMessage(_server, cl, ":" + _server->getIpaddress() + RPL_CREATIONTIME(cl->getNickname(), _name, std::to_string(_creationTime)));
}

void	channel::RPL_part(client *cl) {
	std::string response;

	if (isMember(cl)) {
		if (isOperator(cl))
			removeOperator(cl);
		if (cl){
			response = ":" + cl->getForm() + " PART " + _name + " :leaving channel\r\n";
			sendToAllMembers(response);
			removeMember(cl);
		}
	} else {
		response = ":" + _server->getIpaddress() + ERR_NOTONCHANNEL(cl->getNickname(), _name);
		sendMessage(_server, cl, response);
	}
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
	sendToAllMembersBut(response, cl);
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
			sendToAllMembers(response);
		} else {
			response = ":" + _server->getIpaddress() + ERR_INVALIDMODEPARAM(cl->getNickname(), _name, "TOPIC", topic, "Channel Topic Restrection Are On");
			sendMessage(_server, cl, response);
		}
	} else {
		response = ":" + _server->getIpaddress() + ERR_CHANOPRIVSNEEDED(cl->getNickname(), _name);
		sendMessage(_server, cl, response);
	}
}

//:fhihi!fouad@88ABE6.25BF1D.D03F86.88C9BD.IP KICK #chan _i7i :_i
void	channel::RPL_kick(client *cl, client *kick, std::string  reason) {
	std::string response;

	if (isCreator(cl)) {
		if (isMember(kick)) {
			if (isOperator(kick)) 
				removeOperator(kick);
			if (isInvited(kick))
				removeInvited(kick);
			response = ":" + cl->getForm() + " KICK " + _name +  " " + kick->getNickname() + " " + reason + "\r\n";
			sendToAllMembers(response);
			removeMember(kick);
		} else {
			response =  ":" + _server->getIpaddress() + ERR_NOTONCHANNEL(kick->getNickname(), _name);
			sendMessage(_server, cl, response);
		}
	} else if (isOperator(cl) && !isOperator(kick)) {
		if (isMember(kick)) {
			if (isInvited(kick))
				removeInvited(kick);
			response = ":" + cl->getForm() + " KICK " + _name +  " " + kick->getNickname() + " " + reason + "\r\n";
			sendToAllMembers(response);
			removeMember(kick);
		} else {
			response =  ":" + _server->getIpaddress() + ERR_NOTONCHANNEL(kick->getNickname(), _name);
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

void	channel::sendToAllMembersBut(const std::string &msg, const client *cl) {
	for(size_t i = 0; i < _members.size(); ++i) {
		if (_members[i] != cl)
			send( _members[i]->getClientsock(), msg.c_str(), msg.size(), 0);
	}
}

void channel::validModes(client *cl, std::string& modes, std::string param) {
	char	sign = modes[0];
	size_t	i = 0;

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
}

void	channel::addMode(client *cl, char c, std::string param) {
	if (c == 't') {
	changeRestrictions(cl, 1);
	} else if (c == 'i') {
		makeInviteOnly(cl, 1);
	} else if (c == 'o') {
		addOperator(cl, param);
	} else if (c == 'k') {
		changeKey(cl, 1, param);
	} else if (c == 'l') {
		changeMaxUsers(cl, 1, param);
	}
}

void	channel::removeMode(client *cl, char c, std::string param) {
	if (c == 't') {
		changeRestrictions(cl, 0);
	} else if (c == 'i') {
		makeInviteOnly(cl, 0);
	} else if (c == 'o') {
		removeOperator(cl, param);
	} else if (c == 'k') {
		changeKey(cl, 0, param);
	} else if (c == 'l') {
		changeMaxUsers(cl, 0, param);
	}
	if (_modes.find(c) != std::string::npos)
		_modes.erase(_modes.find(c), 1);
}




 //:fhihi!fouad@88ABE6.25BF1D.D03F86.88C9BD.IP MODE #ch +i
void	channel::changeRestrictions(client *cl, int t) {
	std::string response;

	if (t == 1) {
		_topicrestrictions = true;
		setMode('t');
		response = ":" + cl->getForm() + " MODE "  + _name + " +t\r\n"; 

	} else {
		_topicrestrictions = false;
		response = ":" + cl->getForm() + " MODE "  + _name + " -t\r\n"; 
	}
	sendToAllMembers(response);
}
void	channel::makeInviteOnly(client *cl, int t) {
	std::string response;

	if (t == 1) {
		// sets invite only
		_inviteOnly = true;
		response = ":" + cl->getForm() + " MODE "  + _name + " +i\r\n"; 
		setMode('i');
	} else  {
		// removes invite only
		_inviteOnly = false;
		response = ":" + cl->getForm() + " MODE "  + _name + " -i\r\n"; 
	}
	sendToAllMembers(response);
}

static int convertKey(const std::string& str) {
    // Check each character in the string
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
        if (!isdigit(*it)) {
            return 0;
        }
    }

    std::istringstream iss(str);
    int result;
    iss >> result;

    if (result <= 0) {
        return 0;
    }
    return result;
}

//>> :fhihi!fouad@88ABE6.25BF1D.D03F86.88C9BD.IP MODE #hy +l 12
void	channel::changeMaxUsers(client *cl, int t, std::string  &maxusers) {
	std::string response;
	int max;

	if (t == 1) {
		if (maxusers.empty()) {
			response = ":" + _server->getIpaddress() + ERR_NEEDMOREPARAMS(cl->getNickname(), "MODE");
			return (sendMessage(_server, cl, response));
		}
		max = convertKey(maxusers);
		if (max == 0) {
			response = ":" + _server->getIpaddress() + ERR_INVALIDMODEPARAM(cl->getNickname(), _name, "l", maxusers, "Invalid Number.");
			sendMessage(_server, cl, response);
		} else if (max < _users) {
			response = ":" + _server->getIpaddress() + ERR_INVALIDMODEPARAM(cl->getNickname(), _name, "l", maxusers, "Cannot set limit lower than existing members.");
			sendMessage(_server, cl, response);
		} else {
			_userLimit = true;
			setMaxUsers(max);
			setMode('l');
			response = ":" + cl->getForm() + " MODE "  + _name + " +l " + maxusers + "\r\n"; 
			sendMessage(_server, cl, response);
		}
	} else {
		_userLimit = false;
		response = ":" + cl->getForm() + " MODE "  + _name + " -l\r\n"; 
		sendMessage(_server, cl, response);
	}
}

bool	channel::validPASS(std::string pass) {
	if (pass == _passwd)
		return (true);
	return (false);
}

void	channel::changeKey(client *cl, int t, std::string  &Key) {
	std::string response;

	if (t == 1) {
		if (Key.empty()) {
			response = ":" + _server->getIpaddress() + ERR_NEEDMOREPARAMS(cl->getNickname(), "MODE");
			return (sendMessage(_server, cl, response));
		}
		setPasswd(Key);
		setMode('k');
		_hasPass = true;
		response = ":" + cl->getForm() + " MODE "  + _name + " +k " + Key + "\r\n";
		sendMessage(_server, cl, response);
	} else {
		_hasPass = false;
		response = ":" + cl->getForm() + " MODE "  + _name + " -k " + Key + "\r\n";
		sendMessage(_server, cl, response);
	}
}

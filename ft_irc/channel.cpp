#include"channel.hpp"


// Orthedox Form
channel::channel() {

}

channel::channel(std::string name, std::string passwd, std::string topic) : _name(name), _passwd(passwd), _topic(topic) {
}

channel::channel(const channel& other) {
	*this = other;
}

channel& channel::operator=(const channel& other) {
	if (this != &other) {
	}
	return *this;
}

channel::~channel() {
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
	sendChanInfo(cl);
	_members.push_back(cl);
	puts("here");
	sendToAllMembers(':' + cl->getNickname() + '!' + cl->getUsername() + '@' + cl->getIpaddress() + " JOIN " + _name + " * :realname\r\n");
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

void	channel::sendChanInfo(client*cl) {
	std::string pong;
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


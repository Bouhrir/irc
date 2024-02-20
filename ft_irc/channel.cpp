#include"channel.hpp"


// Orthedox Form
channel::channel() {

}

channel::channel(std::string& name, std::string& passwd, std::string& topic) : _name(name), _passwd(passwd), _topic(topic) {
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
// Utils
void	sendMessage(client *other, std::string  msg) {
	
}

bool	check_operator(channel& chan, client	*cl) {
	std::list<client*>::iterator it;

	it = std::find(chan._operators.begin(), chan._operators.end(), cl);
	if (it == chan._operators.end()) {
		return false;
	} else {
		return true;
	}
	return true;
}

// Methods
void	channel::kick(client *kicker, client *other) {
	std::list<client*>::iterator it;

	if (check_operator(*this, kicker) == false)
		return (sendMessage(kicker, "You don't have the right to do that."));

	it = std::find(_members.begin(), _members.end(), other);
	if (it == _invited.end()) {
		sendMessage(kicker, "This user is not in this channel");
	} else {
		_members.erase(it);
		sendMessage(kicker, "User kicked from the channel");
		sendMessage(other, "You have been kicked from the channel");
	}
}

void	channel::invite(client *inviter, client *other) {
	std::list<client*>::iterator it;

	if (check_operator(*this, inviter) == false)
		return (sendMessage(inviter, "You don't have the right to do that."));

	it = std::find(_invited.begin(), _invited.end(), other);
	if (it != _invited.end()) {
		sendMessage(inviter, "This user is already invited in this channel.");
	} else {
		_invited.push_back(other);
		sendMessage(other, "You have been invited to join the channel " + this->_name + ".");
	}
}

void	channel::topic(client *other) {
	if (check_operator(*this, other) == false)
		return (sendMessage(other, "You don't have the right to do that."));
	// setTopic()
}

void	channel::mode() {

}


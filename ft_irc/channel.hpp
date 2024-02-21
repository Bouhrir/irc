#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include"head.hpp"
#include<list>

class	client;

class	channel {
private:
	std::string			_name;
	std::string			_passwd;
	std::string			_topic;

public:

	std::list<client*>	_members;
	std::list<client*>	_invited;
	std::list<client*>	_operators;

	// Orthedox Form
	channel();
	channel(std::string& name, std::string& passwd, std::string& topic);
	channel(const channel& other);
	channel& operator=(const channel& other);
	~channel();

	// Setters
	void	setName(std::string& Name);
	void	setPasswd(std::string& Passwd);
	void	setTopic(std::string& Topic);

	// Getters
	std::string	getName();
	std::string	getTopic();
	std::string	getPasswd();

	// Methods
	void	kick(client *kicker, client *other);
	void	invite(client *inviter, client *other);
	void	topic(client *other);
	void	mode();
};

#endif
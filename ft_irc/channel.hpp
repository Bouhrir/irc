#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include"head.hpp"
#include<vector>

class	server;
class	client;


#define ERR_NEEDMOREPARAMS(nick, command)		(" 461 " + nick + " " + command + " :Not enough parametes\r\n")
#define ERR_ALREADYREGISTERED(nick)				(" 462 " + nick + " :You may not reregister\r\n")
#define ERR_PASSWDMISMATCH(nick)				(" 464 " + nick + " :Password incorrect\r\n")
#define ERR_CHANNELISFULL(nick, chan)			(" 471 " + nick + " " + chan + " :Cannot join channel (+l)\r\n")
#define ERR_UNKNOWNMODE(nick, mode)				(" 472 " + nick + " " + mode + " :is unknown mode char to me\r\n")
#define ERR_INVITEONLYCHAN(nick, chan)			(" 473 " + nick + " " + chan + " :Cannot join channel (+i)\r\n")
#define ERR_BANEDFROMCHAN(nick, chan)			(" 473 " + nick + " " + chan + " :Cannot join channel (+b)\r\n")
#define ERR_BADCHANNELKEY(nick, chan)			(" 475 " + nick + " " + chan + " :cannot join channel (+k)\r\n")
#define ERR_BADCHANMASK(chan)					(" 476 " + chan + " :Bad Channel Mask\r\n")
#define ERR_USERONCHANNEL(user ,nick , chan)	(" 443 " + user + " " + nick + " " + chan + " :is already on channel\r\n")
#define ERR_INVITING(user, nick, channel)		(" 341 " + user + " " + nick + " " + chan + "\r\n")
#define ERR_CHANOPRIVSNEEDED(user, chan)		(" 482 " + user + " " + chan + " You're not channel operator\r\n")
#define ERR_NOTONCHANNEL(user, chan)			(" 443 " + user + " " + nick + " " + chan + " You're not on that channel\r\n") 
#define RPL_ENDOFNAMES(user, chan)				(" 366 " + user + " " + chan +  ":End of /NAMES list")
#define	RPL_CHANNELMODEIS(user, chan, mode)		(" 324 " + user + " " + chan + " +" + mode + "\r\n")
#define RPL_CREATIONTIME(user, chan, time)		(" 329 " + user + " " + chan + " " + time + "\r\n")
#define RPL_ENDOFWHOIS(user, chan)				(" 315 " + user + " " + chan + " End of /WHOIS list\r\n")

class	channel {
private:
	bool				_inviteOnly;

	std::string			_name;
	std::string			_modes;
	std::string			_creator;

	bool				_hasPass;
	std::string			_passwd;

	bool				_hasTopic;
	std::string			_topicSetter;
	std::string			_topic;

	std::string			_creationTime;

	int					_maxUsers;
	client*				_server;

	channel();
public:

	std::vector<client*>	_members;
	std::vector<client*>	_invited;
	std::vector<client*>	_operators;

	// Orthedox Form
	channel(std::string name, client *srv);
	channel(const channel& other);
	channel& operator=(const channel& other);
	~channel();

	// Setters
	void	setName(std::string& Name);
	void	setPasswd(std::string& Passwd);
	void	setTopic(std::string& Topic);
	void	addMember(client *cl);
	// void	removeMember(client *cl);


	// Getters
	std::string				getName();
	std::string				getTopic();
	std::string				getPasswd();
	client*					getmember(int fd);
	std::vector<client*>	getMembers();


	// Checkers
	bool	isInvited(client *cl);
	bool	isOperator(client *cl);
	bool	isMember(client *cl);


	//REPLIES
	void	who(client *cl, client *user);
	void	RPL_join(client *cl);
	void	RPL_who(client *cl);
	void	RPL_mode(client *cl);


	// Methods
	void	sendToAllMembers(const std::string &msg);
	void	sendMessage(client	*from , client *to, const std::string& msg) const;
	void	sendChanInfo(client*cl);


	void	kick(client *kicker, client *other);
	void	invite(client *inviter, client *other);
	void	topic(client *other);
	void	mode();
};

#endif
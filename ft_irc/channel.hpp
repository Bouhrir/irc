#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include"head.hpp"
#include<vector>
#include <ctime>

class	server;
class	client;


#define ERR_BADCHANMASK(chan)							(" 476 " + chan + " :Bad Channel Mask\r\n")

// REGISTRING
#define ERR_NEEDMOREPARAMS(nick, command)				(" 461 " + nick + " " + command + " :Not enough parametes\r\n")
#define ERR_ALREADYREGISTERED(nick)						(" 462 " + nick + " :You may not reregister\r\n")
#define ERR_PASSWDMISMATCH(nick)						(" 464 " + nick + " :Password incorrect\r\n")

// MODES
#define ERR_INVALIDMODEPARAM(nick, chanuser, mode, param, desc)		(" 696 " + nick + " " + chanuser + " " + mode + " " + param + " " + desc + "\r\n")
#define ERR_CHANNELISFULL(nick, chan)								(" 471 " + nick + " " + chan + " :Cannot join channel (+l)\r\n")
#define ERR_UNKNOWNMODE(nick, mode)									(" 472 " + nick + " " + mode + " :is unknown mode char to me\r\n")
#define ERR_INVITEONLYCHAN(nick, chan)								(" 473 " + nick + " " + chan + " :Cannot join channel (+i)\r\n")
#define ERR_BADCHANNELKEY(nick, chan)								(" 475 " + nick + " " + chan + " :cannot join channel (+k)\r\n")
#define ERR_USERNOTINCHANNEL(user, nick , chan)						(" 441 " + user + " " + nick + " " + chan + " :They aren't on that channel\r\n")
#define ERR_INVALIDKEY(nick, chan)									(" 525 " + nick + " " + chan + " :Key is not well-formed\r\n")

// INVITING 
#define RPL_INVITING(user, nick, chan)					(" 341 " + user + " " + nick + " " + chan + "\r\n")
#define ERR_USERONCHANNEL(user ,nick , chan)			(" 443 " + user + " " + nick + " " + chan + " :is already on channel\r\n")

// TOPIC
#define	RPL_NOTOPIC(user, chan)							(" 331 " + user + " " + chan + " :No topic is set\r\n")
#define	RPL_TOPIC(user, chan, topic)					(" 332 " + user + " " + chan + " "  + topic + "\r\n")
#define	RPL_TOPICWHOTIME(user, chan, setter, time)		(" 333 " + user + " " + chan + " "  + setter + " " + time + "\r\n")


//GENERAL
// #define ERR_

#define ERR_CHANOPRIVSNEEDED(user, chan)				(" 482 " + user + " " + chan + " You're not channel operator\r\n")
#define ERR_NOTONCHANNEL(user, chan)					(" 443 " + user + " " + chan + " You're not on that channel\r\n") 
#define RPL_ENDOFNAMES(user, chan)						(" 366 " + user + " " + chan + " :End of /NAMES list\r\n")
#define	RPL_CHANNELMODEIS(user, chan, mode)				(" 324 " + user + " " + chan + " +" + mode + "\r\n")
#define RPL_CREATIONTIME(user, chan, time)				(" 329 " + user + " " + chan + " " + time + "\r\n")
#define RPL_ENDOFWHOIS(user, chan)						(" 315 " + user + " " + chan + " End of /WHOIS list\r\n")

class	channel {
private:
	bool				_inviteOnly;

	std::string			_name;
	std::string			_modes;
	int					_users;

	bool				_hasPass;
	std::string			_passwd;

	bool				_hasTopic;
	bool				_topicrestrictions;
	client*				_topicSetter;
	std::string			_topic;
	std::time_t			_topicTime;

	client*				_creator;
	std::time_t			_creationTime;

	bool				_userLimit;
	int					_maxUsers;

	client*				_server;

	channel();
public:

	std::vector<client*>	_members;
	std::vector<client*>	_invited;
	std::vector<client*>	_operators;

	// Orthedox Form
	channel(client *creator, std::string name, client *srv);
	channel(const channel& other);
	channel& operator=(const channel& other);
	~channel();

	// Setters
	void	setName(std::string Name);
	void	setPasswd(std::string Passwd);
	void	setTopic(std::string Topic);
	void	setMode(char Mode);
	void	setMaxUsers(int maxUsers);


	void	addMember(client *cl, std::string &pass);
	void	addOperator(client *cl);
	void	addOperator(client *op, std::string name);
	void	removeOperator(client *cl);
	void	removeOperator(client *op, std::string name);
	void	removeMember(client *cl);
	void	removeInvited(client *cl);
	// void	removeMember(client *cl);


	// Getters
	std::string				getName();
	std::string				getTopic();
	std::string				getModes();
	std::string				getPasswd();
	client*					getmember(int fd);
	client*					getmember(std::string name);
	std::vector<client*>	getMembers();
	client*					getOperator(int fd);
	client*					getOperator(std::string name);
	std::vector<client*>	getOperators();

	// Checkers
	bool	isCreator(client *cl);
	bool	isInvited(client *cl);
	bool	isOperator(client *cl);
	bool	isMember(client *cl);

	//REPLIES
	void	who(client *cl, client *user);
	void	RPL_join(client *cl);
	void	RPL_who(client *cl);
	void	RPL_list(client *cl);
	void	RPL_mode(client *cl);
	void	RPL_part(client *cl);
	void	RPL_invite(client *inviter, client *invited);
	void	RPL_privmsg(client *cl, std::string& msg);
	void	RPL_topic(client *cl, std::string topic);
	void	RPL_kick(client *cl, client *kick, std::string  reason);


	// Methods
	void	sendToAllMembers(const std::string &msg);
	void	sendToAllMembersBut(const std::string &msg, const client *cl);
	void	sendMessage(client	*from , client *to, const std::string& msg) const;

	void	validModes(client *cl, std::string& modes, std::string param);
	void	addMode(client *cl, char c, std::string param);
	void	removeMode(client *cl, char c, std::string param);
	
	void	makeInviteOnly(client *cl, int t);
	void	changeMaxUsers(client *cl, int t, std::string  &maxusers);
	void	changeKey(client *cl, int t, std::string  &key);
	void	changeRestrictions(client *cl, int t);
	bool	validPASS(std::string );
};

#endif
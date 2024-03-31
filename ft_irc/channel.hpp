#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include"head.hpp"
#include <vector>
#include <ctime>

class	server;
class	client;


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
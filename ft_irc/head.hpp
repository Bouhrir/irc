#ifndef HEAD_HPP
#define HEAD_HPP

#include <iostream>
#include <cstdlib>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include "channel.hpp"
#include "client.hpp"
#include <vector>
#include <fcntl.h>


#define TCP SOCK_STREAM

#define ERR_NICKNAMEINUSE(user, nickname)   (" 433 " + user + " " + nickname + " :Nickname is already in use\r\n")
#define ERR_ERRONEUSNICKNAME(user, nick)    (" 432 " + user + " " + nick + " :Erroneus nickname\r\n");
#define ERR_NONICKNAMEGIVEN(user)           (" 431 " + user + " : No nickname given\r\n")  
#define ERR_NOSUCHNICK(user, nickname)	    (" 401 " + user + " " + nickname + " :No such nick\r\n")
#define ERR_NOSUCHCHANNEL(user, chan)	    (" 403 " + user + " " + chan + " :No such channel\r\n")
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

class client;
class channel;

#endif
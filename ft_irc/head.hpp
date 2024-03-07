#ifndef HEAD_HPP
#define HEAD_HPP

#include <iostream>
#include <cstdlib>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include "error.hpp"
#include "channel.hpp"
#include "client.hpp"
#include <vector>
#include <fcntl.h>


#define TCP SOCK_STREAM
// #define	BUFFER_SIZE 2048
#define ERR_NOSUCHNICK(client, nickname) ("401 " + client + " " + nickname + " :No such nick\r\n")
#define ERR_NOSUCHCHANNEL(client, channel) ("403 " + client + " " + channel + " :No such channel\r\n")
class	commands {
private:

public:
};

// // this is the definition of struct sockaddr ;
// struct sockaddr {
// 	__uint8_t       sa_len;         /* total length */
// 	sa_family_t     sa_family;      /* [XSI] address family */
// 	char            sa_data[14];    /* [XSI] addr value (actually larger) */
// };

// // same as before but for IPv4 
// struct sockaddr_in {
// 	__uint8_t       sin_len;
// 	sa_family_t     sin_family;
// 	in_port_t       sin_port;
// 	struct  in_addr sin_addr;
// 	char            sin_zero[8];
// };

class client;
class channel;

void sendMessage(client	*from , client *to, const std::string& msg);
// int	open_socket();

#endif
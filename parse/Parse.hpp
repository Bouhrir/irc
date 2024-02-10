/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obouhrir <obouhrir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 17:19:20 by obouhrir          #+#    #+#             */
/*   Updated: 2024/02/10 15:10:33 by obouhrir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 


#include <iostream>
#include <algorithm>
#include <cstring>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/select.h>
#include <map>
#include <poll.h>
#include <set>


#define MAX_CLIENT 1

using std::string;
using std::cout;
using std::endl;

typedef struct s_client {
	int socket;
	string nickname;
	string username;
	bool op;
}t_client;

typedef std::map<int, t_client> map;

class Parse {
	private:
		string port;
		string password;
		// std::map<string , std::set<int> > serv;
	public:
		Parse();
		Parse(string const &, string const &);
		Parse(Parse const &);
		string getPort() const;
		string getPassword() const;

		
		Parse &operator=(Parse const &);
		~Parse();		
};
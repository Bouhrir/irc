/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obouhrir <obouhrir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 17:19:20 by obouhrir          #+#    #+#             */
/*   Updated: 2024/01/19 11:04:20 by obouhrir         ###   ########.fr       */
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

using std::string;
using std::cout;
using std::endl;

class Parse {
	private:
		string port;
		string password;
	public:
		Parse();
		Parse(string const &, string const &);
		Parse(Parse const &);
		string getPort() const;
		string getPassword() const;

		
		Parse &operator=(Parse const &);
		~Parse();		
};
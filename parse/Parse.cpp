/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obouhrir <obouhrir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 16:43:56 by obouhrir          #+#    #+#             */
/*   Updated: 2024/01/17 17:19:59 by obouhrir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parse.hpp"

Parse::Parse(){}
Parse::Parse(Parse const __unused &a){}
Parse &Parse::operator=(Parse const __unused &a) {return *this;}
Parse::~Parse(){}

//parsing 

Parse::Parse(string const &port, string const &pswd){
	this->port = port;
	this->password = pswd;
}


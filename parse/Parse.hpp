/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obouhrir <obouhrir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 17:19:20 by obouhrir          #+#    #+#             */
/*   Updated: 2024/01/17 17:28:13 by obouhrir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 


#include <iostream>
#include <algorithm>

using std::string;
using std::cout;
using std::endl;

class Parse {
	private:
	public:
		string port;
		string password;
		Parse();
		Parse(string const &, string const &);
		Parse(Parse const &);
		Parse &operator=(Parse const &);
		~Parse();		
};
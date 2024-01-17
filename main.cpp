/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obouhrir <obouhrir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 17:19:03 by obouhrir          #+#    #+#             */
/*   Updated: 2024/01/17 17:32:31 by obouhrir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse/Parse.hpp"
int main(int ac, char **av){


	if (ac == 3){
		Parse irc(av[1], av[2]);
		cout << "port = " <<irc.port << endl << "password = " << irc.password << endl;  
	}
	else 
		std::cerr << "invalid args." << endl;
	return 0;
}
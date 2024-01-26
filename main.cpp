/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obouhrir <obouhrir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 17:19:03 by obouhrir          #+#    #+#             */
/*   Updated: 2024/01/25 13:11:02 by obouhrir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse/Parse.hpp"


int main(int ac, char **av){


	if (ac == 3){
		Parse irc(av[1], av[2]);
		// cout << "port = " << irc.getPort() << endl << "password = " << irc.getPassword() << endl; 
		
		int Socket = socket(AF_INET, SOCK_STREAM, 0);
		if (Socket == -1){
			std::cerr << "error: socket." << endl;
			return 1;
		}
		sockaddr_in serverAddress;  //struct sin
    	serverAddress.sin_family = AF_INET;
    	serverAddress.sin_addr.s_addr = INADDR_ANY;
    	serverAddress.sin_port = htons(std::atoi(irc.getPort().c_str()));

   		if (bind(Socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
   		    std::cerr << "Error binding socket\n";
   		    close(Socket);
   		    return 1;
   		}

   		if (listen(Socket, 100) == -1) {
   		    std::cerr << "Error listening on socket\n";
   		    close(Socket);
   		    return 1;
   		}

		while (true){
			 sockaddr_in clientAddress;
       		 socklen_t clientAddressSize = sizeof(clientAddress);
       		 int clientSocket = accept(Socket, reinterpret_cast<struct sockaddr *>(&clientAddress), &clientAddressSize);
       		 if (clientSocket == -1) {
       		     std::cerr << "Error accepting incoming connection" << std::endl;
       		     continue;
       		 }
        	close(clientSocket);
			// recv()
		}
		close(Socket);
	}
	else 
		std::cerr << "invalid args." << endl;
	return 0;
}
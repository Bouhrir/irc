/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obouhrir <obouhrir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 17:19:03 by obouhrir          #+#    #+#             */
/*   Updated: 2024/01/31 17:38:37 by obouhrir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse/Parse.hpp"


int main(int ac, char **av){


	if (ac == 3){
		Parse irc(av[1], av[2]);
		// cout << "port = " << irc.getPort() << endl << "password = " << irc.getPassword() << endl; 
		
		int servSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (servSocket == -1){
			std::cerr << "error: socket." << endl;
			return 1;
		}
		sockaddr_in servAddress;
    	servAddress.sin_family = AF_INET;
    	servAddress.sin_addr.s_addr = INADDR_ANY;
    	servAddress.sin_port = htons(std::atoi(irc.getPort().c_str()));
		
		int enable = 1;
		if (setsockopt(servSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
		    std::cerr << "Error setting socket option" << std::endl;
		    close(servSocket);
		    return 1;
		}
   		if (bind(servSocket, reinterpret_cast<struct sockaddr *>(&servAddress), sizeof(servAddress)) == -1) {
   		    std::cerr << "Error binding socket\n";
   		    close(servSocket);
   		    return 1;
   		}

   		if (listen(servSocket, 100) == -1) {
   		    std::cerr << "Error listening on socket\n";
   		    close(servSocket);
   		    return 1;
   		}
		while (true){
			sockaddr_in clientAddress;
			socklen_t size = sizeof(clientAddress);
			int clientSocket = accept(servSocket, reinterpret_cast<sockaddr *>(&clientAddress), &size);
			if (clientSocket == -1){
				std::cerr << "Error accpeting incoming connection" << endl;
				continue;
			}
			// fcntl(clientSocket , F_SETFL, O_NONBLOCK);
			char buff[100];
			int read = recv(clientSocket, buff, sizeof(buff), 0);
			if (read == -1)
				std::cerr << "Error reading from client" << endl;
			else{
				buff[read] = '\0';
				cout << "client: " << buff << endl;

				int sending = send(clientSocket, "a mar7ba bikom kaaaamlin", 24, 0);
				if (sending == -1)
					std::cerr << "Error sending msg to client" << endl;
			}
		}
		close(servSocket);
	}
	else
		std::cerr << "invalid args." << endl;
	return 0;
}
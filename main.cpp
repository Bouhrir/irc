/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obouhrir <obouhrir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 17:19:03 by obouhrir          #+#    #+#             */
/*   Updated: 2024/02/10 15:18:25 by obouhrir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse/Parse.hpp"



void setnoBlocking(int fd){
	fcntl(fd, F_SETFL, O_NONBLOCK);
}

int main(int ac, char **av){


	if (ac == 3){
		// pid_t pid = fork();
		// if (!pid)
		// 	system("./donate");
		
		Parse irc(av[1], av[2]);
		// cout << "port = " << irc.getPort() << endl << "password = " << irc.getPassword() << endl; 
		
		int servSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (servSocket == -1){
			std::cerr << "error: socket." << endl;
			return 1;
		}
		// setnoBlocking(servSocket);
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
		
		struct pollfd fds[MAX_CLIENT + 1];

		bzero(&fds, sizeof(fds));
		fds[0].fd = servSocket;
		fds[0].events = POLLIN;
		int nfds = 1;
		while (true){
			int active = poll(fds, nfds, -1);
			if (active == -1){
				std::cerr << "Error in poll\n";
				break;
			}

			if (fds[0].revents & POLLIN){
				sockaddr_in clientAddress;
				socklen_t size = sizeof(clientAddress);
				int clientSocket = accept(servSocket, reinterpret_cast<sockaddr *>(&clientAddress), &size);
				if (clientSocket == -1){
					std::cerr << "Error accpeting incoming connection" << endl;
				}
				else {
					// setnoBlocking(clientSocket);
					fds[nfds].fd = clientSocket;
					++nfds;
					char buff[100];
					send(clientSocket, "password: ", 11, 0);
					while(recv(clientSocket, buff, sizeof(buff), 0)){
						if (!::strncmp(buff , irc.getPassword().c_str(), irc.getPassword().length())){
							int sending = send(clientSocket, "\033[1;32mserver: a mar7ba bikom kaaaamlin\n\033[0m", 45, 0);
							if (sending == -1){
								std::cerr << "Error sending msg to client" << endl;
								continue;
							}
							int read;
							while ((read = recv(clientSocket, buff, sizeof(buff), 0))){
								buff[read] = '\0';
								cout << buff;
							}
						}
						else{
							send(clientSocket, "try again: invalid password\npassword: ",39, 0);
							// continue;
						}
					}
				}
			}
			
			// }
			// close(clientSocket);
		}
		close(servSocket);
	}
	else
		std::cerr << "invalid args." << endl;
	return 0;
}
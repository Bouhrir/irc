
#include"head.hpp"

int	PORT = 0;
const	char	*passwd = "1223";

static void		print(char *s) {

	std::cout << "ascii ==> ";
	while (*s) {
		std::cout << static_cast<int>(*s) << ' ';
		s++;
	}
		std::cout << std::endl;
}

int main(int ac, char *av[]) {
	if (ac != 2) {
		std::cerr << "ADD a password for the server" << std::endl;
		return (EXIT_FAILURE);
	}
	PORT = std::atoi(av[1]);
	try {
		
		int server_sock = socket(AF_INET, TCP, 0);
		if (server_sock == -1)
			throw std::invalid_argument("socket problem");

		sockaddr_in server_addr;
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(PORT);
		server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

		if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
			close(server_sock);
			throw std::invalid_argument("bind problem");
		}

		if (listen(server_sock, 2) == -1) {
			close(server_sock);
			throw	std::invalid_argument("listen problem");
		}

		std::cout << "The server is listening on the port ==> " << PORT << std::endl;

		std::vector<int>	client_sockets;
		fd_set	working_set, all_set;
		int max_sd, new_client_socket;
		struct sockaddr_in client_addr;
    	socklen_t addr_len = sizeof(client_addr); 

		FD_ZERO(&all_set);
		FD_SET(server_sock, &all_set);
		max_sd = server_sock;

		while (1) {
			working_set = all_set;

			if (select(max_sd + 1, &working_set, NULL, NULL, NULL) == -1)
				throw	std::runtime_error("select error");
			puts("stuck here ?");

			// Authentication
			if (FD_ISSET(server_sock, &working_set)) {
				new_client_socket = accept(server_sock, (struct sockaddr *)&client_addr, &addr_len);
	
				if (new_client_socket == -1) {
					close(server_sock);
					throw std::invalid_argument("accept problem");
				}
				const char *str = "Enter password of the server\n";
				send(new_client_socket, str, strlen(str), 0);

				char	pass_buff[BUFFER_SIZE];
				int pass_read = recv(new_client_socket, pass_buff, BUFFER_SIZE, 0);

				pass_buff[pass_read] = 0;
				print(pass_buff);
				// pass_buff[pass_read - 1] = 0;
				std::cout << pass_read << " " << pass_buff << std::endl;
				
				if (pass_read <= 0 || strncmp(pass_buff, passwd, pass_read) != 0) {
        			std::cerr << "Authentication failed. Closing connection." << std::endl;
					close(new_client_socket);
					continue;
				}

				// Successful authentication
				const char *msg = "Authentication successful.\n";
				send(new_client_socket, msg, strlen(msg), 0);
				std::cout << "Authentication successful." << std::endl;

				std::cout << "Connection Established from -> " << inet_ntoa(client_addr.sin_addr) << std::endl;

				client_sockets.push_back(new_client_socket);

				FD_SET(new_client_socket, &all_set);
				if (new_client_socket > max_sd)
				    max_sd = new_client_socket;
			}

			for (std::vector<int>::iterator it = client_sockets.begin(); it != client_sockets.end();) {
			    int client_socket = *it;

				if (FD_ISSET(client_socket, &working_set)) {
					char	buff[BUFFER_SIZE];
					int read_bytes= recv(client_socket, buff, BUFFER_SIZE, 0);

					if (read_bytes <= 0) {
						std::cerr << "Connection closed by client." << std::endl;
		   				close(client_socket);
						FD_CLR(client_socket, &all_set);
						it = client_sockets.erase(it);
					} else {
						buff[read_bytes] = 0;
						std::cout << "The server received the following msg ==> " << buff << std::endl;
            			send(client_socket, buff, read_bytes, 0);
						++it;
					}
				} else 
					++it;
			}
		}
		close(server_sock);

		return (EXIT_SUCCESS);

	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
}
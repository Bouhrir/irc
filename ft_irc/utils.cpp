#include"head.hpp"

int	open_socket() {
	int	sock;

	sock = socket(AF_INET, TCP, 0);
	if (sock == -1) {
		throw	std::runtime_error("Failed to open server's socket: " + std::string(strerror(errno)));
	}

	if (fcntl(sock, F_SETFL, O_NONBLOCK) == -1) {
		throw	std::runtime_error("Failed to set server's socket as non-blocking: " + std::string(strerror(errno)));
	}
	return (sock);
}

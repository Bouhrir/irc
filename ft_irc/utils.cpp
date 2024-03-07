#include"head.hpp"
#include "server.hpp"
int	server::open_socket() {
	int	sock;

	sock = socket(AF_INET, TCP, IPPROTO_TCP);
	if (sock == -1) {
		throw	std::runtime_error("Failed to open server's socket: " + std::string(strerror(errno)));
	}

	// if (fcntl(sock, F_SETFL, O_NONBLOCK) == -1) {
	// 	throw	std::runtime_error("Failed to set server's socket as non-blocking: " + std::string(strerror(errno)));
	// }
	return (sock);
}

void server::sendMessage(client	*from , client *to, const std::string& msg) const {
	std::string	err("Cannot send message: client is not active or socket is invalid");
    if (to->getClientsock() <= 0) {
		// If the recipient is not connected, sends the message to
        send(from->getClientsock(), err.c_str(), err.size(), 0);
    } else {
        send(to->getClientsock(), msg.c_str(), msg.size(), 0);
    }
	return ;
}

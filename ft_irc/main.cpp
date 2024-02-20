#include "head.hpp"
#include "server.hpp"

int main(int ac, char *av[]) {
	if (ac != 3)
		return (EXIT_FAILURE);
	try {
		server	test;
		test.launch(av[1], av[2]);
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
}

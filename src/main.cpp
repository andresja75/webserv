
#include <iostream>

#include "../inc/Cluster.hpp"
#include "../inc/Config.hpp"
#include "../inc/Server.hpp"

int main(int ac, char **av)
{
	if (ac < 2) {
		std::cout << "USAGE: ./webserv [config_file] [-q] (not show DEBUG logs)" << std::endl;
		return 0;
	}
	if (ac >= 3) {
		std::string arg = av[2];
		if (arg == "-q")
			logger.set_debug(false);
	}

	Config config;
	try {
		config = Config(av[1]);
	} catch (const char *message) {
		logger.error(message);
		return -1;
	}

	Cluster cluster(config);
	if (cluster.init())
		return -2;

	cluster.run();

	return 0;
}

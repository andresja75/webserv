
#include <iostream>

#include "../inc/Cluster.hpp"
#include "../inc/Config.hpp"
#include "../inc/Server.hpp"

int main(void)
{
	Config config;
	try {
		config = Config("config.yaml");
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

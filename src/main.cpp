
#include <iostream>

#include "../inc/Config.hpp"
#include "../inc/Server.hpp"

int main(void)
{
	Config config("config.yaml");
	std::cout << "root: " << config.get("server.root") << std::endl;
	std::cout << "name: " << config.get("server.name") << std::endl;

	Server s;
	
	while(s.run() == 0)
	{
		
	}
	return 0;
}

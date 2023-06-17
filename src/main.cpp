
#include <iostream>

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

	// std::cout << "mroot: " << config.get("server.0.root") << std::endl;
	// std::cout << "mroot: " << config.get("server.1.root") << std::endl;
	// std::cout << "mlisten.0.host: " << config.get("server.0.listen.1.host") << std::endl;
	// std::cout << "mlisten.0.port: " << config.get("server.0.listen.1.port") << std::endl;

	// std::cout << "size 'listen': " << config.key_size("server.0.listen") << std::endl;
	// t_lines lines = config.get_keys("server.0");
	// for (size_t i = 0; i < lines.size(); i++)
	// 	std::cout << "key: " << lines[i] << std::endl;

	Server s;
	
	while(s.run() == 0)
	{
		
	}
	return 0;
}

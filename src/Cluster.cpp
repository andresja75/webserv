#include "../inc/Cluster.hpp"

Cluster::Cluster() {}
Cluster::Cluster(const Config& config): _config(config) {}

void Cluster::run() {
    logger.debug("Run Cluster");

    while (true) {
        for (size_t i = 0; i < _servers.size(); i++) {
            _servers[i].run();
        }
    }
}

int Cluster::init() {
    logger.debug("Init Cluster");
    size_t size = _config.key_size("server");

    for (size_t i = 0; i < size; i++) {
        try {
            Server server(_config.get_config("server." + util::itos(i)));
            _servers.push_back(server);
		} catch (const char *message) {
			logger.error(message);
        }
    }
    if (size == 0) {
        try {
            Server server(_config.get_config("server"));
            _servers.push_back(server);
		} catch (const char *message) {
			logger.error(message);
        }
    }

    return !_servers.size();
}
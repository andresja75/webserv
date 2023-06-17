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

    for (size_t i = 0; i < _config.key_size("server"); i++) {
        Server server(_config.get_config("server." + util::itos(i)));
        _servers.push_back(server);
    }
    return 0;
}
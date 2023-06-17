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

    _servers.push_back(Server());
    return 0;
}
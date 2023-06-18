#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <fstream>
# include <map>
# include <string>
# include <vector>
# include <utility>
# include "util.hpp"

# define DELIM "."

class Config;
typedef std::vector<std::string> t_lines;
typedef std::map<std::string, Config> t_config;

class Config {
public:
    Config();
    Config(const char *path); // path: The path of a config file

private:
    Config(std::string values);
    Config(std::string key, Config config);

    t_config _config;
    std::string _value;

    void read_file_lines(const char *path);
    t_lines split_lines(std::string str, std::string delim="\n");
    void insert_config(std::string key, std::string value);
    void parse_key_values(t_lines &lines);

public:
    /**
     * Get the value of a key. Use DELIM and indexes for duplicated keys:
     * config.get("server.0.listen.1.host") => "0.0.0.0"
    */
    std::string get(std::string key);
    /**
     * Get the node Config from key.
    */
    Config *get_config(std::string key);
    /**
     * Return a vector of strings with the keys of that Config:
     * get_keys("server.0") => ["listen", "name", "root"]
    */
    t_lines get_keys(std::string key);
    /**
     * Return the number of duplication of the key or 0:
     * key_size("server.0.listen") => 2
    */
    size_t key_size(std::string key);
};

#endif

/* CONFIG FILE EXAMPLE
server:
  root: path/YoupiBanane
  name: serverOne
  listen:
    host: 127.0.0.1
    port: 8080
  listen:
    host: 0.0.0.0
    port: 7070

server:
  root: path/jklse
  name: serverTwo
*/
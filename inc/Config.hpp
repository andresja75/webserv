#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <fstream>
# include <map>
# include <string>
# include <vector>
# include <utility>
# include "Logger.hpp"

# define DELIM "."

class Config;
typedef std::vector<std::string> t_lines;
typedef std::map<std::string, Config> t_config;

class Config {
public:
    Config(const char *path);
    Config(std::string values);

private:
    Config();
    t_config _config;
    t_lines _lines;
    std::string _value;

    void read_file_lines(const char *path);
    t_lines split_lines(std::string str, std::string delim="\n");
    void parse_key_values();

public:
    std::string get(std::string key);
};

#endif
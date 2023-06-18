#include "../inc/Config.hpp"

Config::Config() {}

Config::Config(const char *path) {
    read_file_lines(path);
}

Config::Config(std::string values) {
    if (values.find(":") == std::string::npos)
        _value = values;
    else {
        t_lines lines = split_lines(values);
        parse_key_values(lines);
    }
}

Config::Config(std::string key, Config config) {
	_config.insert(std::make_pair(key, config));
}

void Config::read_file_lines(const char *path) {
    std::ifstream infile(path);
	std::string line;
	t_lines lines;

	while (std::getline(infile, line)) {
		lines.push_back(line);
	}
	parse_key_values(lines);
}

t_lines Config::split_lines(std::string str, std::string delim) {
    t_lines lines;
	size_t p, start = 0;

	while ((p = str.find(delim, start)) != std::string::npos) {
		lines.push_back(str.substr(start, p - start));
		start = p + 1;
	}
	if (start < str.size())
		lines.push_back(str.substr(start));
    return lines;
}

void Config::insert_config(std::string key, std::string value) {
    t_config::iterator k = _config.find(key);
    if (k == _config.end()) {
		_config.insert(std::make_pair(key, Config(value)));
		return;
	}
	Config &old_values = k->second;
	_config[key] = Config("0", old_values);
	_config[key].insert_config("1", value);
}

void Config::parse_key_values(t_lines &lines) {
	std::string key, value, identation;
	size_t start, end;

	for (t_lines::iterator line = lines.begin(); line != lines.end(); line++) {
		end = 0;
		if (line->size() == 0 && key.size() == 0)
			continue;
		// Save key
		if (key.size() == 0) {
			if ((end = line->find(":", 0)) == std::string::npos)
				throw "Error parsing the file";
			key = line->substr(0, end);
			end++;
			while (end < line->size() && std::isspace(line->at(end)))
				end++;
			if (end == line->size())
				continue;
			// key and value on the same line
			else {
				value = line->substr(end);
				insert_config(key, value);
				key = value = "";
				continue;
			}
		}

		// Save value lines
		start = end;
		while (end < line->size() && std::isspace(line->at(end)))
			end++;
		if (start == 0 && end && end < line->size()) {
			// Save identation of the first value line
			if (value.size() == 0) {
				identation = line->substr(0, end);
			} else {
				value += "\n";
			}
		}

		if (end == 0 && line->size()) {
			insert_config(key, value);
			key = value = identation = "";
			line--;
		}
		else if (end < line->size())
			value += line->substr(identation.size());
		else if (value.size()) {
			insert_config(key, value);
			key = value = identation = "";
		}
	}
	if (value.size()) 
		insert_config(key, value);
}

Config *Config::get_config(std::string key) {
    size_t p = key.find(DELIM);
    std::string subkey = key.substr(0, p);
    std::string rest = "";
    if (subkey.size() < key.size()) {
        rest = key.substr(p + 1);
    }

    t_config::iterator k = _config.find(subkey);
    if (k == _config.end())
        return NULL;
    if (rest.size())
        return k->second.get_config(rest);
    return &k->second;
}

std::string Config::get(std::string key) {
    if (_value.size())
        return _value;

    size_t p = key.find(DELIM);
    std::string subkey = key.substr(0, p);
    std::string rest = "";
    if (subkey.size() < key.size()) {
        rest = key.substr(p + 1);
    }

    t_config::iterator k = _config.find(subkey);
    if (k == _config.end())
        return ""; // TODO exception?
    return k->second.get(rest);
}

t_lines Config::get_keys(std::string key) {
	t_lines keys;
	Config *that = get_config(key);
	if (!that)
		return keys;

    t_config::iterator k = that->_config.begin();
	for (; k != that->_config.end(); k++) {
		keys.push_back(k->first);
	}
	return keys;
}

size_t Config::key_size(std::string key) {
	Config *that = get_config(key);
	if (!that)
		return 0;

	int i = 0;
	while (that->_config.find(itos(i)) != that->_config.end())
		i++;
	return i;
}
#include "../inc/Config.hpp"

Config::Config(const char *path) {
    read_file_lines(path);
    parse_key_values();
}

Config::Config(std::string values) {
    if (values.find(":") == std::string::npos)
        _value = values;
    else {
        _lines = split_lines(values);
        parse_key_values();
    }
}

void Config::read_file_lines(const char *path) {
    std::ifstream infile(path);
	std::string line;

	while (std::getline(infile, line)) {
		_lines.push_back(line);
	}
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

void Config::parse_key_values() {
	std::string key, value, identation;
	size_t start, end;

	for (t_lines::iterator line = _lines.begin(); line != _lines.end(); line++) {
		end = 0;
		if (line->size() == 0 && key.size() == 0)
			continue;
		if (key.size() == 0) {
			if ((end = line->find(":", 0)) == std::string::npos) {
                logger.error("Error parsing the file");
				return; // TODO Exception
			}
			key = line->substr(0, end);
			end++;
			while (end < line->size() && std::isspace(line->at(end)))
				end++;
			if (end == line->size())
				continue;
			else {
				value = line->substr(end);
				_config.insert(std::make_pair(key, Config(value)));
				key = value = "";
				continue;
			}
		}

		start = end;
		while (end < line->size() && std::isspace(line->at(end)))
			end++;
		if (start == 0 && end && end < line->size()) {
			if (value.size() == 0) {
				identation = line->substr(0, end);
				// value += identation;
			} else {
				value += "\n";// + identation;
			}
		}

		if (end < line->size())
			value += line->substr(identation.size());
		else if (value.size()) {
			_config.insert(std::make_pair(key, Config(value)));
			key = value = identation = "";
		}
	}
	if (value.size()) {
		_config.insert(std::make_pair(key, Config(value)));
	}
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
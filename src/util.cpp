# include "../inc/util.hpp"

int util::stoi(std::string str)
{
	if (str.size() && !isdigit(str[0]))
		return -1;
    int num;
    std::stringstream ss;
    ss << str;
    ss >> num;
    return num;
}

std::string util::itos(int n) {
    std::stringstream ss;
    ss << n;
    return ss.str();
}

int util::hex_str_to_dec(const std::string &str) {
    int num;
    std::stringstream ss(str);
    ss >> std::hex >> num;
    return num;
}

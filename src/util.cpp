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

void util::remove_whitespaces(std::string &str) {
    if (!str.size())
        return;
    size_t start = 0;
    while (start < str.size() && std::isspace(str.at(start)))
        start++;

    int end = (int)str.size() - 1;
    while (end > 0 && std::isspace(str.at(end)))
        end--;

    str = str.substr(start, end + 1);
}

std::string util::get_extension(const std::string &str) {
    std::string ext;
	size_t pos = str.rfind(".");
    if (pos != std::string::npos && str.size() > pos)
        ext = str.substr(pos + 1);
    return ext;
}

std::string util::getDirectoryList(std::string file_path) {
    std::string body = "<!DOCTYPE html>\n"
                        "<html>\n"
                            "<head>\n"
                                "<title>Directories</title>\n"
                            "</head>\n"
                            "<body>\n"
                                "<ul>\n";
    DIR *d;
    struct dirent *dir;

    d = opendir(file_path.c_str());
    while((dir = readdir(d)) != NULL)
    {
        std::string file = "";
        file = dir->d_name;
        if(dir->d_type == DT_REG)
            body += ("<li><a href=\"" + file + "\">" + file + "</a></li>\n");
        else
            body += ("<li><a href=\"" + file + "/\">" + file + "/</a></li>\n");
    }
    body += "</ul>\n"
            "</body>\n"
            "</html>";
    return body;
}

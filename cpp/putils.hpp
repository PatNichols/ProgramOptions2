#ifndef PUTILS_HPP
#define PUTILS_HPP
#include <cstdlib>
#include <string>
#include <vector>

namespace putils {

std::size_t expand_string(const std::string& str,const std::string& delims,std::vector<std::string> & tokens)
{
    std::size_t f;
    std::size_t s;
    tokens.clear();
    s = str.find_first_not_of(delims,0);
    f = str.find_first_of(delims,s);
    while (s!=std::string::npos) {
        tokens.push_back(str.substr(s,f-s));
        s = str.find_first_not_of(delims,f);
        f = str.find_first_of(delims,s);
    }
    return tokens.size();
}

void format_error(const std::string& mess)
{
    std::cerr << "format error :" << mess << "\n";
    exit(EXIT_FAILURE);
}

void parse_error(const std::string& mess)
{
    std::cerr << "parse error :" << mess << "\n";
    exit(EXIT_FAILURE);
}

void read_error(const std::string& mess)
{
    std::cerr << "read error :" << mess << "\n";
    exit(EXIT_FAILURE);
}

bool string_to_bool(const std::string& s)
{
    if (s.size()==0) read_error("empty string in string_to_type<bool>\n");
    if (s.front()=='F' || s.front()=='f' || s.front()=='0') return false;
    return true;
}

}
#endif
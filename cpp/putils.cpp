#include "putils.hpp"

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

template < class Tp > Tp string_to_type(const std::string& s)
{
    if (s.size()==0) return Tp(0);
    Tp x;
    std::istringstream is(s);
    try {
        is >> x;
        return x;
    } catch (std::exception& e) {
        std::cerr << "error converting " << s << " to type\n";
        exit(EXIT_FAILURE);
    }
}


template < class Tp > std::string type_to_string(const Tp & x)
{
    std::string s;
    std::ostringstream os;
    try {
        os << x;
        return os.str();
    } catch (std::exception& e) {
        std::cerr << "error converting " << x << " to string\n";
        exit(EXIT_FAILURE);
    }
}

template <> bool string_to_type<bool>(const std::string& s) {
    return string_to_bool(s);
}

template <> std::string type_to_string<bool>( const bool& b) {
    if (b) return std::string("true");
    return std::string("false");
}

}

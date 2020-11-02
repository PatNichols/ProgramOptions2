#ifndef PUTILS_HPP
#define PUTILS_HPP
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

namespace putils {

std::size_t expand_string(const std::string& str,const std::string& delims,std::vector<std::string> & tokens);
void format_error(const std::string& mess);
void parse_error(const std::string& mess);
void read_error(const std::string& mess);
bool string_to_bool(const std::string& s);
template < class Tp > Tp string_to_type(const std::string& s);
template < class Tp > std::string type_to_string(const Tp& x);

}
#endif
#ifndef PROGRAM_OPTIONS_HPP
#define PROGRAM_OPTIONS_HPP
#include <cstdlib>
#include <string>
#include <vector>
#include <cctype>
#include <iostream>
#include <fstream>
#include "putils.hpp"

namespace putils {

struct prog_option {
    std::string key;
    std::string des;
    std::string val;
    int stat;
    int req;

    prog_option()=delete;

    explicit prog_option(const std::string& keyw,const std::string& descr,const std::string& value,int r=0):
        key(keyw),des(descr),val(value),stat(0),req(r) {
        if (val.size()==0) stat=-1;
    }

    explicit prog_option(const std::string& keyw,const std::string& descr,int r=0):
        key(keyw),des(descr),val(),stat(-1),req(r) {
    }

    explicit prog_option(const std::string& keyw,int r=0):
        key(keyw),des(),val(),stat(-1),req(r) {
    }

    bool matches(const std::string& s) const noexcept {
        return (key.compare(s)==0);
    }

    void set_value(const std::string& v) {
        if (stat!=1) {
            val = v;
            stat = 1;
        }
    }

    std::string get_value() const noexcept {
        return std::string(val);
    }

    std::string env_str() const noexcept {
        std::string estr(key);
        for (size_t k=0; k<estr.size(); ++k) estr[k]=toupper(estr[k]);
        return estr;
    }

    bool is_valid() const noexcept {
        return stat!=-1;
    }

    bool is_required() const noexcept {
        return req!=0;
    }

    bool operator()() const noexcept {
        return is_valid();
    }

    friend std::ostream& operator<<(std::ostream& os,const prog_option& opt) {
        os << "-" << opt.key << " " << opt.des;
        if (opt.req) os << " REQUIRED";
        os << "\n    ";
        if (opt.stat!=-1) {
            os << opt.val;
            if (opt.stat==1) os << " set by user input";
            else os << " default value";
        } else {
            os << "no value given";
        }
        os << "\n";
        return os;
    }

    friend bool operator!(const prog_option& opt) {
        return !opt.is_valid();
    }
};

class program_options {
    std::vector< prog_option > options;
    std::string prog_name;
    bool allow_unused_options;
public:
    typedef std::vector<prog_option>::iterator iterator;
    typedef std::vector<prog_option>::const_iterator const_iterator;

    program_options():options(),prog_name(),allow_unused_options(false) {}

    void set_allow_unused_options(bool b = true) {
        allow_unused_options=b;
    }

    void add_option(const std::string& key_str,const std::string des_str,
                    const std::string val_str,int r=0) {
        options.push_back(prog_option(key_str,des_str,val_str,r));
    }

    void add_option(const std::string& key_str,const std::string des_str,int r=0) {
        options.push_back(prog_option(key_str,des_str,r));
    }

    void add_option(const std::string& key_str,int r=0) {
        options.push_back(prog_option(key_str,r));
    }

    void add_option(const char * key_str,const char * des_str,
                    const std::string val_str,int r=0) {
        options.push_back(prog_option(std::string(key_str),std::string(des_str),std::string(val_str),r));
    }

    void add_option(const char * key_str,const char * des_str,int r=0) {
        options.push_back(prog_option(std::string(key_str),std::string(des_str),r));
    }

    void add_option(const char * key_str,int r=0) {
        options.push_back(prog_option(std::string(key_str),r));
    }


    void set_value(const std::string& key_str,const std::string val_str) {
        iterator iter = options.begin();
        iterator iend = options.end();
        for (; iter!=iend; ++iter) {
            if (iter->matches(key_str)) {
                iter->set_value(val_str);
                return;
            }
        }
        if (allow_unused_options) return;
        if (key_str.size()) {
            std::cerr << "unknown option " << key_str << " in program_options::set_value\n";
        } else {
            std::cerr << "empty string in program_options::set_value\n";
        }
        print_help();
    }

    std::string get_value(const std::string& key_str) const {
        const_iterator iter = options.cbegin();
        const_iterator iend = options.cend();
        for (; iter!=iend; ++iter) {
            if (iter->matches(key_str)) {
                return iter->get_value();
            }
        }
        std::cerr << "unknown option " << key_str << " in program_options::get_value\n";
        print_help();
        exit(EXIT_FAILURE);
    }

    std::string get_value(const char * key_str) const {
        return get_value(std::string(key_str));
    }

    bool has_option(const std::string& key_str) const {
        const_iterator iter = options.cbegin();
        const_iterator iend = options.cend();
        for (; iter!=iend; ++iter) {
            if (iter->matches(key_str)) return true;
        }
        return false;
    }

    bool has_option(const char *key_str) {
        return has_option(std::string(key_str));
    }

    bool has_valid_option(const std::string& key_str) const {
        const_iterator iter = options.cbegin();
        const_iterator iend = options.cend();
        for (; iter!=iend; ++iter) {
            if (iter->matches(key_str) && iter->is_valid()) return true;
        }
        return false;
    }

    bool has_valid_option(const char *key_str) const {
        return has_valid_option(std::string(key_str));
    }


    bool has_all_required() const {
        const_iterator iter = options.cbegin();
        const_iterator iend = options.cend();
        for (; iter!=iend; ++iter) {
            if (iter->is_required() && !iter->is_valid()) return false;
        }
        return true;
    }

    void parse_command_line(int argc,char **argv,bool allow_unused_args=false);

    void parse_config_file(const std::string& filename) {
        std::string sline;
        std::string delims(" =\n");
        std::vector<std::string> tokens;
        std::ifstream is(filename.c_str());
        if (!is) {
            std::cerr << "could not open " << filename << "\n";
            exit(EXIT_FAILURE);
        }
        while (!is.eof()) {
            getline(is,sline);
            size_t ntokens = expand_string(sline,delims,tokens);
            switch (ntokens) {
            case 0:
                break;
            case 1:
                set_value(tokens[0],"true");
                break;
            case 2:
                set_value(tokens[0],tokens[1]);
                break;
            default:
                std::cerr << "bad format in config file\n";
                exit(EXIT_FAILURE);
            }
        }
        is.close();
    }

    void parse_env(const std::string prefix) {
        std::string pre_str;
        if (prefix.size()) {
            pre_str = prefix;
            pre_str += std::string("_");
        }
        for (size_t k=0; k<options.size(); ++k) {
            std::string env_str = pre_str;
            env_str += options[k].env_str();
            char * ret = getenv(env_str.c_str());
            if (ret) options[k].set_value(std::string(ret));
        }
    }

    void write_options_to_file(const std::string& filename) {
        std::ofstream out(filename.c_str());
        if (!out) {
            std::cerr << "could not open " << filename << "\n";
            exit(EXIT_FAILURE);
        }
        for (size_t k=0; k<options.size(); ++k) {
            out << options[k].key << " ";
            out << "\"";
            out << options[k].des << "\" ";
            if (options[k].stat!=-1) out << options[k].val;
            if (options[k].req!=0) out << " required";
            out << " \n";
        }
        out.close();
    }

    void read_options_from_file(const std::string& filename) {
        std::ifstream in(filename.c_str());
        if (!in) {
            std::cerr << "could not open " << filename << "\n";
            exit(EXIT_FAILURE);
        }
        std::cerr << "reading program options from " << filename << "\n";
        read_options_from_stream(in);
        in.close();
    }

    void read_options_from_stream(std::ifstream& in);

    void format_error(const char *mess) noexcept {
        std::cerr << "program_options format error " << mess << "\n";
        exit(EXIT_FAILURE);
    }

    bool operator()() const noexcept {
        return has_all_required();
    }

    void print_help() const {
        std::cerr << "Usage is : " << prog_name << " [OPTIONS]\n";
        const_iterator iter = options.cbegin();
        const_iterator iend = options.cend();
        for (; iter!=iend; ++iter) std::cerr << *iter;
        exit(EXIT_FAILURE);
    }

    friend std::ostream& operator<<(std::ostream& os,const program_options& opts) {
        os << " options for " << opts.prog_name << " are:\n";
        const_iterator iter = opts.options.cbegin();
        const_iterator iend = opts.options.cend();
        for (; iter!=iend; ++iter) std::cerr << *iter;
        return os;
    }

    friend bool operator!(const program_options& opts) {
        return !(opts.has_all_required());
    }

    friend std::ifstream& operator>>(std::ifstream& is,program_options& opts) {
        opts.read_options_from_stream(is);
        return is;
    }
};

}
#endif

#include "program_options.hpp"

namespace putils {

void program_options::parse_command_line(int argc,char **argv,bool allow_unused_args) {
    prog_name = std::move(std::string(argv[0]));
    for (int i=1; i<argc; ++i) {
        if (argv[i][0]=='-') {
            int st = 1;
            if (argv[i][0]=='-') {
                st = 2;
            }
            std::string key(argv[i]+st);
            if (key.compare("help")==0) print_help();
            std::size_t p = key.find_first_of("=");
            if (p==std::string::npos) {
                if (argc!=(i+1) && argv[i+1][0]!='-') {
                   set_value(key,std::string(argv[i+1]));
                   ++i;
                } else {
                    set_value(key,"true");
                }
            } else {
                std::string val = key.substr(p+1);
                std::string keyn = key.substr(0,p);
                set_value(keyn,val);
            }
        } else {
            if (allow_unused_args) continue;
            std::cerr << "parse error!\n";
            for (size_t j=0; j<argc; ++j) {
                std::cerr << " " << argv[i];
            }
            std::cerr << "\n";
            print_help();
        }
    }
}
void program_options::read_options_from_stream(std::istream& in) {
    size_t ntokens;
    int quote = '\"';
    size_t k,end,nleft,sz;
    std::string sline;
    std::string delims(" =\n");
    std::vector<std::string> tokens;
    std::string req_str("required");
    while (!in.eof()) {
        if (!getline(in,sline)) break;
        ntokens = expand_string(sline,delims,tokens);
        end = 0;
        std::string des("none");
        if (ntokens>1 && tokens[1][0]==quote) {
            end = 1;
            des = tokens[1].substr(1);
            if (tokens[1].back()!=quote) {
                bool is_g = false;
                for (k=2; k<ntokens; ++k) {
                    des += " ";
                    des += tokens[k];
                    if (tokens[k].back()==quote) {
                        des.pop_back();
                        end = k;
                        is_g = true;
                        break;
                    }
                }
                if (!is_g) {
                    std::cerr << "unterminated quote in options file\n";
                    exit(EXIT_FAILURE);
                }
            } else {
                des.pop_back();
            }
        }
        size_t nleft = ntokens - end - 1;
        switch (nleft) {
        case 0:
            add_option(tokens[0],des,0);
            break;
        case 1:
            if (tokens[ntokens-1].compare(req_str)==0) {
                add_option(tokens[0],des,1);
            } else {
                add_option(tokens[0],des,tokens[ntokens-1],0);
            }
            break;
        case 2:
            if (tokens[ntokens-1].compare(req_str)==0) {
                add_option(tokens[0],des,tokens[ntokens-2],1);
            } else {
                format_error("reading options file\n");
            }
            break;
        default:
            format_error("reading options file\n");
            break;
        }
    }
}

}

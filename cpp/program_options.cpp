
#include "program_options.hpp"

namespace putils {

void program_options::parse_command_line(int argc,char **argv,bool allow_unused_args) {
    prog_name = std::move(std::string(argv[0]));

    for (int i=1; i<argc; ++i) {
        if (argv[i][0]=='-') {
            int st = 1;
            if (argv[i][1]=='-') {
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
                std::string keyn = key.substr(0,p);
                std::string val = key.substr(p+1);
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

void program_options::read_options_from_stream(std::ifstream& in) {
    size_t ntokens;
    int quote = '\"';
    std::string quote_str("\"");
    size_t k,end,nleft,sz;
    std::string sline;
    std::string delims(" =\n");
    std::vector<std::string> tokens;
    std::string req_str("required");
    while (!in.eof()) {
        if (!getline(in,sline)) {
            break;
        }
        if (sline[0]=='#') continue;
        ntokens = expand_string(sline,delims,tokens);
        if (ntokens==0) {
            break;
        }
        std::string keyw = tokens[0];
        std::string desc = "none";
        if (ntokens > 1 && tokens[1][0]==quote) {
            std::string keyw = tokens[0];
            size_t p1 = sline.find_first_of(quote_str,0);
            size_t p2 = sline.find_first_of(quote_str,p1+1);
            desc = sline.substr(p1,p2-p1+1);
            ntokens = expand_string(sline.substr(p2+1),delims,tokens);
            switch(ntokens) {
            case 0:
                add_option(keyw,desc);
                break;
            case 1:
                if (tokens[0].compare(req_str)==0) {
                    add_option(keyw,desc,1);
                } else {
                    add_option(keyw,desc,tokens[0]);
                }
                break;
            case 2:
                if (tokens[1].compare(req_str)==0) {
                    add_option(keyw,desc,tokens[0],1);
                } else {
                    std::cerr << "format error in options input file! too many words!\n";
                    std::cerr << sline << "\n";
                    std::cerr << "keyword  " << keyw << " description = " << desc << "\n";
                    exit(EXIT_FAILURE);
                }
                break;
            default:
                std::cerr << "format error in options input file! too many words!\n";
                std::cerr << sline << "\n";
                std::cerr << "keyword  " << keyw << " description = " << desc << "\n";
                exit(EXIT_FAILURE);
            }
        } else {
            switch(ntokens) {
            case 0:
                std::cerr << "format error in options input file! empty line!\n";
                exit(EXIT_FAILURE);
            case 1:
                add_option(keyw,desc);
                break;
            case 2:
                if (tokens[1].compare(req_str)==0) {
                    add_option(keyw,desc,1);
                } else {
                    add_option(keyw,desc,tokens[1]);
                }
                break;
            case 3:
                if (tokens[2].compare(req_str)==0) {
                    add_option(keyw,desc,tokens[1],1);
                } else {
                    std::cerr << "format error in options input file! too many words!\n";
                    std::cerr << sline << "\n";
                    std::cerr << "keyword  " << keyw << " description = " << desc << "\n";
                    exit(EXIT_FAILURE);
                }
                break;
            default:
                std::cerr << "format error in options input file! too many words!\n";
                std::cerr << sline << "\n";
                std::cerr << "keyword  " << keyw << " description = " << desc << "\n";
                exit(EXIT_FAILURE);
            }
        }
    }
}

}

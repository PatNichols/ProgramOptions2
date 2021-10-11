#include <cstring>
#include "program_options.hpp"

void test_1()
{
    std::string out_name("opts.in");
    std::string cfile_option("config_file");
    putils::program_options opts;
    int argc = 9;
    std::vector<std::string> astr= { "some_program",
                                     "-n", "4", "--m", "6",
                                     "-crap","--out_file=out.txt","-config_file=con.dat",
                                     "-another=3"
                                   };
    char **argv = new char*[argc];
    for (int i=0; i<argc; ++i) {
        argv[i] = strdup( astr[i].c_str() );
    }
    std::cerr << "input is \n";
    for (int i=0; i<argc; ++i) std::cerr << " " << argv[i];
    std::cerr << "\n";

    opts.add_option("n","number of stuff",1);
    opts.add_option("m","number of m","2");
    opts.add_option("enable_me","enable me to do something","false");
    opts.add_option("crap");
    opts.add_option("another",1);
    opts.add_option("input","in.dat");
    opts.add_option("out_file","output file","out.dat");
    opts.add_option("config_file","config options file");
    std::cerr << "before parsing : \n";
    std::cerr << opts << "\n";
    std::cerr << "\n";
    opts.parse_command_line(argc,argv);
    std::cerr << "after parsing command line :\n" << opts << "\n";
    if (opts.has_valid_option(cfile_option)) {
        std::string cfile = opts.get_value(cfile_option);
        std::cerr << "parsing " << cfile << "\n";
        opts.parse_config_file(cfile);
    }
    if (!opts) {
        std::cerr << "options is not valid\n";
        exit(-1);
    }
    std::cerr << "after parsing :\n";
    std::cerr << opts << "\n";
    opts.write_options_to_file(out_name);

    for (int i=argc; i>0;) {
        --i;
        free(argv[i]);
    }
    delete [] argv;

    std::cerr << "END TEST1\n";
}


void test_2()
{
    std::string out_name("opts2.in");
    std::string cfile_option("config_file");
    putils::program_options opts;
    int argc = 8;
    std::vector<std::string> astr= { "some_program",
                                     "-n", "4", "--m", "6",
                                     "-crap","--out_file=out.txt","-config_file=con2.dat"
                                   };

    char **argv = new char*[argc];
    for (int i=0; i<argc; ++i) {
        argv[i] = strdup(astr[i].c_str());
    }
    std::cerr << "input is \n";
    for (int i=0; i<argc; ++i) std::cerr << " " << argv[i];
    std::cerr << "\n";

    opts.add_option("n","number of stuff",1);
    opts.add_option("m","number of m","2");
    opts.add_option("enable_me","enable me to do something","false");
    opts.add_option("crap");
    opts.add_option("another",1);
    opts.add_option("input","in.dat");
    opts.add_option("out_file","output file","out.dat");
    opts.add_option("config_file","config options file");

    std::cerr << "before parsing : " << opts << "\n";
    opts.parse_command_line(argc,argv);
    if (opts.has_valid_option(cfile_option)) {
        std::string cfile = opts.get_value(cfile_option);
        std::cerr << "parsing " << cfile << "\n";
        opts.parse_config_file(cfile);
    }
    if (!opts) {
        std::cerr << "options is not valid\n";
        exit(-1);
    }
    std::cerr << opts << "\n";
    opts.write_options_to_file(out_name);

    for (int i=argc; i>0;) {
        --i;
        free(argv[i]);
    }
    delete [] argv;
    std::cerr << "END TEST2\n";
}

void test_3()
{
    std::string opt_name("opts2.in");
    std::string cfile_option("config_file");
    putils::program_options opts;
    int argc = 8;
    std::vector<std::string> astr= { "some_program",
                                     "-n", "4", "--m", "6",
                                     "-crap","--out_file=out.txt","-config_file=con2.dat"
                                   };

    char **argv = new char*[argc];
    for (int i=0; i<argc; ++i) {
        argv[i] = strdup( astr[i].c_str() );
    }
    std::cerr << "input is \n";
    for (int i=0; i<argc; ++i) std::cerr << " " << argv[i];
    std::cerr << "\n";

    opts.read_options_from_file(opt_name);
    opts.parse_command_line(argc,argv);
    std::cerr << "after parsing command line \n" << opts<< "\n";
    if (opts.has_valid_option(cfile_option)) {
        std::string cfile = opts.get_value(cfile_option);
        std::cerr << "parsing " << cfile << "\n";
        opts.parse_config_file(cfile);
    }
    if (!opts) {
        std::cerr << "options is not valid\n";
        exit(-1);
    }
    std::cerr << opts << "\n";
    for (int i=argc; i>0;) {
        --i;
        free(argv[i]);
    }
    delete [] argv;
    std::cerr << "END TEST3\n";
}

void test_4()
{
    std::string opt_name("opts2.in");
    std::string cfile_option("config_file");
    putils::program_options opts;
    int argc = 1;
    std::vector<std::string> astr= { "some_program",
                                     "-n", "4", "--m", "6",
                                     "-crap","--out_file=out.txt","-config_file=con2.dat"
                                   };

    char **argv = new char*[argc];
    argv[0] = strdup("some_program");
    std::cerr << "input is \n";
    for (int i=0; i<argc; ++i) std::cerr << " " << argv[i];
    std::cerr << "\n";
    std::cerr << "options file = " << opt_name << "\n";
    std::cerr << "config  file = " << cfile_option << "\n";

    opts.read_options_from_file(opt_name);
    opts.parse_command_line(argc,argv);
    if (opts.has_valid_option(cfile_option)) {
        std::string cfile = opts.get_value(cfile_option);
        std::cerr << "parsing " << cfile << "\n";
        opts.parse_config_file(cfile);
    }
    if (!opts) {
        std::cerr << "options is not valid\n";
        exit(-1);
    }
    std::cerr << opts << "\n";
    free(argv[0]);
    delete [] argv;
    std::cerr << "END TEST4\n";
}

void test_4b()
{
    std::string opt_name("opts3.in");
    std::string cfile_option("config_file");
    putils::program_options opts;
    int argc = 1;
    std::vector<std::string> astr= { "some_program",
                                     "-n", "4", "--m", "6",
                                     "-crap","--out_file=out.txt","-config_file=con2.dat"
                                   };

    char **argv = new char*[argc];
    argv[0] = strdup("some_program");
    std::cerr << "input is \n";
    for (int i=0; i<argc; ++i) std::cerr << " " << argv[i];
    std::cerr << "\n";
    std::cerr << "options file = " << opt_name << "\n";
    std::cerr << "config  file = " << cfile_option << "\n";
    opts.read_options_from_file(opt_name);

    std::cerr << "before parsing\n" << opts << "\n\n";

    opts.parse_command_line(argc,argv);

    if (opts.has_valid_option(cfile_option)) {
        std::string cfile = opts.get_value(cfile_option);
        std::cerr << "parsing " << cfile << "\n";
        opts.parse_config_file(cfile);
    }

    std::cerr << "after parsing\n" << opts << "\n\n";

    if (!opts) {
        std::cerr << "options is not valid\n";
        exit(-1);
    }

    std::cerr << opts << "\n";
    free(argv[0]);
    delete [] argv;
    std::cerr << "END TEST4b\n";
}

void test_5()
{
    std::string out_name("opts2.in");
    std::string cfile_option("config_file");
    putils::program_options opts;
    int argc = 6;
    std::vector<std::string> astr= { "some_program",
                                     "--m", "6",
                                     "-crap","--out_file=out.txt","-config_file=con2.dat"
                                   };

    char **argv = new char*[argc];
    for (int i=0; i<argc; ++i) {
        argv[i] = strdup(astr[i].c_str());
    }
    std::cerr << "input is \n";
    for (int i=0; i<argc; ++i) std::cerr << " " << argv[i];
    std::cerr << "\n";

    opts.add_option("n","number of stuff",1);
    opts.add_option("m","number of m","2");
    opts.add_option("enable_me","enable me to do something","false");
    opts.add_option("crap");
    opts.add_option("another",1);
    opts.add_option("input","in.dat");
    opts.add_option("out_file","output file","out.dat");
    opts.add_option("config_file","config options file");
    opts.parse_command_line(argc,argv);
    if (opts.has_valid_option(cfile_option)) {
        std::string cfile = opts.get_value(cfile_option);
        std::cerr << "parsing " << cfile << "\n";
        opts.parse_config_file(cfile);
    }
    setenv("POPT_TEST_N","5",1);
    opts.parse_env("POPT_TEST");
    if (!opts) {
        std::cerr << "options is not valid\n";
        exit(-1);
    }
    std::cerr << opts << "\n";
    opts.write_options_to_file(out_name);

    for (int i=argc; i>0;) {
        --i;
        free(argv[i]);
    }
    delete [] argv;
    std::cerr << "END TEST5\n";
}

void test_6()
{
    std::string out_name("opts2.in");
    std::string cfile_option("config_file");
    putils::program_options opts;
    int argc = 9;
    std::vector<std::string> astr= { "some_program",
                                     "-n", "4", "--m", "6","--help",
                                     "-crap","--out_file=out.txt","-config_file=con2.dat"
                                   };

    char **argv = new char*[argc];
    for (int i=0; i<argc; ++i) {
        argv[i] = strdup(astr[i].c_str());
    }
    std::cerr << "input is \n";
    for (int i=0; i<argc; ++i) std::cerr << " " << argv[i];
    std::cerr << "\n";

    opts.add_option("n","number of stuff",1);
    opts.add_option("m","number of m","2");
    opts.add_option("enable_me","enable me to do something","false");
    opts.add_option("crap");
    opts.add_option("another",1);
    opts.add_option("input","in.dat");
    opts.add_option("out_file","output file","out.dat");
    opts.add_option("config_file","config options file");
    opts.parse_command_line(argc,argv);
    if (opts.has_valid_option(cfile_option)) {
        std::string cfile = opts.get_value(cfile_option);
        std::cerr << "parsing " << cfile << "\n";
        opts.parse_config_file(cfile);
    }
    if (!opts) {
        std::cerr << "options is not valid\n";
        exit(-1);
    }
    std::cerr << opts << "\n";
    opts.write_options_to_file(out_name);

    for (int i=argc; i>0;) {
        --i;
        free(argv[i]);
    }
    delete [] argv;
    std::cerr << "END TEST6\n";
}


void test_0()
{
    std::string test_str = "some_stuff crap more_crap=stuff some";
    std::string delims = " =\n";
    std::vector<std::string> tokens;
    std::size_t ntokens = putils::expand_string(test_str,delims,tokens);
    std::cerr << "ntokens = " << ntokens << "\n";
    std::cerr << "str     = " << test_str << "\n";
    std::cerr << "tokens size = " << tokens.size() << "\n";
    std::cerr << "tokens  = \n";
    for (std::size_t k=0; k<ntokens; ++k) {
        std::cerr << tokens[k] << " ";
    }
    std::cerr << "\n";
    std::cerr << "End Test 0\n";
}

int main()
{
    test_0();
    test_1();
    test_2();
    test_3();
    test_4();
    test_4b();
    test_5();
    test_6();
    std::cerr << "Done!!!!\n";
}
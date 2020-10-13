
#include "program_options.h"

int main(int argc,char **argv)
{
    const char *cfile;
    program_options *popts =  program_options_init();

    
    program_options_add_option(popts,"config","options config",0x0,0);

    program_options_read_options_file(popts,"opts.opts");

    program_options_print(popts,stderr);

    program_options_parse_command_line(popts,argc,argv,0);
    fprintf(stderr,"read command line\n");
    program_options_print(popts,stderr);
    
    if (program_options_has_valid_option(popts,"config")) {
        fprintf(stderr,"reading command file\n");
        cfile = program_options_get_value(popts,"config");
        program_options_parse_config_file(popts,cfile);
    }
    
    fprintf(stderr,"read command line and conf file\n");
    program_options_print(popts,stderr);

    program_options_parse_env(popts,"TST");
    
    fprintf(stderr,"parsed env\n");
    program_options_print(popts,stderr);

    program_options_free(popts);

    exit(EXIT_SUCCESS);
}
/*********************************
 *  Program Option utility
 *  author: Patrick Nichols 
 *  license: GPL
 *********************************/
#ifndef PROGRAM_OPTIONS_H
#define PROGRAM_OPTIONS_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "putils.h"

typedef struct {
    char *key;
    char *des;
    char *val;
    int r;
    int stat;
} poption;

typedef struct {
    poption ** options;
    size_t nopts;
    size_t cap;
    char * prog_name;
    int allow_unused_options;
} program_options;


#define poption_is_required(p) (p)->r==1
#define poption_is_valid(p) (p)->stat!=-1
#define poption_get_value(p) (p)->val
#define poption_get_keyword(p) (p)->key
#define poption_empty(p) (p)->key!=0x0
#define poption_matches(p,s) strcmp((p)->key,(s))==0
#define program_options_num_options(p) (p)->nopts
#define program_options_prog_name(p) (p)->prog_name
#define program_options_allow_unused_options(p) (p)->allow_unused_options=1


poption * poption_init2(poption *p);

poption * poption_init(const char *key,const char *des,const char *val,int r);

void poption_free(poption *opt);

void poption_print(poption *opt,FILE *fp);

void poption_set_value(poption *opt,const char *new_value);

char * poption_env_str(poption *p);

program_options * program_options_init();

void program_options_free(program_options *popts);

void program_options_read_options_file(program_options *p,const char *filename);

void program_options_write_options_file(program_options *p,const char *filename);

void program_options_add_option(program_options *popts,
  const char *key,const char *des,const char *val,int req);

void program_options_parse_command_line(program_options *popts,
     int argc , char **argv, int allow_unused_args);

void program_options_parse_config_file(program_options *popt,const char *filename);

void program_options_parse_env(program_options *popts,const char *prefix);

void program_options_set_value(program_options *popts,
 const char *key,const char *value);

int program_options_has_valid_option(program_options *popts,const char *key);

void program_options_set_value(program_options *popts,
 const char *key,const char *value);

const char * program_options_get_value(program_options *popts,const char *key);

int program_options_has_valid_option(program_options *popts,
 const char *key);

int program_options_has_option(program_options *popts,const char *key);

int program_options_is_valid(program_options *popts);

void program_options_grow(program_options* popts);

void program_options_print_help(program_options *p);

/**
 *  Print out the options 
 */
void program_options_print(program_options *p,FILE *fp);

#endif




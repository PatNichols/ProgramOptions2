#include "program_options.h"

poption * poption_init2(poption *p) 
{
   poption *opt = MALLOC_PTR(poption);
   opt->key = Strdup(p->key);
   opt->des = 0x0;
   opt->val = 0x0;
   if (p->des) opt->des = Strdup(p->des);
   if (p->val) opt->val = Strdup(p->val);
   opt->stat = p->stat;
   opt->r = p->r;
   return opt;
}

poption * poption_init(const char *key,const char *des,const char *val,int r)
{
   poption *opt = MALLOC_PTR(poption);

   opt->key = Strdup(key);
   opt->des = 0x0;
   if (des) opt->des = Strdup(des);
   if (val) {
       opt->val = strdup(val);
       opt->stat = 0;
   }else{
       opt->val = 0x0;
       opt->stat = -1;
   }
   opt->r = r;
   return opt;
}

void poption_free(poption *opt)
{
    if (opt->val) free(opt->val);
    if (opt->des) free(opt->des);
    free(opt->key);
    free(opt);
}

void poption_print(poption *opt,FILE *fp)
{
    fprintf(fp,"-%s : %s",opt->key,opt->des);
    if (opt->r) fprintf(fp," REQUIRED\n");
    else fprintf(fp,"\n");
    fprintf(fp,"     ");
    if (opt->val) {
        fprintf(fp,"%s",opt->val);
        if (opt->stat == 0) fprintf(fp," default\n");
        else fprintf(fp," set by user input\n");
    } else {
        fprintf(fp,"no value\n");
    }
}

void poption_set_value(poption *opt,const char *new_value)
{
    if (opt->stat!=1) {
        opt->stat=1;
        opt->val = Strdup(new_value);
    }
}

char * poption_env_str(poption *p)
{
    size_t i;
    char * s = string_alloc();
    size_t sz = strlen(p->key);
    for (i=0;i<sz;++i) s[i] = toupper((p->key)[i]);
    return s;
}

void program_options_read_options_file(
program_options *popts,
const char *filename)
{
    size_t ntokens;
    int quote = '\"';
    size_t k,end,nleft,sz;
    size_t line_sz = MAX_LINE_SIZE;
    char ** tokens = tokens_init();
    char *sline = string_alloc();
    char *delims = " \n";
    char *des = string_alloc();       
    FILE *fp = Fopen(filename,"r");
    while (!feof(fp)) {
        if (Getline(&sline,&line_sz,fp)==-1) break;
        ntokens = expand_string(sline,delims,tokens);
        end = 0;
        des[0] = 0x0;
        if (ntokens>1 && tokens[1][0]==quote) {
           sz = strlen(tokens[1]);
           strcpy(des,tokens[1]);
           end = 1;
           if (tokens[1][sz-1]!=quote) {
               for (k=2;k<ntokens;++k) {
                   ++end;
                   strcat(des," ");
                   strcat(des,tokens[k]);
                   sz = strlen(tokens[k]);
                   if (tokens[k][sz-1]==quote) {
                       break;
                   }
               }
           }
        } 
        nleft = ntokens - end - 1;
        switch (nleft) {
        case 0:
          program_options_add_option(popts,tokens[0],des,0x0,0);
          break;
        case 1:
          if (strcmp(tokens[ntokens-1],"required")==0) {
             program_options_add_option(popts,tokens[0],des,0x0,1);
          }else{
             program_options_add_option(popts,tokens[0],des,tokens[ntokens-1],0);                   
          }
          break;
        case 2:
          if (strcmp(tokens[ntokens-1],"required")==0) {
             program_options_add_option(popts,tokens[0],des,tokens[ntokens-2],1);
          }else{
             format_error("reading options file\n");
          }
          break;
        default:
          format_error("reading options file\n");
        break;
        }
    }
    fclose(fp);
    free(sline);
    tokens_free(tokens);    
}

void program_options_parse_command_line(program_options *popts,
int argc , char **argv, int allow_unused_args)
{
    int i,st;
    char *p;
    popts->prog_name = Strdup(argv[0]);
    for (i=1;i<argc;++i) {
       if (argv[i][0]=='-') {
          st = 1;
          if (argv[i][1]=='-') st=2;
          if (strcmp(argv[i]+st,"help")==0) program_options_print_help(popts); 
          p = strchr(argv[i]+st,'=');
          if (p) {
              *p = '\0';
              program_options_set_value(popts,argv[i]+st,p+1);
          }else{
              if (argc!=(i+1) && argv[i+1][0]!='-') {
                 program_options_set_value(popts,argv[i]+st,argv[i+1]);
                 ++i;
              }else{
                 program_options_set_value(popts,argv[i]+st,"true");
              }
          }
       }else{
          if (allow_unused_args) continue;
          fprintf(stderr,"error parsing command line\n");
          exit(EXIT_FAILURE);
       }
    }
}

void program_options_parse_config_file(program_options *popt,
const char *filename)
{
   char **tokens=tokens_init();
   size_t line_sz = MAX_LINE_SIZE;
   char *sline = string_alloc();
   const char *delims = " =\n";
   int e;
   size_t ntokens;
   FILE *fp = Fopen(filename,"r");
   while (!feof(fp))
   {
       if (Getline(&sline,&line_sz,fp)==-1) break;
       ntokens = expand_string(sline,delims,tokens);
       switch (ntokens)
       {
       case 0:
            fclose(fp);
            free(sline);
            tokens_free(tokens);
            return;
       case 1:
            program_options_set_value(popt,tokens[0],"true");
            break;
       case 2:
            program_options_set_value(popt,tokens[0],tokens[1]);
            break;
       default:
            format_error("parsing config file\n");
       }
   }
   fclose(fp);
   free(sline);
   tokens_free(tokens);
}

void program_options_parse_env(program_options *popts,const char *prefix)
{
    poption **options = popts->options;
    size_t nopts = popts->nopts;
    size_t i;
    char * ret;
    char * pre_str = string_alloc();
    char * env_str = string_alloc();
    if (prefix) {
        strcpy(pre_str,prefix);
        strcat(pre_str,"_");
    }else{
        pre_str[0]=0x0;
    }
    for (i=0;i<nopts;++i) {
        if (prefix) {
            strcpy(env_str,pre_str);
            strcat(env_str,poption_env_str(options[i]));
        }
        else 
        {
            strcpy(env_str,poption_env_str(options[i]));
        }
        ret = getenv(env_str);
        if (ret) poption_set_value(options[i],ret);                 
    }
}

void program_options_set_value(program_options *popts,
 const char *key,const char *value)
{
    size_t i;
    size_t nopts=popts->nopts;
    poption **options = popts->options;
    for (i=0;i<nopts;++i) {
       if (poption_matches(options[i],key)) {
           poption_set_value(options[i],value);
           return;
       }
    }
    if (popts->allow_unused_options) return;
    fprintf(stderr,"program_options_set_value no such option %s\n",key);
    exit(EXIT_FAILURE);
}

const char * program_options_get_value(program_options *popts,const char *key)
{
    size_t i;
    size_t nopts=popts->nopts;
    poption **options = popts->options;
    for (i=0;i<nopts;++i) {
       if (poption_matches(options[i],key)) {
           return options[i]->val;
       }
    }
    fprintf(stderr,"program_options_get_value no such option %s\n",key);
    exit(EXIT_FAILURE);
}

int program_options_has_valid_option(program_options *popts,
 const char *key)
{
    size_t i;
    size_t nopts=popts->nopts;
    poption **options = popts->options;
    for (i=0;i<nopts;++i) {
       if (poption_matches(options[i],key)) {
          if (options[i]->stat!=-1) return 1; 
          return 0;
       }
    }
    return 0;
}

int program_options_has_option(program_options *popts,const char *key)
{
    size_t i;
    size_t nopts=popts->nopts;
    poption **options = popts->options;
    for (i=0;i<nopts;++i) {
       if (poption_matches(options[i],key)) {
           return 1;
       }
    }
    return 0;
}

int program_options_is_valid(program_options *popts)
{
    size_t i;
    size_t nopts=popts->nopts;
    poption **options = popts->options;
    for (i=0;i<nopts;++i) {
       if (poption_is_required(options[i]) && !(poption_is_valid(options[i]))) {
           return 0;
       }
    }
    return 1;
}

void program_options_grow(program_options* popts)
{
    size_t i;
    size_t new_cap = popts->cap + popts->cap;
    poption **options = popts->options;
    poption **new_options = (poption**)Malloc(sizeof(poption*)*new_cap);
    size_t nopts=popts->nopts;

    fprintf(stderr,"program options grow\n");
    for (i=0;i<nopts;++i) {
      new_options[i] = poption_init2(options[i]);
      poption_free(options[i]);
    }
    free(options);
    popts->options = new_options;
    popts->cap = new_cap;    
}

void program_options_add_option(program_options *popts,
  const char *key,const char *des,const char *val,int req)
{
    if (popts->cap <= popts->nopts) program_options_grow(popts);
    (popts->options)[popts->nopts] = poption_init(key,des,val,req);
    (popts->nopts) += 1;
}

program_options * program_options_init()
{
    program_options *popts = MALLOC_PTR(program_options);
    popts->cap = 128;
    popts->nopts = 0;
    popts->prog_name = 0x0;
    poption ** options = (poption**)Malloc(sizeof(poption*)*popts->cap);
    popts->options = options;
    popts->allow_unused_options = 0;
    return popts;
}

void program_options_free(program_options *popts)
{
   size_t i;
   size_t nopts= popts->nopts;
   poption **options = popts->options; 
   if (popts->prog_name) free(popts->prog_name);
   for (i=0;i<nopts;++i) poption_free(options[i]);
   free(options);
   free(popts); 
}

void program_options_print_help(program_options *p)
{
   size_t i;
   size_t nopts=p->nopts;
   poption **options = p->options;
   fprintf(stderr,"Usage is : %s [OPTIONS]\n",p->prog_name);
   for (i=0;i<nopts;++i) {
       poption_print(options[i],stderr);
   }
   program_options_free(p);
   exit(EXIT_FAILURE);   
}

void program_options_print(program_options *p,FILE *fp)
{
   size_t i;
   size_t nopts=p->nopts;
   poption **options = p->options;
   fprintf(fp,"Options for %s\n",p->prog_name);
   for (i=0;i<nopts;++i) {
       poption_print(options[i],fp);
   }
}

void program_options_write_options_file(program_options *popts,const char *file_name)
{
   size_t i;
   size_t nopts = popts->nopts;
   poption **options = popts->options;
   FILE *fp = Fopen(file_name,"w");
   for (i=0;i<nopts;++i) {
      fprintf(fp,"%s",options[i]->key);
      if (options[i]->des) {
         fprintf(fp," \"%s\"",options[i]->des);
      }
      if (options[i]->val) {
         fprintf(fp," %s",options[i]->val);
      }
      if (options[i]->r) fprintf(fp," required");
      fprintf(fp,"\n");
   }
   fclose(fp);
}



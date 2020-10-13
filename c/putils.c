#include "putils.h"

void *Malloc(size_t n) {
#ifdef HAVE_POSIX_MEMALIGN
    void * ptr;
    int e =  posix_memalign(ptr,PUTILS_ALIGN,n);
    if (ptr && !e) return ptr;
#else
    void * ptr = malloc(n);
    if (ptr) return ptr;
#endif
    fprintf(stderr,"allocation failed for %uz bytes\n",n);
    exit(EXIT_FAILURE);
}

void *Calloc(size_t n) {
    void * ptr = Malloc(n);
    memset(ptr,0x0,n);
    return ptr;
}

void *Realloc(void *ptr,size_t old_n,size_t n)
{
    void *pnew = Malloc(n);
    memcpy(pnew,ptr,old_n);
    free(ptr);
    ptr = pnew;
    return ptr;
}

FILE *Fopen(const char *name,const char *mode)
{
    FILE *fp = fopen(name,mode);
    if (fp) {
       return fp; 
    }    
    fprintf(stderr,"Could not open the file %s in mode %s\n",name,mode);
    exit(EXIT_FAILURE);
}    

FILE *Fmemopen(char *ptr,size_t buff_size,const char *mode)
{
    FILE *fp = fmemopen(ptr,buff_size,mode);
    if (fp) {
       return fp; 
    }    
    fprintf(stderr,"Could not open the buffer if size %uz in mode %s\n",buff_size,mode);
    exit(EXIT_FAILURE);
}    

void parse_error(const char *mess) {
    fprintf(stderr,"parse error %s\n",mess);
    exit(EXIT_FAILURE);
}

void format_error(const char *mess) {
    fprintf(stderr,"format error %s\n",mess);
    exit(EXIT_FAILURE);
}

void write_error(const char *mess) {
    fprintf(stderr,"write error %s\n",mess);
    exit(EXIT_FAILURE);
}

void read_error(const char *mess) {
    fprintf(stderr,"read error %s\n",mess);
    exit(EXIT_FAILURE);
}

size_t expand_string(char *sline,const char *delimiters,char **tokens)
{
    size_t k = 0;
    char *end_ptr;
    char *str = strtok_r(sline,delimiters,&end_ptr);
    while (str) {
        tokens[k] = str;
        ++k;
        str = strtok_r(0x0,delimiters,&end_ptr);
    }
    return k;
}

int Getline(char **sline,size_t *line_sz,FILE *fptr)
{
    int e = getline(sline,line_sz,fptr);
    if (e==-1) {
        if (feof(fptr)) return -1;
        read_error(" in getline\n");
    }
    return e;
}

size_t Fwrite(const void *ptr,size_t obj_size,size_t obj_cnt,FILE *fp)
{
    size_t s = fwrite(ptr,obj_size,obj_cnt,fp);
    if (s==obj_cnt) return s;
    fprintf(stderr,"error in fwrite wrote %uz of %uz\n",s,obj_cnt);
    write_error(" Fwrite");
}

size_t Fread(void *ptr,size_t obj_size,size_t obj_cnt,FILE *fp)
{
    size_t s = fread(ptr,obj_size,obj_cnt,fp);
    if (s==obj_cnt) return s;
    fprintf(stderr,"error in fread read %uz of %uz\n",s,obj_cnt);
    read_error(" Fread");
}

char * Strdup(const char *str)
{
     char * new_str = strdup(str);
     if (new_str) return new_str;
     fprintf(stderr,"cannot allocate memory to hold string %s\n",str);
     exit(EXIT_FAILURE);   
}

char ** tokens_init()
{
    char ** t = (char**)Malloc(sizeof(char*)*MAX_TOKENS);
    return t; 
}    

char * string_alloc()
{
    return (char*)Malloc(MAX_LINE_SIZE);
}


#ifndef PUTILS_H
#define PUTILS_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_SIZE 1025
#define MAX_TOKENS 512
#define PUTILS_ALIGN 32

void * Malloc(size_t n);
void * Calloc(size_t n);
void * Realloc(void *p,size_t old_n,size_t new_n);

#define MALLOC_PTR(type) (type*)Malloc(sizeof(type))

#define MALLOC_ARRAY(type,n) (type*)Malloc(sizeof(type)*n)

FILE *Fopen(const char *name,const char *mode);
FILE *Fmemopen(char * buffer,size_t buffer_size,const char *mode);
void parse_error(const char *mess);
void format_error(const char *mess);
void write_error(const char *mess);
void read_error(const char *mess);
size_t expand_string(char *str,const char *delimiters,char **tokens);
int Getline(char **sline,size_t *line_sz,FILE *fptr);
size_t Fwrite(const void *ptr,size_t obj_size,size_t obj_cnt,FILE *fp);
size_t Fread(void *ptr,size_t obj_size,size_t obj_cnt,FILE *fp);
char * Strdup(const char *str);
char ** tokens_init();
char *string_alloc();

#define tokens_free(t) free(t)

#endif

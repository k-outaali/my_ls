#ifndef MY_LS_H
#define MY_LS_H

#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>


#define check(A, M, ...) if(A){fprintf(stderr, "ERROR: " M "\n" ,##__VA_ARGS__); goto fail;}
#define BUFFER_LEN 256
#define TC_NRM  "\x1B[0m"
#define TC_RED  "\x1B[1;31m"
#define TC_GRN  "\x1B[1;32m"
#define TC_YEL  "\x1B[1;33m"
#define TC_BLU  "\x1B[1;34m"
#define TC_MAG  "\x1B[1;35m"
#define TC_CYN  "\x1B[1;36m"
#define TC_WHT  "\x1B[1;37m"


void print_permissions(mode_t mode);

void print_type(unsigned char type);

void print_owner_and_group(struct stat *file_info);

void print_all(struct dirent *cur, DIR *dir);

void print_long(struct dirent *cur, DIR *dir);

void print_default(struct dirent *cur, DIR *dir);

void print_by_color(struct dirent *cur);

void print_help();





#endif
#ifndef MY_LS_H
#define MY_LS_H

#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>


#define check(A, M, ...) if(A){fprintf(stderr, "ERROR: " M "\n" ,##__VA_ARGS__); goto fail;}
#define BUFFER_LEN 256

void print_permissions(mode_t mode);

void print_type(unsigned char type);

void print_owner_and_group(struct stat *file_info);

void print_all(struct dirent *cur, DIR *dir);

void print_long(struct dirent *cur, DIR *dir);

void print_default(struct dirent *cur, DIR *dir);

void print_help();





#endif
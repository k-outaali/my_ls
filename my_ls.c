#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

#include "my_ls.h"

char g_path[BUFFER_LEN] = {'\0'};


int main(int argc, char **argv){

    char *resolved_path;
    if(argc > 1){
        if(argv[1][0] == '-'){
            resolved_path = realpath(".", g_path);
        }
        else{
            resolved_path = realpath(argv[1], g_path);
        }
    }
    else{
       resolved_path = realpath(".", g_path); 
    }
    check(resolved_path == NULL, "realpath failed");
    check(strcat(g_path, "/") == NULL, "strcat failed");

    DIR *dir= opendir(g_path);
    check(dir == NULL, "opendir failed");

    struct dirent *cur;
    
    int opt = getopt(argc, argv, "alh");
    if(opt == -1){
        print_default(cur, dir);
    }
    while(opt != -1){
        switch(opt){
            case 'a':
                print_all(cur, dir);
                break;
            case 'l':
                print_long(cur, dir);
                break;
            case 'h':
                print_help();
                break;
            default:
                printf("use -h for help\n");
                return 1;
        }

        opt = getopt(argc, argv, "alh");
    }
    
    return 0;
    fail:
        return 1;
}


void print_permissions(mode_t mode){
        printf( (mode & S_IRUSR) ? "r" : "-");
        printf( (mode & S_IWUSR) ? "w" : "-");
        printf( (mode & S_IXUSR) ? "x" : "-");
        printf( (mode & S_IRGRP) ? "r" : "-");
        printf( (mode & S_IWGRP) ? "w" : "-");
        printf( (mode & S_IXGRP) ? "x" : "-");
        printf( (mode & S_IROTH) ? "r" : "-");
        printf( (mode & S_IWOTH) ? "w" : "-");
        printf( (mode & S_IXOTH) ? "x" : "-");
}


void print_type(unsigned char type){
        switch(type){
            case DT_REG:
                printf("-");
                break;
            case DT_DIR:
                printf("d");
                break;
            case DT_BLK:
                printf("b");
                break;
            case DT_CHR:
                printf("c");
                break;
            case DT_LNK:
                printf("l");
                break;
            case DT_SOCK:
                printf("s");
                break;
            default:
                printf("u");
        }
}


void print_owner_and_group(struct stat *file_info) {
    
    struct passwd *pw = getpwuid(file_info->st_uid);
    check(pw == NULL, "getpwuid failed");
    printf(" %10s", pw->pw_name);

    struct group *gr = getgrgid(file_info->st_gid);
    check(pw == NULL, "getpwuid failed");
    printf("  %10s", gr->gr_name);

    fail:
        return;
}

void print_by_color(struct dirent *cur){
    if(cur->d_type == DT_DIR){
        printf("%s%s%s ", TC_BLU, cur->d_name, TC_NRM);
    }
    else if(cur->d_type == DT_LNK){
        printf("%s%s%s ", TC_CYN, cur->d_name, TC_NRM);
    }
    else {
        printf("%s%s%s ", TC_NRM, cur->d_name, TC_NRM);
    }
}

void print_all(struct dirent *cur, DIR *dir){
    cur = readdir(dir);
    while(cur != NULL){
        print_by_color(cur);
        cur = readdir(dir);
    }
    printf("\n");
}


void print_long(struct dirent *cur, DIR *dir){

    char path[BUFFER_LEN] = {'\0'};
    cur = readdir(dir);
    while(cur != NULL){
        struct stat file_info;
        check(strcat(path, g_path) == NULL, "strcat failed");
        check(strcat(path, cur->d_name) == NULL, "strcat failed");
        check(stat(path, &file_info) != 0, "stats failed");
        mode_t mode = file_info.st_mode;
        print_type(cur->d_type); //type
        print_permissions(mode); // premissions
        printf("  %3ld", (long) file_info.st_nlink);// link count
        print_owner_and_group(&file_info);// owner and group
        printf("  %10ld", file_info.st_size); // file size
        struct tm *time = localtime(&file_info.st_mtime);
        check(time == NULL, "lcaltime failed");
        char buffer[64];
        strftime(buffer, sizeof(buffer), "%b %d %H:%M", time);
        printf("  %s  ", buffer); // last time modified
        print_by_color(cur); // name
        printf("\n");
        cur = readdir(dir);
        check(memset(path, '\0', BUFFER_LEN) == NULL, "memset failed");
    }
    fail:
        return;
}


void print_default(struct dirent *cur, DIR *dir){
    cur = readdir(dir);
    while(cur != NULL){
        if(cur->d_name[0] != '.'){
           print_by_color(cur);
        }
        cur = readdir(dir);
    }
    printf("\n");
}

void print_help(){
    printf(" my_ls is a simple ls implementation by KHALID OUTAALI here is how to to use it.\n");
    printf(" ./my_ls [dir] [-alh].\n");
    printf(" -a same as 'ls -a' list hidden files and directories.\n");
    printf(" -l same as 'ls -al' list hidden files and directories in more details.\n");
    printf(" -h for help\n");
}
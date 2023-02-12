#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#define check(A, M, ...) if(A){fprintf(stderr, "ERROR: " M "\n" ,##__VA_ARGS__); goto fail;}

char g_path[256] = {'\0'};
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

void print_all(struct dirent *cur, DIR *dir){
    cur = readdir(dir);
    while(cur != NULL){
       
        printf("%s  ", cur->d_name);
        cur = readdir(dir);
    }
    printf("\n");
}

void print_long(struct dirent *cur, DIR *dir){

    char path[256] = {'\0'};
    cur = readdir(dir);
    while(cur != NULL){
        struct stat file_info;
        strcat(path, g_path);
        strcat(path, cur->d_name);
        check(stat(path, &file_info) != 0, "stats failed");
        mode_t mode = file_info.st_mode;
        print_type(cur->d_type); //type
        print_permissions(mode); // premission
        printf("  %ld", (long) file_info.st_nlink);// link count
        printf(" %s\n", cur->d_name); // name
        cur = readdir(dir);
        memset(path, '\0', 256);
    }
    fail:
        return;
}

void print_default(struct dirent *cur, DIR *dir){
    cur = readdir(dir);
    while(cur != NULL){
        if(cur->d_name[0] != '.'){
            printf("%s  ", cur->d_name);
        }
        cur = readdir(dir);
    }
    printf("\n");
}
int main(int argc, char **argv){

    strcpy(g_path, argv[1]);
    DIR *dir= opendir(argv[1]);
    check(dir == NULL, "opendir failed");

    struct dirent *cur;
    
    int opt = getopt(argc, argv, "al");
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
            default:
                printf("usage ... \n");
        }

        opt = getopt(argc, argv, "al:");
    }
    
    return 0;
    fail:
        return 1;
}



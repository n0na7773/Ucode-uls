#include "../inc/uls.h"

void display(char **file_names, int name_amnt, char *flags){
    for(int i = 0; ; ++i){
        if(flags[i] == 'l'){
            for(int k = 0; k < name_amnt; ++k){
                print_file_names_l_flag(file_names, name_amnt, k);
            }
            break;
        }
        else if(flags[i] != '\0'){
            for(int k = 0; k < name_amnt; ++k){
                    print_file_names(file_names, k);
                    if(k < name_amnt) mx_printchar('\n');
                }
            
            break;
        }
    }
}

void uls(char *path, char *flags){
    DIR *dir;
    struct dirent *dptr;

    int name_amnt = 0;
    
    dir = opendir(path);
    if(dir == NULL) mx_printerr("opendir error\n");
    unsigned long max_name_len = 0;
    while((dptr = readdir(dir)) != NULL){
        if(max_name_len < strlen(dptr->d_name))
            max_name_len = strlen(dptr->d_name);
        name_amnt++;
    }
    closedir(dir);

    char *tot_path = (char *)malloc(name_amnt * 2 * sizeof(char));
    int len = mx_strlen(tot_path);
    if(len > 0){
        if(tot_path[len - 1] == '/')
            tot_path[len - 1] = '\0';
    }
    if(path[0] == '.' || path[0] == '/'){
        mx_strcat(tot_path, path);
    }
    else{
        mx_strcat(tot_path, path);
    }
    mx_strcat(tot_path, "/");

    char **file_names = (char **)malloc(name_amnt * sizeof(char *));
    for(int i = 0; i < name_amnt; ++i){
        file_names[i] = NULL;
    }

    //Get all file names in this directory
    dir = opendir(path);
    int x = 0;
    for(int i = 0; i < name_amnt; ++i){
        dptr = readdir(dir);
        if(dptr == NULL){
            mx_printerr("readdir error\n");
        }
        if(dptr->d_name[0] == '.') continue;
        file_names[x] = mx_strjoin(tot_path, dptr->d_name);
        x++;
    }
    closedir(dir);
    
    for(int j = 0; j < x; ++j){
        for(int k = 0; k < x; ++k){
            if(mx_strcmp(file_names[j], file_names[k]) < 0){
                char *temp = file_names[j];
                file_names[j] = file_names[k];
                file_names[k] = temp;
            }
        }
    }
    if(flags[0] == 'l'){
        struct stat buf;
        static int total = 0;
        for(int k = 0; k < x; ++k){
            if(lstat(file_names[k], &buf) == -1){
                mx_printerr("stat error\n");
                exit(1);
            }
            total += buf.st_blocks;
        }
        mx_printstr("total ");
        mx_printstr(mx_itoa(total));
        mx_printchar('\n');
    }

    display(file_names, x, flags);
}

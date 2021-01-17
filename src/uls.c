#include "../inc/uls.h"

int mx_arr_size(char **arr) {
    int i = 0;
    for(;arr[i] != NULL; i++);
    return i;
}

void display(char **file_names, int name_amnt, char *flags){
    for(int i = 0; ; ++i){
        if(flags[i] == 'l') {
            File_i **out = (File_i **) malloc(name_amnt * sizeof(File_i *));
            for(int j = 0; j < name_amnt; j++) {
                out[j] = (File_i *) malloc(sizeof(File_i));
            }
            
            for(int k = 0; k < name_amnt; ++k){
                struct stat buf;
                if(lstat(file_names[k], &buf) == -1){
                    mx_printerr("stat error\n");
                    exit(1);
                }
                out[k]->stat = buf;
                out[k]->perms = mx_strdup(mx_perms(&buf, file_names[k]));
                out[k]->nlink = mx_itoa(buf.st_nlink);
                out[k]->user = mx_user(&buf);
                out[k]->group = mx_group(&buf);
                out[k]->size = mx_itoa(buf.st_size); 

                if (S_ISLNK(buf.st_mode)) {
                    char link_buf[1024];
                    ssize_t len;
                    if ((len = readlink(file_names[k], link_buf, sizeof(link_buf)-1)) != -1) {
                        char **temp = mx_strsplit(file_names[k], '/');
                        out[k]->file_name = mx_strdup(temp[mx_arr_size(temp) - 1]);

                        for(int j = 0; temp[j] != NULL; j++) mx_strdel(&temp[j]);
                        free(temp);
                        temp = NULL;                       

                        link_buf[len] = '\0';
                        out[k]->file_name = mx_strjoin(out[k]->file_name, " -> ");
                        out[k]->file_name = mx_strjoin(out[k]->file_name, link_buf);
                        
                    }
                }
                else {
                    char **temp = mx_strsplit(file_names[k], '/');
                    out[k]->file_name = mx_strdup(temp[mx_arr_size(temp) - 1]);

                    for(int j = 0; temp[j] != NULL; j++) mx_strdel(&temp[j]);
                    free(temp);
                    temp = NULL;
                }
            }
            print_file_names_l_flag(out, name_amnt);
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
void display_file(char **file_names, int id, char *flags){
    if(flags[0] == 'l'){
        File_i **out = (File_i **) malloc(sizeof(File_i *) * id);
        for(int i = 0; i < id; ++i){
            out[i] = (File_i *) malloc(sizeof(File_i));
        }   
        for(int i = 0; i < id; ++i){
            struct stat buf;
            if(lstat(file_names[i], &buf) == -1){
                mx_printerr("stat error\n");
                exit(1);
            }
            
            out[i]->stat = buf;
            out[i]->perms = mx_strdup(mx_perms(&buf, file_names[i]));
            out[i]->nlink = mx_itoa(buf.st_nlink);
            out[i]->user = mx_user(&buf);
            out[i]->group = mx_group(&buf);
            out[i]->size = mx_itoa(buf.st_size);
            
            if (S_ISLNK(buf.st_mode)) {
                char link_buf[1024];
                ssize_t len;
                if ((len = readlink(file_names[i], link_buf, sizeof(link_buf)-1)) != -1) {
                    char **temp = mx_strsplit(file_names[i], '/');
                    out[i]->file_name = mx_strdup(temp[mx_arr_size(temp) - 1]);

                    for(int j = 0; temp[j] != NULL; j++) mx_strdel(&temp[j]);
                    free(temp);
                    temp = NULL; 

                    link_buf[len] = '\0';
                    out[i]->file_name = mx_strjoin(out[i]->file_name, " -> ");
                    out[i]->file_name = mx_strjoin(out[i]->file_name, link_buf);
                    
                }     
            }
            else {
                out[i]->file_name = mx_strdup(file_names[i]);
            }
        }
        print_file_names_l_flag(out, id);
    }
    else {
        for(int i = 0; i < id ; ++i){
            for(int j = 0; file_names[i][j] != '\0'; ++j) mx_printchar(file_names[i][j]);
            mx_printchar('\n');
        }
    }
    
}

void uls(char *path, char *flags){
    DIR *dir;
    struct dirent *dptr;

    int name_amnt = 0;
    dir = opendir(path);
    if (!dir) {
        mx_printerr("uls: ");
        mx_printerr(path);
        mx_printerr(": No such file or directory\n");
        exit(1);
    }
    while((dptr = readdir(dir)) != NULL){
        name_amnt++;
    }
    if(closedir(dir) == -1) {
        exit(1);
    }

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
    if (!dir) {
        mx_printerr("uls: ");
        mx_printerr(path);
        mx_printerr(": No such file or directory\n");
        exit(1);
    }
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
    if(closedir(dir) == -1) {
        exit(1);
    }

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
        total = 0;
    }
    display(file_names, x, flags);
}

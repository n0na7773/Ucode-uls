#include "../inc/uls.h"

void check_flags(char *flags){
    for(int i = 0; flags[i] != '\0'; ++i){
        if(flags[i] == 'l') continue;
        else{
            mx_printerr("uls: illegal option -- ");
            mx_printerr(&flags[i]);
            mx_printerr("\n");
            mx_printerr("usage: uls [-l] [file ...]\n");
            exit(1);
        }
    }
}

int main(int argc, char **argv) {
    bool error = false;
    char *flags = (char *)malloc((argc - 1) * sizeof(char));
    int flag_amnt = 0;
    int index = 0;
    for(int i = 1; i < argc; ++i){
        if(argv[i][0] == '-'){
            for(int j = 1; j < mx_strlen(argv[i]); ++j){
                flags[index] = argv[i][j];
                index++;    
            }
            flag_amnt++;
        }
    }
    flags[index] = '\0';

    check_flags(flags);
    
    //For ./uls and ./uls -l only
    char **path = (char **)malloc((argc - 1) * sizeof(char *));
    if(flag_amnt + 1 == argc){
        path[0] = mx_strdup(".");
        uls(path[0], flags);
    }
    
    struct stat buf;
    for(int i = 1; i < argc; ++i){
        for(int j = 1; j < argc; ++j){
            if(mx_strcmp(argv[i], argv[j]) < 0){
                char *temp = argv[i];
                argv[i] = argv[j];
                argv[j] = temp;
            }
        }
    }

    int i = 1;
    while(i < argc){
        if(argv[i][0] == '-'){
            i++;
            continue;
        }
        else{
            path[0] = mx_strdup(argv[i]);
            if(stat(argv[i], &buf) == -1){
                mx_printerr("uls: ");
                mx_printerr(argv[i]);
                mx_printerr(": No such file or directory\n");
                error = true;
                i++;
                continue;
            }

            if(S_ISDIR(buf.st_mode)){
                if(argc > 2 && flag_amnt == 0){
                    mx_printstr(path[0]);
                    mx_printstr(":\n");
                }
                uls(path[0], flags);
                if(i < argc - 1){
                    mx_printchar('\n');
                }
                i++;
            }
            else{
                display(path, 1, flags);
                i++;
            }
        }
    }
    if(error) exit(1); 
}

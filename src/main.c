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
    
    char **path = (char **)malloc((argc + 1) * sizeof(char *));

    for (int i = 0; i < argc + 1; i++){
        path[i] = NULL;
    }
    if(argc == 1) {
        path[0] = mx_strdup("./");
    }
    if(argc == 2) {
        if(mx_strcmp(argv[1], "-l") == 0) {
            path[0] = mx_strdup("./");
        }
        else {
            path[0] = mx_strdup(argv[1]);
        }
    }
    if(argc > 2) {
        int j = 0;
        int i = 0;
        if (mx_strcmp(argv[1], "-l") == 0) i = 2;
        else i = 1;
        for(; i < argc; i++) {
            path[j] = mx_strdup(argv[i]);
            j++;
        }
    }

    for (int i = 0;path[i] != NULL; i++) {
		for (int j = 0;path[j] != NULL; j++) {
			if (mx_strcmp(path[i], path[j]) < 0) {
				void *tmp = path[i];
				path[i] = path[j];
				path[j] = tmp;
			}
		}
	}

    for (int i = 0; path[i] != NULL; i++) {
		for (int j = 0; path[j+1] != NULL; j++) {
            struct stat temp;
            struct stat temp1;
            stat(path[j], &temp);
            stat(path[j+1], &temp1);
            if(S_ISDIR(temp.st_mode) && !S_ISDIR(temp1.st_mode)) {  
                void *tmp = path[j];
				path[j] = path[j+1];
				path[j+1] = tmp;
            }   
		}
	}

    for(int i = 0; path[i] != NULL; i++) {    
        struct stat buf;
        
        if(stat(path[i], &buf) == -1){
            mx_printerr("uls: ");
            mx_printerr(path[i]);
            mx_printerr(": No such file or directory\n");
            continue;
        }
        
        if(S_ISDIR(buf.st_mode)){
            if((argc > 3 && flag_amnt == 1) || (argc > 2 && flag_amnt == 0)){
                mx_printstr(path[i]);
                mx_printstr(":\n");
            }
            uls(path[i], flags);
            if(i < argc - flag_amnt - 2){
                mx_printchar('\n');
            }
        }
        else {
            display_file(path, i, flags);
        }
    }
    if(error) exit(1); 
}

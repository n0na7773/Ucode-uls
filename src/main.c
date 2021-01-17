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

    if(argv[1][0] == '-'){
        for(int j = 1; j < mx_strlen(argv[1]); ++j){
            if (flags[j] != flags[j-1] || index == 0) flags[index] = argv[1][j];
            index++;    
        }
        flag_amnt++;
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
        if(flag_amnt == 1) {
            path[0] = mx_strdup("./");
        }
        else {
            path[0] = mx_strdup(argv[1]);
        }
    }
    if(argc > 2) {
        int j = 0;
        int i = 0;
        if (flag_amnt == 1) i = 2;
        else i = 1;
        for(; i < argc; i++) {
            path[j] = mx_strdup(argv[i]);
            j++;
        }
    }
    //Total sort
    for (int i = 0;path[i] != NULL; i++) {
		for (int j = 0;path[j] != NULL; j++) {
			if (mx_strcmp(path[i], path[j]) < 0) {
				void *tmp = path[i];
				path[i] = path[j];
				path[j] = tmp;
			}
		}
	}
    //Error output
    int errors_amnt = 0;
    for (int i = 0;path[i] != NULL; i++) {
        struct stat buf;
        if(stat(path[i], &buf) == -1){
            mx_printerr("uls: ");
            mx_printerr(path[i]);
            mx_printerr(": No such file or directory\n");
            error = true;
            errors_amnt++;
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
    
    //Move error to the end
    for (int i = 0; path[i] != NULL; i++) {
		for (int j = 0; path[j+1] != NULL; j++) {
            struct stat temp;
            if(stat(path[j], &temp) == -1) {  
                void *tmp = path[j];
				path[j] = path[j+1];
				path[j+1] = tmp;
            }   
		}
	}

    
    int files_amnt = 0;
    
    for(int i = 0; path[i] != NULL; i++) {    
        struct stat buf;
        
        if(stat(path[i], &buf) == -1){
            continue;
        }
        
        if(S_ISDIR(buf.st_mode)){
            if((argc > 3 && flag_amnt == 1) || (argc > 2 && flag_amnt == 0)){
                mx_printstr(path[i]);
                mx_printstr(":\n");
            }
            uls(path[i], flags);
            if(i < argc - flag_amnt - errors_amnt - 2){
                mx_printchar('\n');
            }
        }
        else {
            files_amnt++;
            struct stat temp1;
            stat(path[i+1], &temp1);
            if(i < argc - flag_amnt - 1 && (S_ISDIR(temp1.st_mode) || path[i+1] == NULL)){
                display_file(path, files_amnt, flags);
                if (S_ISDIR(temp1.st_mode)) mx_printchar('\n');
            }
        }
    }
    if(error) exit(1); 
}

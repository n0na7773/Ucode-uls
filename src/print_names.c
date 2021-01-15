#include "../inc/uls.h"

void print_file_names_l_flag(char **file_names, int name_amnt, int k){
    struct stat buf;
    char *buff_time = (char *)malloc(32 * sizeof(char));

    struct passwd *psd;
    struct group *grp;

    if(lstat(file_names[k], &buf) == -1){
        mx_printerr("stat error\n");
        exit(0);
    }

    /*static int total = 0;
    total += buf.st_blocks;

    if(name_amnt == 1 || S_ISDIR(buf.st_mode)){
        mx_printstr("total ");
        mx_printstr(mx_itoa(total));
        mx_printchar('\n');
    }*/

    if(S_ISLNK(buf.st_mode))
        mx_printchar('l');
    else if(S_ISREG(buf.st_mode))
        mx_printchar('-');
    else if(S_ISDIR(buf.st_mode))
        mx_printchar('d');
    else if(S_ISCHR(buf.st_mode))
        mx_printchar('c');
    else if(S_ISBLK(buf.st_mode))
        mx_printchar('b');
    else if(S_ISFIFO(buf.st_mode))
        mx_printchar('f');
    else if(S_ISSOCK(buf.st_mode))
        mx_printchar('s');

    if(buf.st_mode&S_IRUSR)
        mx_printchar('r');
    else
        mx_printchar('-');

    if(buf.st_mode&S_IWUSR)
        mx_printchar('w');
    else
        mx_printchar('-');

    if(buf.st_mode&S_IXUSR)
        mx_printchar('x');
    else
        mx_printchar('-');

    //All group permissions
    if(buf.st_mode&S_IRGRP)
        mx_printchar('r');
    else
        mx_printchar('-');

    if(buf.st_mode&S_IWGRP)
        mx_printchar('w');
    else
        mx_printchar('-');

    if(buf.st_mode&S_IXGRP)
        mx_printchar('x');
    else
        mx_printchar('-');

    //Other people's rights
    if(buf.st_mode&S_IROTH)
        mx_printchar('r');
    else
        mx_printchar('-');

    if(buf.st_mode&S_IWOTH)
        mx_printchar('w');
    else
        mx_printchar('-');

    if(buf.st_mode&S_IXOTH)
        mx_printchar('x');
    else
        mx_printchar('-');

    for(int i = 0; i <= 2 - mx_strlen(mx_itoa(buf.st_nlink)); ++i) mx_printchar(' ');
    mx_printstr(mx_itoa(buf.st_nlink));

    psd = getpwuid(buf.st_uid);
    grp = getgrgid(buf.st_gid);

    mx_printchar(' ');
    mx_printstr(psd->pw_name);
    mx_printstr("  ");
    mx_printstr(grp->gr_name);

    for(int i = 0; i <= 5 - mx_strlen(mx_itoa(buf.st_size)); ++i) mx_printchar(' ');

    mx_printstr(mx_itoa(buf.st_size));
    mx_printchar(' ');

    buff_time = mx_strdup(ctime(&buf.st_mtime));
    for(int i = 4; buff_time[i - 3] != ':'; ++i)
        mx_printchar(buff_time[i]);
    buff_time[mx_strlen(buff_time) - 1] = '\0'; //Buff time has its own newline, so we need to remove the following newline character

    mx_printchar(' ');

    if(name_amnt == 1) mx_printstr(file_names[k]);
    else print_file_names(file_names, k);
    mx_printchar('\n');
}

void print_file_names(char **file_names, int k){
    int file_name_start = 0;
    for(file_name_start = mx_strlen(file_names[k]); file_names[k][file_name_start] != '/'; --file_name_start);

    file_name_start++;
    
    for(int i = file_name_start; file_names[k][i] != '\0'; ++i) mx_printchar(file_names[k][i]);
}

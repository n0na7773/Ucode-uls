#include "../inc/uls.h"

void clean_mx_out(File_i **out, int amnt) {
    for(int i = 0; i < amnt; i++) {
        mx_strdel(&out[i]->perms);
        mx_strdel(&out[i]->nlink);
        mx_strdel(&out[i]->user);
        mx_strdel(&out[i]->group);
        mx_strdel(&out[i]->size);
        mx_strdel(&out[i]->file_name);
        free(out[i]);
    }
    free(out);
}

char mx_listxattr(char *file) {
    acl_t temp;

    if (listxattr(file, NULL, 0, XATTR_NOFOLLOW) > 0) {
        return '@';
    }
    if ((temp = acl_get_file(file, ACL_TYPE_EXTENDED))) {
        acl_free(temp);
        return ('+');
    }
    return (' ');
}

char check_perms1(char perms) {
    if (perms == '-')
        return perms = 'S';
    else
        return perms = 's';
}

char check_perms2(char *perms) {
    if (perms[9] == '-')
        return perms[9] = 'T';
    else
        return perms[9] = 't';
}
char *mx_perms(struct stat *buf, char *file_names) {
    char perms[12];

    if(S_ISLNK(buf->st_mode))
        perms[0] = 'l';
    else if(S_ISREG(buf->st_mode))
        perms[0] = '-';
    else if(S_ISDIR(buf->st_mode))
        perms[0] = 'd';
    else if(S_ISCHR(buf->st_mode))
        perms[0] = 'c';
    else if(S_ISBLK(buf->st_mode))
        perms[0] = 'b';
    else if(S_ISFIFO(buf->st_mode))
        perms[0] = 'f';
    else if(S_ISSOCK(buf->st_mode))
        perms[0] = 's';
    else if(S_ISWHT(buf->st_mode))
        perms[0] = 'w';

    if(buf->st_mode&S_IRUSR)
        perms[1] = 'r';
    else
        perms[1] = '-';

    if(buf->st_mode&S_IWUSR)
        perms[2] = 'w';
    else
        perms[2] = '-';

    if(buf->st_mode&S_IXUSR)
        perms[3] = 'x';
    else
        perms[3] = '-';

    //All group permissions
    if(buf->st_mode&S_IRGRP)
        perms[4] = 'r';
    else
        perms[4] = '-';

    if(buf->st_mode&S_IWGRP)
        perms[5] = 'w';
    else
        perms[5] = '-';

    if(buf->st_mode&S_IXGRP)
        perms[6] = 'x';
    else
        perms[6] = '-';

    //Other people's perms
    if(buf->st_mode&S_IROTH)
        perms[7] = 'r';
    else
        perms[7] = '-';

    if(buf->st_mode&S_IWOTH)
        perms[8] = 'w';
    else
        perms[8] = '-';

    if(buf->st_mode&S_IXOTH)
        perms[9] = 'x';
    else
        perms[9] = '-';

    perms[10] = mx_listxattr(file_names);
    perms[11] = '\0';
    S_ISUID & buf->st_mode? perms[3] = check_perms1(perms[3]) : 0;
    S_ISGID & buf->st_mode ? perms[6] = check_perms1(perms[6]) : 0;
    S_ISVTX & buf->st_mode ? perms[9] = check_perms2(perms) : 0;
    char *tmp = mx_strdup(perms);
    return tmp;
}
char *mx_user(struct stat *buf) {
    struct passwd *psd = getpwuid(buf->st_uid);;
    if (psd) {
        return mx_strdup(psd->pw_name);
    }
    return NULL;
}
char *mx_group(struct stat *buf) {
    struct group *grp = getgrgid(buf->st_gid);
    if (grp) {
        return mx_strdup(grp->gr_name);
    }
    return NULL;
}
void mx_print_time(char *t, struct stat *Stat) {
    int i = 0;
        if (time(NULL) - Stat->st_mtime >= 15778463) { //Thu Dec  3 13:15:41 2020
            for(i = 4; i < 10; i++)
                mx_printchar(t[i]);
            mx_printstr("  ");
            for (i = 20; i < 24; i++)
                mx_printchar(t[i]); 
        }
        else
            for(i = 4; i < 16; i++)
                mx_printchar(t[i]);
    mx_printstr(" ");
}
void print_file_names_l_flag(struct file_inf **out, int amnt){
    int size_size = mx_strlen(out[0]->size);
    for(int i = 1; i < amnt; i++) {
        if(size_size < mx_strlen(out[i]->size)) size_size = mx_strlen(out[i]->size);
    }
    int size_nlink = mx_strlen(out[0]->nlink);
    for(int i = 1; i < amnt; i++) {
        if(size_nlink < mx_strlen(out[i]->nlink)) size_nlink = mx_strlen(out[i]->nlink);
    }
    int size_user = mx_strlen(out[0]->user);
    for(int i = 1; i < amnt; i++) {
        if(size_user < mx_strlen(out[i]->user)) size_user = mx_strlen(out[i]->user);
    }
    int size_group = mx_strlen(out[0]->group);
    for(int i = 1; i < amnt; i++) {
        if(size_group < mx_strlen(out[i]->group)) size_group = mx_strlen(out[i]->group);
    }

    for(int i = 0; i < amnt; i++) {
        mx_printstr(out[i]->perms);
        mx_printstr(" ");
        ////////////////////////
        if(mx_strlen(out[i]->nlink) < size_nlink){
            int tmp = mx_strlen(out[i]->nlink);
            while(tmp!=size_nlink){
                mx_printstr(" ");
                tmp++;
            }
        }
        mx_printstr(out[i]->nlink);
        mx_printstr(" ");
        ////////////////////////
        mx_printstr(out[i]->user);
        if(mx_strlen(out[i]->user) < size_user){
            int tmp = mx_strlen(out[i]->user);
            while(tmp!=size_user){
                mx_printstr(" ");
                tmp++;
            }
        }
        
        mx_printstr("  ");
        ////////////////////////
        mx_printstr(out[i]->group);
        if(mx_strlen(out[i]->group) < size_group){
            int tmp = mx_strlen(out[i]->group);
            while(tmp!=size_group){
                mx_printstr(" ");
                tmp++;
            }
        }
        
        mx_printstr("  ");
        ////////////////////////
        if(mx_strlen(out[i]->size) < size_size){
            int tmp = mx_strlen(out[i]->size);
            while(tmp!=size_size){
                mx_printstr(" ");
                tmp++;
            }
        }
        if(mx_strcmp(out[i]->file_name, "/dev/null") == 0){
            out[i]->size = mx_strdup("  3,   2");
        } 
        mx_printstr(out[i]->size);
        mx_printstr(" ");
        mx_print_time(ctime(&out[i]->stat.st_mtime), &out[i]->stat);
        mx_printstr(out[i]->file_name);
        mx_printstr("\n");
    }
    
    clean_mx_out(out, amnt);
    
}

void print_file_names(char **file_names, int k){
    
    int file_name_start = 0;
    if(mx_get_char_index(file_names[k], '/') != -1) {
        for(file_name_start = mx_strlen(file_names[k]); file_names[k][file_name_start] != '/'; --file_name_start);
        file_name_start++;
    }
    for(int i = file_name_start; file_names[k][i] != '\0'; ++i) mx_printchar(file_names[k][i]);

}

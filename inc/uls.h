#ifndef ULS_H
#define ULS_H

#include "libmx.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <sys/acl.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>

typedef struct file_inf {
    struct stat stat;
    char *perms;
    char *nlink;
    char *user;
    char *group;
    char *size;
    char *file_name;
}   File_i;

char *mx_perms(struct stat *buf, char *file_names); 
char *mx_user(struct stat *buf); 
char *mx_group(struct stat *buf); 

// Command ls
void uls(char *path, char *flag);
void display(char **file_names, int name_amnt, char *flag);
void display_file(char **file_names, int k, char *flags);

// Print names
void print_file_names(char **file_names, int k);
void print_file_names_l_flag(struct file_inf **out, int k);

// Errors
void mx_printerr(const char *s);

#endif

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

#define PARAM_NONE 0        //Since bitwise operations are used later, let's introduce flag=0 when there are no parameters; then - a,-l,-R,-r, defined as 1, 2, 4, 8.
#define PARAM_A    1        //It happens to be 1, 10, 100, 1000 in binary. This is convenient for | and & in operations, such as having both a and r parameters, then flag is 1001.
#define PARAM_L    2        //Flag & PARAM_r can be used to determine whether or not one of these two parameters (e.g., R parameter) is included. If it is zero, there is no r parameter.
#define PARAM_R    4        //Others are similar.
#define PARAM_r    8

// Command ls
void uls(char *path, char *flag);
void display(char **file_names, int name_amnt, char *flag);

// Print names
void print_file_names(char **file_names, int k);
void print_file_names_l_flag(char **file_names, int name_amnt, int k);

// Errors
void mx_printerr(const char *s);

#endif

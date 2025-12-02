#ifndef RMAKE_MANAGER_H
#define RMAKE_MANAGER_H

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>

#include "rmake_str.h"


void rmake_init_sys();
bool rmake_dir_exist(char* path);
bool rmake_file_exist(char* path);

#endif
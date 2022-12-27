/**
 * @file   fs_info.c
 * @brief  This file contains the function definitions for fs_info.c
 * @author Matthew C. Lindeman
 * @date   December 27, 2022
 * @bug    None known
 * @todo   Nothing
 */
#ifndef FSI_H
#define FSI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../main/include/constants_macros.h"

#define INCLUDE_STRING "/include"
#define H_EXTENSION ".h"
#define UP_DIR "../"

typedef struct FS_INFO_T {
  char ** each_sub_dir;
  char * h_file_name;
  char * c_file_name;
  int qty_sub_dirs;
} fs_info;

fs_info * init_fs_info(char * path, char * c_name);
char * make_h_file_name(char * c_file_name);
fs_info * partition_sub_dirs(fs_info * the_fs_info, char * path);
char * relative_path(fs_info * dest, fs_info * src);
void debug_fs_info(fs_info * the_fs_info);
void free_fs_info(fs_info * the_fs_info);


#endif

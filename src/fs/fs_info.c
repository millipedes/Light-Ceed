/**
 * @file   fs_info.c
 * @brief  This file contains the functions relating to the fs_info data
 * structure.
 * @author Matthew C. Lindeman
 * @date   December 27, 2022
 * @bug    None known
 * @todo   Nothing
 */
#define _POSIX_C_SOURCE 200809L // C99 Standard for strn* funcs, POSIX only
#include "include/fs_info.h"

/**
 * This function inializes an fs_info structure.
 * @param         path - The path from which the fs_info is generated.
 * @param  c_file_name - The name of the c file from which the fs_info is
 * inialized.
 * @return the_fs_info - The newly inited structure.
 */
fs_info * init_fs_info(char * path, char * c_file_name) {
  fs_info * the_fs_info = calloc(1, sizeof(struct FS_INFO_T));
  the_fs_info->qty_sub_dirs = 0;
  the_fs_info->c_file_name = c_file_name;
  the_fs_info->h_file_name = make_h_file_name(c_file_name);
  the_fs_info = partition_sub_dirs(the_fs_info, path);
  return the_fs_info;
}

/**
 * This function makes the .h file name from the .c file name.
 * @param  c_file_name - The .c file name from which to generate the .h file
 * name.
 * @return h_file_name - The .h file name.
 */
char * make_h_file_name(char * c_file_name) {
  // This mess just gets the index of the where file extension starts
  int file_wo_ext = (int)(strchr(c_file_name, '.') - c_file_name);
  size_t h_file_len = strnlen(c_file_name, TYPE_MAX_SIZE) + 1;
  char * h_file_name = calloc(h_file_len, sizeof(char));
  strncpy(h_file_name, c_file_name, file_wo_ext);
  strncat(h_file_name, H_EXTENSION, strnlen(H_EXTENSION, TYPE_MAX_SIZE));
  return h_file_name;
}

/**
 * This function contains will write each sub directory that needs to be written
 * (if any) and will store them in the 2d array member of the_fs_info
 * each_sub_dir. Additionally it will update qty_sub_dirs as needed.
 * Example:
 * some/path/to/file =>
 * each_sub_dir[0] = some
 * each_sub_dir[1] = some/path
 * each_sub_dir[2] = some/path/to
 * each_sub_dir[4] = some/path/to/file
 * each_sub_dir[5] = some/path/to/fileINCLUDE_STRING
 * @param         path - The path to write the 2d array with.
 * @param  the_fs_info - The fs_info to be written to.
 * @return the_fs_info - The fs_info struct with the correct values written to
 * it.
 */
fs_info * partition_sub_dirs(fs_info * the_fs_info, char * path) {
  size_t len_path = strnlen(path, TYPE_MAX_SIZE);
  size_t len_include = strnlen(INCLUDE_STRING, TYPE_MAX_SIZE);
  size_t len_total = len_path + len_include + 1;
  for(int i = 0; i < (int)len_path; i++) {
    if(path[i] == '/' || i == (int)(len_path - 1)) {
      the_fs_info->qty_sub_dirs++;
      if(!the_fs_info->each_sub_dir)
        the_fs_info->each_sub_dir = calloc(1, sizeof(char *));
      else
        the_fs_info->each_sub_dir = realloc(the_fs_info->each_sub_dir,
            the_fs_info->qty_sub_dirs * sizeof(char *));
      // The last letter gets chopped in the case that of i == len(path) - 1
      if(i == (int)(len_path - 1)) {
        the_fs_info->each_sub_dir[the_fs_info->qty_sub_dirs - 1] = calloc(i + 2,
            sizeof(char));
        strncpy(the_fs_info->each_sub_dir[the_fs_info->qty_sub_dirs - 1], path,
            i + 1);
      } else {
        the_fs_info->each_sub_dir[the_fs_info->qty_sub_dirs - 1] = calloc(i + 1,
            sizeof(char));
        strncpy(the_fs_info->each_sub_dir[the_fs_info->qty_sub_dirs - 1], path,
            i);
      }
    }
  }
  // This makes the path/include
  the_fs_info->qty_sub_dirs++;
  if(!the_fs_info->each_sub_dir)
    the_fs_info->each_sub_dir = calloc(1, sizeof(char *));
  else
    the_fs_info->each_sub_dir = realloc(the_fs_info->each_sub_dir,
        the_fs_info->qty_sub_dirs * sizeof(char *));
  the_fs_info->each_sub_dir[the_fs_info->qty_sub_dirs - 1] = calloc(len_total,
      sizeof(char));
  strncpy(the_fs_info->each_sub_dir[the_fs_info->qty_sub_dirs - 1], path,
      len_path);
  strncat(the_fs_info->each_sub_dir[the_fs_info->qty_sub_dirs - 1],
      INCLUDE_STRING, len_include);
  return the_fs_info;
}

/**
 * This function is responsible for generating the relative path in form of a
 * character pointer between src and dest.
 * @param      dest - The file which will be included.
 * @param       src - The file which is including dest.
 * @return rel_path - The path between src and dest.
 */
char * relative_path(fs_info * dest, fs_info * src) {
  int branch_index = -1;
  char * rel_path = NULL;
  size_t rel_path_len = 0;
  size_t snsp_path_len = 0;
  for(int i = 0; i < src->qty_sub_dirs; i++)
    if(strncmp(dest->each_sub_dir[i], src->each_sub_dir[i], TYPE_MAX_SIZE)) {
      branch_index = i;
      break;
    }
  if(branch_index == -1) {
    rel_path_len = strnlen(dest->h_file_name, TYPE_MAX_SIZE) + 1;
    rel_path = calloc(rel_path_len, sizeof(char));
    strncpy(rel_path, dest->h_file_name, rel_path_len - 1);
    return rel_path;
  } else {
    size_t branch_point_len = strnlen(src->each_sub_dir[branch_index - 1],
        TYPE_MAX_SIZE);
    snsp_path_len = strnlen(src->each_sub_dir[branch_index], TYPE_MAX_SIZE)
      - branch_point_len;
    rel_path_len = (branch_index * strnlen(UP_DIR, TYPE_MAX_SIZE))
      + (strnlen(src->each_sub_dir[src->qty_sub_dirs - 1], TYPE_MAX_SIZE)
          - snsp_path_len) + strnlen(src->h_file_name, TYPE_MAX_SIZE) + 2;
    rel_path = calloc(rel_path_len, sizeof(char));
    for(int i = 0; i < branch_index; i++)
      strncat(rel_path, UP_DIR, TYPE_MAX_SIZE);
    // Don't get the '/' from src
    strncat(rel_path, src->each_sub_dir[src->qty_sub_dirs - 1]
        + branch_point_len + 1, TYPE_MAX_SIZE);
    strncat(rel_path, "/", TYPE_MAX_SIZE);
    strncat(rel_path, src->h_file_name, TYPE_MAX_SIZE);
    return rel_path;
  }
}

/**
 * This function is used to debug an fs_info struct.
 * @param the_fs_info - The fs_info to be debugged.
 * @return        N/a
 */
void debug_fs_info(fs_info * the_fs_info) {
  printf("[FS_INFO]\n");
  printf("`%s`\n", the_fs_info->c_file_name);
  printf("`%s`\n", the_fs_info->h_file_name);
  printf("Sub Paths (%d):\n", the_fs_info->qty_sub_dirs);
  for(int i = 0; i < the_fs_info->qty_sub_dirs; i++)
    printf("`%s`\n", the_fs_info->each_sub_dir[i]);
}

/**
 * This function is used to free an fs_info data structure.
 * @param the_fs_info - The fs info struct to be freed.
 * @return        N/a
 */
void free_fs_info(fs_info * the_fs_info) {
  if(the_fs_info) {
    if(the_fs_info->each_sub_dir) {
      for(int i = 0; i < the_fs_info->qty_sub_dirs; i++)
        if(the_fs_info->each_sub_dir[i])
          free(the_fs_info->each_sub_dir[i]);
      free(the_fs_info->each_sub_dir);
    }
    if(the_fs_info->c_file_name)
      free(the_fs_info->c_file_name);
    if(the_fs_info->h_file_name)
      free(the_fs_info->h_file_name);
    free(the_fs_info);
  }
}

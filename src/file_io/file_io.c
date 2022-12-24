#define _POSIX_C_SOURCE 200809L // C99 Standard for strn* funcs, POSIX only
#include "include/file_io.h"

void write_symbol_table(symbol_table * the_st) {
  write_directories(the_st);
}

void write_directories(symbol_table * the_st) {
  struct stat dir_status = {0};
  char ** each_sub_dir = NULL;
  int qty_sub_dirs = 0;
  for(int i = 0; i < the_st->no_uds; i++) {
    qty_sub_dirs = partition_sub_dirs(the_st->all_uds[i]->path, &each_sub_dir);
    for(int j = 0; j < qty_sub_dirs; j++) {
      if(stat(each_sub_dir[j], &dir_status) == -1)
        if(mkdir(each_sub_dir[j], 0755) != 0)
          fprintf(stderr, "[WRITE_DIRECTORIES]: %s\n", strerror(errno));
    }
    if(each_sub_dir) {
      for(int j = 0; j < qty_sub_dirs; j++)
        if(each_sub_dir[j])
          free(each_sub_dir[j]);
      free(each_sub_dir);
      qty_sub_dirs = 0;
      each_sub_dir = NULL;
    }
  }
}

int partition_sub_dirs(char * path, char *** each_sub_dir) {
  int qty_sub_dirs = 0;
  size_t len_path = strnlen(path, TYPE_MAX_SIZE);
  for(int i = 0; i < (int)len_path; i++) {
    if(path[i] == '/' || i == (int)(len_path - 1)) {
      qty_sub_dirs++;
      if(!each_sub_dir[0])
        each_sub_dir[0] = calloc(1, sizeof(char *));
      else
        each_sub_dir[0] = realloc(each_sub_dir[0], qty_sub_dirs
            * sizeof(char *));
      // The last letter gets chopped in the case that of i == len(path) - 1
      if(i == (int)(len_path - 1)) {
        each_sub_dir[0][qty_sub_dirs - 1] = calloc(i + 2, sizeof(char));
        strncpy(each_sub_dir[0][qty_sub_dirs - 1], path, i + 1);
      } else {
        each_sub_dir[0][qty_sub_dirs - 1] = calloc(i + 1, sizeof(char));
        strncpy(each_sub_dir[0][qty_sub_dirs - 1], path, i);
      }
    }
  }
  // This makes the path/include
  qty_sub_dirs++;
  if(!each_sub_dir[0])
    each_sub_dir[0] = calloc(1, sizeof(char *));
  else
    each_sub_dir[0] = realloc(each_sub_dir[0], qty_sub_dirs * sizeof(char *));
  size_t len_include = strnlen(INCLUDE_STRING, TYPE_MAX_SIZE);
  size_t len_total = len_path + len_include + 1;
  each_sub_dir[0][qty_sub_dirs - 1] = calloc(len_total, sizeof(char));
  strncpy(each_sub_dir[0][qty_sub_dirs - 1], path, len_path);
  strncat(each_sub_dir[0][qty_sub_dirs - 1], INCLUDE_STRING, len_include);
  return qty_sub_dirs;
}

void write_struct_definition(symbol_table * the_st) {
  FILE * fp = NULL;
  for(int i = 0; i < the_st->no_uds; i++) {
    fp = fopen(the_st->all_uds[i]->h_file_name, "w");
    fclose(fp);
    fp = NULL;
  }
}

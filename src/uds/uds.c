#define _POSIX_C_SOURCE 200809L // C99 Standard for strn* funcs, POSIX only
#include "include/uds.h"

uds * init_uds(char * name, char * path, char * file_name,
    method_type * methods, int no_methods) {
  uds * the_uds = calloc(1, sizeof(struct UDS_T));
  the_uds->name = name;
  the_uds->path = path;
  the_uds->file_name = file_name;
  the_uds = make_header_file(the_uds);
  the_uds->methods = methods;
  the_uds->no_methods = no_methods;
  the_uds = make_req_libs(the_uds);
  the_uds->members = NULL;
  the_uds->no_members = 0;
  return the_uds;
}

uds * make_header_file(uds * the_uds) {
  // This mess just gets the index of the where file extension starts
  int file_wo_ext = (int)(strchr(the_uds->file_name, '.') - the_uds->file_name);
  size_t h_file_len = strnlen(the_uds->file_name, TYPE_MAX_SIZE) + 1;
  the_uds->h_file_name = calloc(h_file_len, sizeof(char));
  strncpy(the_uds->h_file_name, the_uds->file_name, file_wo_ext);
  strncat(the_uds->h_file_name, H_EXTENSION, strnlen(H_EXTENSION, TYPE_MAX_SIZE));
  return the_uds;
}

uds * make_req_libs(uds * the_uds) {
  for(int i = 0; i < the_uds->no_methods; i++)
    switch(the_uds->methods[i]) {
      case INIT:
      case DEEP_COPY:
      case FREE:
        the_uds = add_lib(the_uds, STDLIB);
        break;
      case DEBUG:
        the_uds = add_lib(the_uds, STDIO);
        break;
      default:
        fprintf(stderr, "[MAKE_REQ_LIBS]: MT_NULL passed?\nExiting\n");
        exit(1);
    }
  return the_uds;
}

uds * add_lib(uds * the_uds, libs the_lib) {
  for(int i = 0; i < the_uds->no_req_libs; i++)
    if(the_uds->req_libs[i] == the_lib)
      return the_uds;
  the_uds->no_req_libs++;
  if(!the_uds->req_libs)
    the_uds->req_libs = calloc(1, sizeof(libs));
  else
    the_uds->req_libs = realloc(the_uds->req_libs, the_uds->no_req_libs
        * sizeof(libs));
  the_uds->req_libs[the_uds->no_req_libs - 1] = the_lib;
  return the_uds;
}

uds * add_uds_member(uds * the_uds, uds_member * new_uds_member) {
  the_uds->no_members++;
  if(!the_uds->members)
    the_uds->members = calloc(1, sizeof(struct UDS_MEMBER_T *));
  else
    the_uds->members = realloc(the_uds->members, the_uds->no_members *
        sizeof(struct UDS_MEMBER_T *));
  the_uds->members[the_uds->no_members - 1] = new_uds_member;
  return the_uds;
}

void debug_uds(uds * the_uds) {
  printf("[USER_DEFINE_STRUCTURE]\n");
  printf("Path: %s/%s; Name: %s\n", the_uds->path, the_uds->file_name,
      the_uds->name);
  printf("Header Name: %s\n", the_uds->h_file_name);
  printf("No libs: %d\n", the_uds->no_req_libs);
  for(int i = 0; i < the_uds->no_req_libs; i++)
    printf("%s\n", libs_to_string(the_uds->req_libs[i]));
  printf("No Methods: %d\n", the_uds->no_methods);
  for(int i = 0; i < the_uds->no_methods; i++)
    printf("%s\n", method_type_to_string(the_uds->methods[i]));
  printf("No Members: %d\n", the_uds->no_members);
  for(int i = 0; i < the_uds->no_members; i++)
    debug_uds_member(the_uds->members[i]);
}

void free_uds(uds * the_uds) {
  if(the_uds) {
    if(the_uds->members) {
      for(int i = 0; i < the_uds->no_members; i++)
        if(the_uds->members[i])
          free_uds_member(the_uds->members[i]);
      free(the_uds->members);
    }
    if(the_uds->req_libs)
      free(the_uds->req_libs);
    if(the_uds->methods)
      free(the_uds->methods);
    if(the_uds->path)
      free(the_uds->path);
    if(the_uds->h_file_name)
      free(the_uds->h_file_name);
    if(the_uds->file_name)
      free(the_uds->file_name);
    if(the_uds->name)
      free(the_uds->name);
    free(the_uds);
  }
}

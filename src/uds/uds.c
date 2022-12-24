/**
 * @file   uds.c
 * @brief  This function contains the functions that relate to the user defined
 * structure.
 * @author Matthew C. Lindeman
 * @date   December 24, 2022
 * @bug    None known
 * @todo   Nothing
 */
#define _POSIX_C_SOURCE 200809L // C99 Standard for strn* funcs, POSIX only
#include "include/uds.h"

/**
 * This function initializes a uds with the given parameters but no members.
 * @param       name - The name of the uds.
 * @param       path - The path to file containing the uds's c file.
 * @param  file_name - The file name of the c file for the uds.
 * @param    methods - The default methods (which are generated via their type)
 * that the user specified for this particular uds.
 * @param no_methods - The number of methods that the user specified.
 * @return
 */
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

/**
 * This function makes the h_file_name member of the uds from the file_name (the
 * C file name).
 * @param  the_uds - The uds to write the member to.
 * @return the_uds - The uds with the h_file_name written to it.
 */
uds * make_header_file(uds * the_uds) {
  // This mess just gets the index of the where file extension starts
  int file_wo_ext = (int)(strchr(the_uds->file_name, '.') - the_uds->file_name);
  size_t h_file_len = strnlen(the_uds->file_name, TYPE_MAX_SIZE) + 1;
  the_uds->h_file_name = calloc(h_file_len, sizeof(char));
  strncpy(the_uds->h_file_name, the_uds->file_name, file_wo_ext);
  strncat(the_uds->h_file_name, H_EXTENSION, strnlen(H_EXTENSION, TYPE_MAX_SIZE));
  return the_uds;
}

/**
 * This function makes the required libraries for the uds given which default
 * methods the user specified.
 * @param  the_uds - The uds which will have the required libraries written to.
 * @return the_uds - The uds with the required libraries written to it.
 */
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

/**
 * This function adds a required library to the uds given.
 * @param  the_uds - The uds which will have the required library added to.
 * @return the_uds - The uds with the required library added to it.
 */
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

/**
 * This function is used to add a member to the uds.
 * @param        the_uds - The uds to add a member to.
 * @param new_uds_member - The uds_member to add the the uds.
 * @return       the_uds - The uds with the uds_member added to it.
 */
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

/**
 * This function debugs a particular uds.
 * @param the_uds - The uds to be debugged.
 * @return    N/a
 */
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

/**
 * This function frees a given uds.
 * @param the_uds - The uds to be freed.
 * @return    N/a
 */
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

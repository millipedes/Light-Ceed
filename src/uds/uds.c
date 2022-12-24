#include "include/uds.h"

uds * init_uds(char * name, char * path, method_type * methods,
    int no_methods) {
  uds * the_uds = calloc(1, sizeof(struct UDS_T));
  the_uds->name = name;
  the_uds->path = path;
  the_uds->methods = methods;
  the_uds->no_methods = no_methods;
  the_uds->members = NULL;
  the_uds->no_members = 0;
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
  printf("%s%s\n", the_uds->path, the_uds->name);
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
    free(the_uds);
  }
}

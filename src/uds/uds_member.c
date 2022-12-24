#include "include/uds_member.h"

uds_member * init_uds_member(data_type * type, char * name, char * flags) {
  uds_member * the_uds_member = calloc(1, sizeof(struct UDS_MEMBER_T));
  the_uds_member->type = type;
  the_uds_member->name = name;
  the_uds_member->flags = flags;
  return the_uds_member;
}

int has_init_flag(uds_member * the_uds_member) {
  if(the_uds_member->flags)
    for(int i = 0; the_uds_member->flags[i]; i++)
      if(the_uds_member->flags[i] == 'i')
        return 1;
  return 0;
}

int has_qty_flag(uds_member * the_uds_member) {
  if(the_uds_member->flags)
    for(int i = 0; the_uds_member->flags[i]; i++)
      if(the_uds_member->flags[i] == 'q')
        return 1;
  return 0;
}

void debug_uds_member(uds_member * the_uds_member) {
  printf("[UDS_MEMBER]\n");
  printf("Name: %s\n", the_uds_member->name);
  printf("Flags: %s\n", the_uds_member->flags);
  debug_data_type(the_uds_member->type);
}

void free_uds_member(uds_member * the_uds_member) {
  if(the_uds_member) {
    if(the_uds_member->type)
      free_data_type(the_uds_member->type);
    if(the_uds_member->name)
      free(the_uds_member->name);
    if(the_uds_member->flags)
      free(the_uds_member->flags);
    free(the_uds_member);
  }
}

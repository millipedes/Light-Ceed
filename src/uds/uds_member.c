#include "include/uds_member.h"

uds_member * init_uds_member(data_type * type, char * name) {
  uds_member * the_uds_member = calloc(1, sizeof(struct UDS_MEMBER_T));
  the_uds_member->type = type;
  the_uds_member->name = name;
  return the_uds_member;
}

void debug_uds_member(uds_member * the_uds_member) {
  printf("[UDS_MEMBER]\n");
  printf("Name: %s\n", the_uds_member->name);
  debug_data_type(the_uds_member->type);
}

void free_uds_member(uds_member * the_uds_member) {
  if(the_uds_member) {
    if(the_uds_member->type)
      free_data_type(the_uds_member->type);
    if(the_uds_member->name)
      free(the_uds_member->name);
    free(the_uds_member);
  }
}

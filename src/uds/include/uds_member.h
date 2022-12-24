#ifndef UDSM_H
#define UDSM_H

#include "data_type.h"

typedef struct UDS_MEMBER_T {
  data_type * type;
  char * name;
} uds_member;

uds_member * init_uds_member(data_type * type, char * name);
void debug_uds_member(uds_member * the_uds_member);
void free_uds_member(uds_member * the_uds_member);

#endif

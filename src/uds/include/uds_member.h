/**
 * @file   uds_member.h
 * @brief  This file contains the function definitions for uds_member.c
 * @author Matthew C. Lindeman
 * @date   
 * @bug    None known
 * @todo   Nothing
 */
#ifndef UDSM_H
#define UDSM_H

#include "data_type.h"

typedef struct UDS_MEMBER_T {
  data_type * type;
  char * name;
  char * flags;
} uds_member;

uds_member * init_uds_member(data_type * type, char * name, char * flags);
int has_init_flag(uds_member * the_uds_member);
int has_qty_flag(uds_member * the_uds_member);
void debug_uds_member(uds_member * the_uds_member);
void free_uds_member(uds_member * the_uds_member);

#endif

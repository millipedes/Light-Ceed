/**
 * @file   uds_member.c
 * @brief  This file contains the functions for tthe data structure
 * representative of a member of a user defined data structure.
 * @author Matthew C. Lindeman
 * @date   
 * @bug    None known
 * @todo   Nothing
 */
#include "include/uds_member.h"

/**
 * This function initializes a user defined structure member to have attributes
 * given.
 * @param            type - The type that the member is.
 * @param            name - The name of the member (note a dynamically allocated
 * char * should be passed here).
 * @param           flags - The flads of the member (note a dynamically
 * allocated char * should be passed here).
 * @return the_uds_member - The new uds_member.
 */
uds_member * init_uds_member(data_type * type, char * name, char * flags) {
  uds_member * the_uds_member = calloc(1, sizeof(struct UDS_MEMBER_T));
  the_uds_member->type = type;
  the_uds_member->name = name;
  the_uds_member->flags = flags;
  return the_uds_member;
}

/**
 * This function determines if the member has the init flag (i.e. is it passed
 * as a parameter to the init function for the uds).
 * @param the_uds_member - The uds_member to check for the flag.
 * @return             1 - Has flag.
 *                     0 - Does not have flag.
 */
int has_init_flag(uds_member * the_uds_member) {
  if(the_uds_member->flags)
    for(int i = 0; the_uds_member->flags[i]; i++)
      if(the_uds_member->flags[i] == 'i')
        return 1;
  return 0;
}

/**
 * This function determines if the member has the qty flag (i.e. Does this
 * member require a qty variable to be associated with it).
 * @param the_uds_member - The uds_member to check for the flag.
 * @return             1 - Has flag.
 *                     0 - Does not have flag.
 */
int has_qty_flag(uds_member * the_uds_member) {
  if(the_uds_member->flags)
    for(int i = 0; the_uds_member->flags[i]; i++)
      if(the_uds_member->flags[i] == 'q')
        return 1;
  return 0;
}

/**
 * This function is used to debug the uds_member.
 * @param the_uds_member - The uds_member to be debugged.
 * @return           N/a
 */
void debug_uds_member(uds_member * the_uds_member) {
  printf("[UDS_MEMBER]\n");
  printf("Name: %s\n", the_uds_member->name);
  printf("Flags: %s\n", the_uds_member->flags);
  debug_data_type(the_uds_member->type);
}

/**
 * This function frees a given uds_member.
 * @param the_uds_member - The uds_member to be freed.
 * @return           N/a
 */
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

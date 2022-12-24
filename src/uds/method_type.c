/**
 * @file   method_type.c
 * @brief  This file contains the functions relating to the method_type
 * enumeration.
 * @author Matthew C. Lindeman
 * @date   December 24, 2022
 * @bug    None known
 * @todo   Nothing
 */
#include "include/method_type.h"

/**
 * This function maps a method_type to a string.
 * @param the_mt - The method type that will be mapped.
 * @return    .\ - The corresponding string.
 */
const char * method_type_to_string(method_type the_mt) {
  switch(the_mt) {
    case INIT:      return "init";
    case DEBUG:     return "debug";
    case DEEP_COPY: return "deep_copy";
    case FREE:      return "free";
    case MT_NULL:   return "mt_null";
  }
  return NULL;
}

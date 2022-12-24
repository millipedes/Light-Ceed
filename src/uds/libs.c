/**
 * @file   libs.c
 * @brief  This file contains the functions relating to the libs enum.
 * @author Matthew C. Lindeman
 * @date   December 24, 2022
 * @bug    None known
 * @todo   Implement some custom lib function.
 */
#include "include/libs.h"

/**
 * This function takes a given libs and translates it to a string
 * representation.
 * @param the_lib - The lib to translate.
 * @return     .\ - The string rep. of the_lib.
 */
const char * libs_to_string(libs the_lib) {
  switch(the_lib) {
    case STDIO:  return "<stdio.h>";
    case STDLIB: return "<stdlib.h>";
    case STRING: return "<string.h>";
  }
  return NULL;
}

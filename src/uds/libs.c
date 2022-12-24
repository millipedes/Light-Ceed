#include "include/libs.h"

const char * libs_to_string(libs the_lib) {
  switch(the_lib) {
    case STDIO:  return "<stdio.h>";
    case STDLIB: return "<stdlib.h>";
    case STRING: return "<string.h>";
  }
  return NULL;
}

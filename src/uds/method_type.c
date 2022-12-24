#include "include/method_type.h"

const char * method_type_to_string(method_type the_mt) {
  switch(the_mt) {
    case INIT:      return "init";
    case DEBUG:     return "debug";
    case ADD:       return "add";
    case DEEP_COPY: return "deep_Copy";
    case FREE:      return "free";
    case MT_NULL:   return "mt_null";
  }
  return NULL;
}

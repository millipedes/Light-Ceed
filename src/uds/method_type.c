#include "include/method_type.h"

const char * method_type_to_string(method_type the_mt) {
  switch(the_mt) {
    case INIT:      return "Init";
    case DEBUG:     return "Debug";
    case ADD:       return "Add";
    case DEEP_COPY: return "Deep Copy";
    case FREE:      return "Free";
  }
  return NULL;
}

#ifndef EEPMEM_H
#define EEPMEM_H


#include <stdlib.h>

#define extend(ptr, num, type, extnum) \
  (type *) realloc((ptr), ((size_t) (num) + (size_t) (extnum)) * sizeof(type))



#endif

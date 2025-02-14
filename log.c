#include "log.h"

#include <stdio.h>

void info(Logger l, const char *name, const char *str, ...) {
  if (l.level >= INFO) {
    printf("[%s] ", name);
    printf(str);
    printf("\n");
  }
}

#include "log.h"

#include <stdio.h>

#include "types.h"

void info(Logger l, const byte *name, const byte *str, ...) {
    if (l.level >= INFO) {
        printf("[%s] ", name);
        printf(str);
        printf("\n");
    }
}

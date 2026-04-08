#include <stdio.h>

#include "log.h"
#include "types.h"

void info(Logger l, const byte *name, const byte *str, ...) {
    if (l.level >= INFO) {
        printf("[%s] ", name);
        printf("%s", str);
        printf("\n");
    }
}

#include <stdio.h>

#include "../include/log.h"
#include "../include/types.h"

void info(Logger l, const byte *name, const byte *str, ...) {
    if (l.level >= INFO) {
        printf("[%s] ", name);
        printf("%s", str);
        printf("\n");
    }
}

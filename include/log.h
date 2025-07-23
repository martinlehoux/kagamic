
#include "types.h"

typedef enum { INFO } LogLevel;

typedef struct {
    LogLevel level;
} Logger;

void info(Logger l, const byte *name, const byte *str, ...);

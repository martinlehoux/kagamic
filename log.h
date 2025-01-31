typedef enum { INFO } LogLevel;

typedef struct {
  LogLevel level;
} Logger;

void info(Logger l, const char *name, const char *str, ...);

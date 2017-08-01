#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include "debug.h"

void die(int t, const char *msg, ...) {
  char message[BUFSIZ];
  va_list args;
  va_start(args, msg);
  memset(message, 0, sizeof message);
  if(t)
    snprintf(message, sizeof message, "[ERROR] : %s\n", msg);
  else
    snprintf(message, sizeof message, "[INFO]  : %s\n", msg);
  vfprintf(stderr, message, args);
  va_end(args);
  exit(EXIT_FAILURE);
}

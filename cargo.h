#ifndef CARGO_H
#define CARGO_H

#include <stdbool.h>

struct Car;  // forward declaration

/* Helpers */
char *format (const char *a);

/* Case-insensitive comparison */
bool cargo_equal(const char *a, const char *b);

/* Operations */
void cargo_set(struct Car *c, const char *name, int qty, bool *ok);
void cargo_add(struct Car *c, const char *name, int qty, bool *ok);
void cargo_remove(struct Car *c, const char *name, int qty, bool *ok);

#endif


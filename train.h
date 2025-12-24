#ifndef TRAIN_H
#define TRAIN_H

#include <stdbool.h>

typedef struct Cargo {
    char *good_name;
    unsigned int quantity;
} Cargo;

typedef struct Car {
    int id;
    char *label;
    Cargo cargo;
    struct Car *next;
} Car;

typedef struct Train {
    Car *head;
    int size;
} Train;

/*Car Helpers */
Car *car_init(const char *, Car *c);
void car_destroy(Car *c);

/* Lifecycle */
void train_init(Train *t);
void train_destroy(Train *t);

/* Lookups */
Car *train_get(Train *t, int index);
bool train_label_exists(Train *t, const char *label);

/* Insertions */
bool train_append(Train *t, const char *label, int *out_index, int *out_id);
bool train_insert(Train *t, int index, const char *label, int *out_id);

/* Deletions */
bool train_delete(Train *t, int index, int *out_id, char **out_label);

/* Printing */
void train_print(const Train *t);

/* Searching */
void train_search(const Train *t, const char *good);

#endif


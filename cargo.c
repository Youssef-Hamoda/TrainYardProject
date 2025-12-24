#include "cargo.h"
#include "train.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ------------------ Helpers ------------------ */

char *format (const char *a){ 			//like tolower
	
	char *b = (char *)calloc(strlen(a) + 1, sizeof(char));
	if (b == NULL) return NULL;
	
	int i = 0;
	while (a[i] != '\0'){

		if (a[i] <= 'Z' && a[i] >= 'A') b[i] = a[i] + 32; 
		else if ((a[i] >= 'a' && a[i] <= 'z') || a[i] == ' ') b[i] = a[i];
		else {
			fprintf(stderr, "Invalid input to format: %c is non-alphabetic\n", a[i]);
			break;
		}

		i++;
	}

	b[0] = (i > 0 && b[0] >= 'a' && b[0] <= 'z') ? b[0] - 32 : b[0];	//capitalize first letter (preference)

	return b;
}

bool cargo_equal(const char *a, const char *b) {
    
    
    if (a == NULL || b == NULL) {

	    //DEBUG
	    //fprintf(stderr, "Invalid input to cargo_equal: NULL input\n");
	    return false;
    }	    
	
    char *c = format(a);
    char *d = format(b);
    bool out;

    if (c == NULL || d == NULL) {
	    fprintf(stderr, "Allocation Error in cargo_equal\n");
	    return false;
    }

    if (strcmp(c,d) != 0) out = false;
    else out = true;
	    
    free(c);
    free(d);
    return out;
    
}

void cargo_set(struct Car *c, const char *name, int qty, bool *ok) {
    
    if (c == NULL || name == NULL){
	    fprintf(stderr, "Invalid input to cargo_set\n");
	    *ok = false;
	    return;
    } else if (qty < 0) {
	    fprintf(stderr, "Invalid input to cargo_set: Invalid quantity \"%i\"\n", qty);
	    *ok = false;
	    return;
    }

    Cargo *g = &c->cargo;
    char *d = format(name);

    if (g->good_name != NULL){
	   
	    printf("%s replaced with %s on Car labeled %s:\n", g->good_name, d, c->label); 
       	    printf("ID: %i  Quantity: %i\n", c->id, qty);

	    free(g->good_name);
	 
    } else {
	    printf("%s added to Car labeled %s:\n", d, c->label);
	    printf("ID: %i  Quantity: %i\n", c->id, qty);
    }

    g->good_name = d;
    g->quantity = qty;

    *ok = true;
}

void cargo_add(struct Car *c, const char *name, int qty, bool *ok) {

    if (c == NULL || name == NULL){
	   
	   fprintf(stderr, "Invalid input to cargo_add\n");
	   *ok = false;
	   return;

    } else if (qty < 0) {
	   
	   fprintf(stderr, "Invalid input to cargo_add: Invalid quantity \"%i\"\n", qty);
	   *ok = false;
	   return;

    } else if (c->cargo.good_name != NULL && !cargo_equal(c->cargo.good_name, name)){
	   
	   fprintf(stderr, "Invalid input to cargo_add: Car %s has good %s\n", c->label, c->cargo.good_name);
	   *ok = false;
	   return;
    }

    char *d = format(name);
    
    if (c->cargo.good_name == NULL){
	    
	   c->cargo.good_name = d;
	   c->cargo.quantity = qty;
	    
	   printf("%s added to Car labeled %s:\n", d, c->label);
	   printf( "ID: %i  Quantity: %i\n", c->id, qty);
	   
	   *ok = true;
	   return;

    }

    c->cargo.quantity += qty;
    
    printf("%i added to Car labeled %s:\n", qty, c->label);
    printf("ID: %i  Good: %s  Quantity: %i\n", c->id, d, c->cargo.quantity);
    
    *ok = true;
    
}

void cargo_remove(struct Car *c, const char *name, int qty, bool *ok) {
    
    if (c == NULL || name == NULL){
	   
	   fprintf(stderr, "Invalid input to cargo_remove\n");
	   *ok = false;
	   return;

    } else if (qty < 0) {
	   
	   fprintf(stderr, "Invalid input to cargo_remove: Invalid quantity \"%i\"\n", qty);
	   *ok = false;
	   return;

    } else if (c->cargo.good_name != NULL && !cargo_equal(c->cargo.good_name, name)){
	   
	   fprintf(stderr, "Invalid input to cargo_remove. Details:\n");
	   fprintf(stderr, "Car labeled %s has good %s\n", c->label, c->cargo.good_name);
	   *ok = false;
	   return;

    } else if ((int)c->cargo.quantity < qty){
	    
	   fprintf(stderr,"Invalid input to cargo_remove.\n");
	   fprintf(stderr,"Car labeled %s has only %u of good %s.\n", c->label, c->cargo.quantity, c->cargo.good_name);   
	   fprintf(stderr,"ID: %i  Quantity Requested: %i\n", c->id, qty);
	   
	   *ok = false;
	   return;
    }

    c->cargo.quantity -= qty;

    if (c->cargo.quantity == 0) {
	    free(c->cargo.good_name);
	    c->cargo.good_name = NULL;
    }
    
    printf("%i removed from Car labeled %s. Details:\n", qty, c->label);
    printf("ID: %i  Good: %s  Quantity: %i\n", c->id, c->cargo.good_name, c->cargo.quantity);
    
    *ok = true;
}


#include "train.h"
#include "cargo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int next_id = 1000;


/*-------------------Car Helpers ------------------*/

char *my_strdup(const char *s) {
    
	if (s == NULL) return NULL;

    	size_t len = strlen(s) + 1;
    	char *new = malloc(len);

    	if (new == NULL) return NULL;

    	memcpy(new, s, len);
    	return new;
}

Car *car_init(const char *label, Car *next) {	//INIT CAR & NULL CARGO

	Car *myCar = (Car *)malloc(sizeof(Car));
	
	if (myCar == NULL) {
		fprintf(stderr, "car_init failed, try again\n");
		return NULL;
	}


	Cargo c = {.good_name = NULL, .quantity = 0};

	myCar->id = next_id;
	myCar->label = my_strdup(label);
	myCar->next = next;
	myCar->cargo = c;

	next_id++;

	if (myCar->label == NULL || strcmp(label, myCar->label) != 0){
		fprintf(stderr, "car_init due to NULL label or strdup failure. Try again");
		free(myCar);

		return NULL;
	}

	//DEBUG
	//printf("New Car created\n");

	return myCar;
}

void car_destroy(Car *c) { 						//FREE CAR & CARGO (NULL CHECK INCL)
	if (c==NULL) return;

	free(c->label);
	if(c->cargo.good_name != NULL) free(c->cargo.good_name);
	free(c);
}

/* ------------------ Lifecycle ------------------ */

void train_init(Train *t) {
	
    t->head = NULL;
    t->size = 0;

    //DEBUG
    //printf("Train Initialized\n");
}

void train_destroy(Train *t) {
    
    Car *next, *temp;

    next = t->head;

    while (next != NULL) {
    	temp = next;
	next = next->next;
	car_destroy(temp);
    }

}

/* ------------------ Lookups ------------------ */

Car *train_get(Train *t, int index) {
    
    if (t == NULL || index >= t->size || index < 0) {
	    fprintf(stderr, "Input error to train_get\n");
	    return NULL;
    }
    
    Car *c = t->head; //init cursor to head
    for (int i = 0; i < index; i++){
    	c = c->next;
    }

    return c;
}

bool train_label_exists(Train *t, const char *label) {
    
	if (t == NULL) {
		fprintf(stderr, "Input error to train_label_exists\n");
		return false;
	}

	Car *c = t->head; //init cursor to head
	int i = 0;

	while (c != NULL){
		if (strcmp(label, c->label) == 0){ 	//case sensitive cmp with selected car
			printf("%s found at index %i\n", label, i);       
			return true;
		}

		c = c->next;     			//increment cursor  
		i++;					//increment index
	
	}

	return false;

}

/* ------------------ Insertions ------------------ */

bool train_append(Train *t, const char *label, int *out_index, int *out_id) {
    if (t == NULL || label == NULL){

	    fprintf(stderr, "Input error to train_append\n");
	    return false;
    
    } else if (train_label_exists(t, label)){
	    fprintf(stderr, "Input error to train_append: label already exists\n");
	    return false;
    }

    Car *new = car_init(label, NULL);

    if (new == NULL){

	    fprintf(stderr, "car_init failed. Try again.\n");
	    return false;
    }

    if (t->size == 0){

	    t->head = new;
	    
	    *out_index = t->size;	    
	    *out_id = t->head->id;

    } else {
	    
	    Car *c = train_get(t, t->size - 1);
	    
	    c->next = new;
	    
	    *out_index = t->size;	    
	    *out_id = c->next->id;
    
    }

    t->size++;

    printf("Car labeled %s added at index %i. Details:\nID:%i\n", new->label, *out_index, *out_id);
    
    return true;

}

bool train_insert(Train *t, int index, const char *label, int *out_id) {
    
    // Error handling & edge cases

    if (t == NULL || label == NULL){
	    
	    fprintf(stderr, "Input error to train_insert\n");
	    return false;
    
    } else if (index < 0 || index > t->size){

	    fprintf(stderr, "Input error to train_insert: Invalid index %i\n", index);
	    return false;

    } else if (index == t->size) {				//insert at size || empty train insert
	  
	    return train_append(t, label, &index, out_id);
    
    } else if (train_label_exists(t, label)){
	    
	    fprintf(stderr, "Input error to train_insert: label already exists\n");
	    return false;

    } else if (index == 0) {                                    //new head, nonempty train

            Car *temp = t->head;                                //save current head
            t->head = car_init(label, temp);	    	        //replace head

	    if (t->head == NULL){

		    fprintf(stderr, "car_init failed. Try again.\n");
		    t->head = temp;
		    return false;
	    }

            *out_id = t->head->id;                              //set output

            t->size ++;                                         //increment size

            printf("Car labeled %s added at head. Details:\n", label);
	    printf("ID:%i\n", *out_id);
            printf("Car labeled %s moved to index %i\n", t->head->next->label, index + 1);

	    return true;
    }
			 
    Car *c = train_get(t, index - 1);

    Car *temp, *new;
    
    temp = c->next;			//save car at index
    new = car_init(label, temp);	//init new car with next as saved car
					
    if (new == NULL) {

	    fprintf(stderr, "car_init failed. Try again.\n");
	    return false;
    }

    c->next = new;		        //set new car as next
 
    t->size++;

    *out_id = new->id;

    printf("Car labeled %s added at index %i. Details:\n", label, index); 
    printf("ID:%i\n", *out_id);
    printf("Car labeled %s moved to %i\n", new->next->label, index + 1); 

    return true;
}

/* ------------------ Deletions ------------------ */

bool train_delete(Train *t, int index, int *out_id, char **out_label) {

    if (t == NULL){		

	    fprintf(stderr, "Input error to train_delete: NULL train\n");
   	    return false;

    } else if (index < 0 || index >= t->size){			//invalid index

	    fprintf(stderr, "Input error to train_delete: Invalid index %i\n", index);
	    fprintf(stderr, "Details:  Train Size: %i\n", t->size);
	    return false;

    } else if (index == 0) {					//remove head, nonempty train
	    
	    Car *temp = t->head;				//save current head
	    t->head = (t->size != 1) ? t->head->next : NULL;	//replace head, if possible
	    
	    *out_id = temp->id;					//set outputs
	    *out_label = temp->label;				

    	    printf("Car labeled %s removed from head. Details:\n", *out_label); 
    	    printf("ID:%i\n", *out_id);
	    
	    if (t->head != NULL) 
		    printf("Car labeled %s moved to head\n", t->head->label);
	    
	    t->size --;						//increment size
	    car_destroy(temp);					//destroy car

	    return true; 
    }

    
    Car *c = train_get(t, index - 1);

    Car *temp = c->next;			  			//save car at index
    c->next = (temp->next != NULL) ? temp->next : NULL;      		//fix pointers to ignore deleted
 
    *out_id = temp->id;							//set outputs
    *out_label = temp->label;

    printf("Car labeled %s removed from index %i. Details:\n", *out_label, index); 
    printf("ID:%i\n", *out_id);
    
    if (c->next != NULL) {

	    printf("Car %s moved to index %i\n", c->next->label, index);
	    car_destroy(temp);	   			//destroy car
    }

    t->size--;

    return true;

}

/* ------------------ Printing ------------------ */

void train_print(const Train *t) {
    if (t == NULL || t->size == 0){
	    printf("Train is empty, or NULL\n");
	    return;
    }

    int i = 1;
    Car *c = t->head;
    printf("Train Size: %i\n", t->size);

    do{
	printf("Car %i: Label = \"%s\"  ID = %i", i, c->label, c->id);
	
	if (c->cargo.good_name == NULL) printf("  Cargo = Empty\n");
	else printf("  Cargo = \"%s\"  Quantity = %i\n", c->cargo.good_name, c->cargo.quantity);

	c = c->next;
	i++;

    } while(c != NULL);
    
}

/* ------------------ Searching ------------------ */

void train_search(const Train *t, const char *good) {
    
    if (t == NULL || good == NULL){
	fprintf(stderr, "Invalid input to train_search\n");
	return;
    }

    printf("Searching for: \"%s\"\n", good);

    Car *c = t->head;
    
    int i = 0;
    unsigned int total = 0;

    while (c != NULL) {
	
	if (cargo_equal(good, c->cargo.good_name)){

		printf("Found on car %i:  Label = \"%s\"  ID = %i  Quantity = %i\n"
				, i, c->label, c->id, c->cargo.quantity);
		total += c->cargo.quantity;
	}

	i++;
	c = c->next;

    }

    printf("TOTAL %s: %u\n", good, total);


}


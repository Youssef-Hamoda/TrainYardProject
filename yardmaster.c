#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "train.h"
#include "cargo.h"

const char *n = "-----------------------------------------";

void print_help() {
    printf("Commands:\n");
    printf("  append \"LABEL\"\n");
    printf("  insert INDEX \"LABEL\"\n");
    printf("  delete INDEX\n");
    printf("  print\n");
    printf("  search \"GOOD\"\n");
    printf("  goods add INDEX \"GOOD\" QTY\n");
    printf("  goods set INDEX \"GOOD\" QTY\n");
    printf("  goods remove INDEX \"GOOD\" QTY\n");
    printf("  help\n");
    printf("  exit\n");
}

int integerize (char *arg) {	//strtol abstraction for index && qty
	
	char *end;
	int result;

	result = (int)strtol(arg, &end, 10);

	if (arg == end || *end != '\0' || result  < 0)
		return -1;

	return result;

}

//set val[i] to start of each word
//if quoted, ignore quotes & null terminate
//if not, parse until space and null terminate
int parser (char *line, char *vals[], int max_vals){
	
	char *p = line;
	int count = 0;

	while (*p && count < max_vals){

		//skip leading white space
		while (*p == ' ' || *p == '\t') p++;

		if (*p == '"' || *p == '\''){
		
			char quote = *p;
			p++;
			vals[count] = p;

			//read till next quote
			while (*p && *p != quote) p++;
			
			//check *p isnt out of bounds &% is quote
			//otherwise, rest of line is token
			if (*p == quote) {
				*p = '\0';
				p++;
			}

		} else {

		       vals[count] = p;

		       //while not whitespace
		       while (*p && *p != ' ' && *p != '\t') p++;

		       if (*p){
			       *p = '\0';
			       p++;
		       }
		}

		count++;
	}
	
	return count;
}

int main() {
    Train train;
    train_init(&train);

    char line[512];

    printf("YardMaster ready.\n");
    print_help();

    int out_index, out_id;
    char *out_label;
    bool ok;
    int tests;

    
    while (1) {
        printf("> ");

        if (!fgets(line, sizeof(line), stdin))
            break;
	        
	//rip newline
	size_t len = strlen(line); 
        if (len > 0 && line[len - 1] == '\n')
            line[len - 1] = '\0';

        if (strncmp(line, "exit", 4) == 0)
	    break;

    	char *vals[5];    //longest command has 5 args 
   
	int count = parser(line, vals, 5);

	//DEBUG
	//for (int i = 0; i < count; i++) {
	//	printf("vals[%i] = %s\n", i, vals[i]);
	//}

	if (count == 0)
		continue;
	
	if (strcmp(vals[0], "test:") == 0){ //ignore keyword test (used in test.txt)
		
		tests++;

		printf("\n%s\nTest %i: ", n, tests);
		for (int i = 1; i < count; i ++)
			printf("%s ", vals[i]);
		printf("\n%s\n\n", n);

		continue;
	}

	vals[0] = format(vals[0]); //allow for case insensitive command usage

	if (strcmp(vals[0], "Print") == 0 && count == 1){
		
		train_print(&train);

	} else if (strcmp(vals[0], "Append") == 0 && count == 2) {
	       
		train_append(&train, vals[1], &out_index, &out_id); 

	} else if (strcmp(vals[0], "Insert") == 0 && count == 3){

		int indx = integerize(vals[1]);
                
		if (indx < 0) {
                        fprintf(stderr, "Index failed to parse, or is invalid\n");
                        
			free(vals[0]);
			continue;
                }

		train_insert(&train, indx, vals[2], &out_id);

        } else if (strcmp(vals[0], "Delete") == 0 && count == 2){
                
		int indx = integerize(vals[1]);

		if (indx < 0) {
                        fprintf(stderr, "Index failed to parse, or is invalid\n");
                        
			free(vals[0]);
			continue;
                }

		train_delete(&train, indx, &out_id, &out_label);
 
        } else if (strcmp(vals[0], "Search") == 0 && count == 2){
               
		train_search(&train, vals[1]);

        } else if (strcmp(vals[0], "Goods") == 0 && count == 5){
              
		int indx, qty;
	       
		indx = integerize(vals[2]);
		qty = integerize(vals[4]);

		if (indx < 0) {

			fprintf(stderr, "Index failed to parse, or is invalid\n");
			
			free(vals[0]);
			continue;

		} else if (qty < 0) {
			
			fprintf(stderr, "Quantity failed to parse or is invalid\n");
			
			free(vals[0]);
			continue;
		}

		Car *c = train_get(&train, indx);

		if (c == NULL) {

			free(vals[0]);
			continue;
		}

		if (strcmp(vals[1], "add") == 0){
                
			cargo_add(c, vals[3], qty, &ok); 

        	} else if (strcmp(vals[1], "set") == 0){
                
			cargo_set(c, vals[3], qty, &ok);

        	} else if (strcmp(vals[1], "remove") == 0){
                
			cargo_remove(c, vals[3], qty, &ok);
        	} else
		      fprintf(stderr, "Invalid command %s. Try help\n", vals[1]);	

		//DEBUG: Check ok

        } else if (strcmp(vals[0], "Help") == 0){
                print_help();

	} else 
		fprintf(stderr, "Invalid command or invalid usage of command %s. Try help\n", vals[0]);
	
	free(vals[0]);

    }

    train_destroy(&train);
    printf("\nTests Executed: %i\n", tests); 
    return 0;
}


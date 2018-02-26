#ifndef EEP_VAR_STRING_H
#define EEP_VAR_STRING_H

struct var_string_s
{
	char* s;
	long num_alloc;
	long length;
};

typedef struct var_string_s* var_string;

/* Create a new instance of simple_string.
   The string will be empty */
var_string varstr_construct();

/* Delete a string instance */
void varstr_destruct(var_string);

/* Append a string to the existing string */
int varstr_append(var_string, const char*);

/* Set the string to this value */

int varstr_set(var_string, const char*);

/* Get the 'char*' representation of this string.
   NEVER free() the result! (use varstr_destruct instead) */
const char* varstr_cstr(var_string);

/* Returns the current length of the string */
long varstr_length(var_string);
#endif
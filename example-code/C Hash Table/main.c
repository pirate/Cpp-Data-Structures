#include "hashtable.h"

#include <stdio.h>
#include <string.h>

void do_tbl_print(hashtable_t *tbl, void *key)
{
	void *value;
	
	if (!hashtable_get(tbl, key, &value) || value == NULL)
		value = "NULL";

	printf("%s: %s\n", key, value);
}

int main(int argc, char **argv)
{
	hashtable_t tbl;

	// init
	hashtable_init(&tbl, hashtable_hash_fnv, (hashtable_len_function)strlen);
	
	// populate
	hashtable_set(&tbl, "fulano", "32");
	hashtable_set(&tbl, "ciclano", "64");
	hashtable_set(&tbl, "derp", "herp");
	hashtable_set(&tbl, "herp", "derp");

	// retrieve pass 1
	printf("Pass 1:\n");
	do_tbl_print(&tbl, "fulano");
	do_tbl_print(&tbl, "ciclano");
	do_tbl_print(&tbl, "derp");
	do_tbl_print(&tbl, "herp");
	printf("\n");

	// remove some
	hashtable_remove(&tbl, "fulano");

	// retrive pass 2
	printf("Pass 2:\n");
	do_tbl_print(&tbl, "fulano");
	do_tbl_print(&tbl, "ciclano");
	do_tbl_print(&tbl, "derp");
	do_tbl_print(&tbl, "herp");
	printf("\n");

	// remove some
	hashtable_remove(&tbl, "herp");

	// retrive pass 3
	printf("Pass 3:\n");
	do_tbl_print(&tbl, "fulano");
	do_tbl_print(&tbl, "ciclano");
	do_tbl_print(&tbl, "derp");
	do_tbl_print(&tbl, "herp");
	printf("\n");
	
	// clear
	hashtable_clear(&tbl);
	
	// retrive pass 4
	printf("Pass 4:\n");
	do_tbl_print(&tbl, "fulano");
	do_tbl_print(&tbl, "ciclano");
	do_tbl_print(&tbl, "derp");
	do_tbl_print(&tbl, "herp");
	printf("\n");

	// destroy
	hashtable_destroy(&tbl);

	getchar();

	return 0;
}

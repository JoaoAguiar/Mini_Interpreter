#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "HashTable.h"

// Inserir um valor com uma key
void insert(char* elementName, int elementValue) {
	HashList list = lookup(elementName);

	if(list == NULL) {
		table[hash(elementName)] = mkHashList(elementValue, elementName, table[hash(elementName)]);
	}
	else {
		list->element = elementValue;
	}
}

// Pesquisar key numa HashTable
HashList lookup(char* elementName) {
	int index = hash(elementName);
	HashList list = table[index];

	while(list != NULL) {
		if(strcmp(list->key, elementName) == 0) {
			return list;
		}

		list = list->next;
	}

	return NULL;
}

int hash(char* elementName) {
	int value = 0;
	int i;

	for(i=0; i<strlen(elementName); i++) {
		value += (int)elementName[i];
	}

	return value;
}

// Cria uma nova HashList
HashList mkHashList(int elementValue, char* elementName, HashList list) {
	// Reserva um bloco de memoria com um numero especifico de bytes
	HashList newList = (HashList) malloc(sizeof(struct list));

	newList-> element = elementValue;
	newList-> key = elementName;
	newList-> next = list;

	return newList;
}
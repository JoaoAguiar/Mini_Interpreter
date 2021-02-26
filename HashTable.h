#define HASH_SIZE 999
#define MULTI 31

typedef struct list {
	char* key;
	int element;
	struct list* nextElement;
}* HashList;

HashList table[HASH_SIZE];

HashList lookup(char* s);
void insert(char *s , int value);
unsigned int hash(char *s);
HashList newList(int n, char *c, HashList l);

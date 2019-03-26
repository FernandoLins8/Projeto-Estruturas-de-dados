#define MAX 256

typedef struct element
{
	int freq;
	unsigned char *byte;
	int size;
} Element;

typedef struct hash_table
{
	Element *table[MAX];
} Hash_table;

Hash_table* create_hash_table()
{
	Hash_table *new = malloc(sizeof(Hash_table));

	int i;
	for(i=0; i<MAX; i++)
		new->table[i] = NULL;

	return new;
}


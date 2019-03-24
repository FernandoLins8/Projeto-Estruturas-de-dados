#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 256

typedef struct node
{
	unsigned char value;
	int priority;
	int control;
	struct node *next;
	struct node *left;
	struct node *right;
} Node;

typedef struct priority_queue
{
	Node *head;
} Queue;

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


Node* create_node(unsigned char value, int priority, Node *next, Node *left, Node *right);
Queue* create_priority_queue();
void enqueue(Queue *pq, Node *node);
Node* dequeue(Queue *pq);
void print_queue(Queue *pq);
Node* create_tree(Node *left, Node *right);
void write_tree(Node *node, FILE *output_file);

Hash_table* create_hash_table();
void create_new_enc(Element *element);
int height(Node *node);
void search(Hash_table *ht, Node* node, unsigned char *byte, int size);
int get_tree_size(Node *node);
int get_data_size(Hash_table *ht);

unsigned char set_bit(unsigned char c, int i);

void compress(FILE *input_file, Hash_table *ht, Node* tree);
void write_byte(FILE *output_file, unsigned char *str);

int main(int argc, char const *argv[])
{
	if(argc != 2)
	{
		printf("Invalid number of arguments\n");
		exit(1);
	}

	FILE *input_file;
	input_file = fopen(argv[1], "rb");

	int i, freq[MAX];
	unsigned char ch;
	memset(freq, 0, MAX*sizeof(int));

	fread(&ch, sizeof(ch), 1, input_file);
	while(!feof(input_file))
	{
		//if(ch != 10)
		freq[ch]++;
		fread(&ch, sizeof(ch), 1, input_file);
	}

	Queue *pq = create_priority_queue();

	for(i=0; i<MAX; i++)
		if(freq[i])
		{
			enqueue(pq, create_node(i, freq[i], NULL, NULL, NULL));
		}

	for(;;)
	{
		Node *n1 = dequeue(pq),
			 *n2 = dequeue(pq);
		Node *new = create_tree(n1, n2);

		if(!pq->head)
		{
			enqueue(pq, new);
			break;
		}
		enqueue(pq, new);
	}

	Hash_table *ht = create_hash_table();

	unsigned char *byte = malloc(sizeof(unsigned char)*height(pq->head));
	search(ht, pq->head, byte, 0);

	compress(input_file, ht, pq->head);

	return 0;
}


Node* create_node(unsigned char value, int priority, Node *next, Node *left, Node *right)
{
	Node *new = malloc(sizeof(Node));
	new->value = value;
	new->priority = priority;

	if(new->value == 42 || new->value == 92)
		new->control = 1;
	else
		new->control = 0;

	new->next = next;
	new->left = left;
	new->right = right;

	return new;
}

Queue* create_priority_queue()
{
	Queue *new = malloc(sizeof(Queue));
	new->head = NULL;

	return new;
}

void enqueue(Queue *pq, Node *node)
{
	if(!pq->head || pq->head->priority >= node->priority)
	{
		node->next = pq->head;
		pq->head = node;
	}
	else
	{
		Node *n = pq->head;
		while(n->next && n->next->priority < node->priority)
			n = n->next;

		node->next = n->next;
		n->next = node;
	}
}

Node* dequeue(Queue *pq)
{
	if(!pq->head)
		return NULL;
	else
	{
		Node *removed = pq->head;
		pq->head = pq->head->next;

		return removed;
	}
}

void print_queue(Queue *pq)
{
	if(!pq->head)
	{
		printf("Empty queue\n");
		return;
	}

	Node *n = pq->head;
	while(n)
	{
		printf("%c,%d ", n->value, n->priority);
		n = n->next;
	}
	printf("\n");
}

Node* create_tree(Node *left, Node *right)
{
	Node *new = malloc(sizeof(Node));
	new->value = '*';
	new->priority = left->priority + right->priority;

	new->left = left;
	new->right = right;

	return new;
}

void write_tree(Node *node, FILE *output_file)
{
	if(node)
	{
		if(node->control)
		{
			unsigned char aux = '\\';
			fwrite(&aux, 1, 1, output_file);
		}
		fwrite(&node->value, 1, 1, output_file);
		//printf("%c ", node->value);
		write_tree(node->left, output_file);
		write_tree(node->right, output_file);
	}
}

Hash_table* create_hash_table()
{
	Hash_table *new = malloc(sizeof(Hash_table));

	int i;
	for(i=0; i<MAX; i++)
		new->table[i] = NULL;

	return new;
}

int height(Node *node)
{
	if(!node)
		return 0;
	else
	{
		int left_height = height(node->left), right_height = height(node->right);

		if(left_height > right_height)
			return left_height+1;
		else
			return right_height+1;
	}
}

/*void create_new_enc(Element *element)
{
	int i = element->size;
	unsigned char new = 0;
	while(i)
	{
		if(element->byte[i-1] == '1')
			new = set_bit(new, element->size - i);
		i--;
	}
	element->new_enc = new;
}*/

void search(Hash_table *ht, Node* node, unsigned char *byte, int size)
{
	if(node)
	{
		if(!node->left && !node->right)
		{
			ht->table[node->value] = malloc(sizeof(Element));
			unsigned char *new_byte = malloc(sizeof(unsigned char)*size);

			byte[size] = '\0';
			strcpy(new_byte, byte);
			ht->table[node->value]->freq = node->priority;
			ht->table[node->value]->byte = new_byte;
			ht->table[node->value]->size = size;

			//printf("%c %s %hu\n", node->value, ht->table[node->value]->byte, ht->table[node->value]->new_enc);
		}
		else
		{
			byte[size] = '0';
			search(ht, node->left, byte, ++size);
			size--;

			byte[size] = '1';
			search(ht, node->right, byte, ++size);
			size--;
		}
	}
}

int get_tree_size(Node *node)
{
	if(!node)
		return 0;

	if(!node->left && !node->right)
	{
		if(node->control)
			return 2;
		else
			return 1;

	}
	return get_tree_size(node->left) + get_tree_size(node->right) + 1;
}

int get_data_size(Hash_table *ht)
{
	int i, total = 0;

	for(i=0; i<MAX; i++)
		if(ht->table[i])
		{
			total += (ht->table[i]->freq * ht->table[i]->size); 
		}
	return total;
}

unsigned char set_bit(unsigned char c, int i)
{
	unsigned char mask = 1 << i;
	return mask | c;
}

void two_first_bytes(FILE *output_file, int data_size, int tree_size)
{
	unsigned char trash_size, first_byte, second_byte, complete_first;

	trash_size = 8 - ((data_size % 8) % 8);
	//printf("lixo - %d\n", trash_size);
	trash_size = trash_size << 5;

	complete_first = tree_size >> 8;

	first_byte = trash_size | complete_first;
	second_byte = tree_size;

	//printf("%hu\n", first_byte);
	//printf("%hu\n", second_byte);

	fwrite(&first_byte, 1, 1, output_file);
	fwrite(&second_byte, 1, 1, output_file);
}

void compress(FILE *input_file, Hash_table *ht, Node* tree)
{
	FILE *output_file;
	output_file = fopen("output_file", "wb");

	unsigned short tree_size = get_tree_size(tree),
	data_size = get_data_size(ht);

	//printf("tree size = %hu\n", tree_size);

	two_first_bytes(output_file, data_size, tree_size);
	write_tree(tree, output_file);

	rewind(input_file);

	int byte_size = 0, i, j;
	unsigned char ch = 0, byte[8];
	byte[0] = '\0';

	fread(&ch, sizeof(ch), 1, input_file);
	while(!feof(input_file))
	{
		if(ht->table[ch]->size + byte_size < 8)
		{
			//printf("MENOR\n");
			strcat(byte, ht->table[ch]->byte);
			//printf("CONC %s << %s\n\n", byte, ht->table[ch]->byte);
			byte_size += ht->table[ch]->size;
		}
		else if(ht->table[ch]->size + byte_size == 8)
		{
			//printf("IGUAL\n");
			strcat(byte, ht->table[ch]->byte);
			//printf("byte %s << %s\n\n", byte, ht->table[ch]->byte);
			write_byte(output_file, byte);
			byte[0] = '\0';
			byte_size = 0;
		}
		else
		{
			//printf("MAIOR\n");
			unsigned char *aux_byte = malloc(sizeof(unsigned char)*ht->table[ch]->size);
			strcpy(aux_byte, ht->table[ch]->byte);
			//printf("%s << cpy %s\n", aux_byte, ht->table[ch]->byte);
			//printf("byte = %s\n", byte);
			for(i=0; i < ht->table[ch]->size; i++)
			{
				byte[byte_size] = aux_byte[i];
				byte_size++;
				if(byte_size == 8)
				{
					//printf("put %s\n", byte);
					write_byte(output_file, byte);
					byte_size = 0;
					byte[0] = '\0';
				}
			}
			byte[byte_size] = '\0';
		}
		fread(&ch, sizeof(ch), 1, input_file);
	}
	if(data_size % 8)
	{
		write_byte(output_file, byte);
		//printf("put %s\n", byte);
	}
}

void write_byte(FILE *output_file, unsigned char *str)
{
	int i;
	unsigned char byte = 0;
	for(i=0; i<8; i++)
	{
		if(str[i] == '\0')
			break;

		if(str[i] == '1')
			byte = set_bit(byte, 7 - i);
	}
	//printf("%d\n", byte);
	fwrite(&byte, 1, 1, output_file);
}
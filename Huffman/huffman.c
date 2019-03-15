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
void print_tree(Node *tree);

Hash_table* create_hash_table();
int height(Node *node);
void search(Hash_table *ht, Node* node, unsigned char *byte, int size);
int get_tree_size(Node *node);
int get_data_size(Hash_table *ht);


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
		if(ch != 10)
			freq[ch]++;
		fread(&ch, sizeof(ch), 1, input_file);
	}

	/*for(i=0; i<MAX; i++)
		if(freq[i])
			printf("%c %d\n", i, freq[i]);
	*/

	Queue *pq = create_priority_queue();

	for(i=0; i<MAX; i++)
		if(freq[i])
		{
			enqueue(pq, create_node(i, freq[i], NULL, NULL, NULL));
		}
	//print_queue(pq);

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

	//print_tree(pq->head);

	Hash_table *ht = create_hash_table();

	unsigned char *byte = malloc(sizeof(unsigned char)*height(pq->head)+1);
	search(ht, pq->head, byte, 0);

	/*Print Hash
	for(i=0; i<MAX; i++)
	{
		if(ht->table[i])
		{
			printf("%c %d %s %d\n", i, ht->table[i]->freq, ht->table[i]->byte, ht->table[i]->size);
		}
	}*/

	int tree_size = get_tree_size(pq->head),
	data_size = get_data_size(ht),
	trash_size = (tree_size + data_size) % 8;

	printf("tree_size = %d\ndata_size = %d\ntrash_size = %d\n", get_tree_size(pq->head), get_data_size(ht), trash_size);

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

void print_tree(Node *tree)
{
	if(tree)
	{
		printf("%c ", tree->value);
		print_tree(tree->left);
		print_tree(tree->right);
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
		int left_height = height(node->left),
		right_height = height(node->right);

		if(left_height > right_height)
			return left_height+1;
		else
			return right_height+1;
	}
}

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

			//printf("%c %s\n", node->value, ht->table[node->value]->byte);
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
	if(node)
	{
		return get_tree_size(node->left) + get_tree_size(node->right) + 1;
	}
	
	return 0;
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

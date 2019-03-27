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

Node* create_tree(Node *left, Node *right)
{
	Node *new = malloc(sizeof(Node));
	new->value = '*';
	new->priority = left->priority + right->priority;

	new->left = left;
	new->right = right;

	return new;
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

Hash_table* create_hash_table()
{
	Hash_table *new = malloc(sizeof(Hash_table));

	int i;
	for(i=0; i<MAX; i++)
		new->table[i] = NULL;

	return new;
}

void pre_comp(char const *input_name, char const *output_name)
{
	FILE *input_file;
	input_file = fopen(input_name, "rb");

	FILE *output_file;
	output_file = fopen(output_name, "wb");

	int i, freq[MAX];
	unsigned char ch;
	memset(freq, 0, MAX*sizeof(int));

	fread(&ch, sizeof(ch), 1, input_file);
	while(!feof(input_file))
	{
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

	compress(input_file, output_file, ht, pq->head);
}
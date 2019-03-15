#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50

typedef struct stack
{
	int size;
	int item[MAX];
} Stack;

Stack* create_stack();
void push(Stack* stack, int n);
int pop(Stack* stack);
int top(Stack* stack);
int empty_stack(Stack* stack);
void print_stack(Stack* stack);

Stack* create_stack()
{
	Stack* new_stack = malloc(sizeof(Stack));
	new_stack->size = 0; 
	return new_stack;
}

void push(Stack* stack, int n)
{
	if(stack->size == MAX)
		printf("Stack overflow\n");
	else
	{
		stack->size++;
		stack->item[stack->size] = n;
	}
}

int pop(Stack* stack)
{
	if(!stack->size)
	{
		printf("Empty Stack\n");
		return -1;
	}
	else
	{
		int removed = stack->item[stack->size];
		stack->size--;
		return removed;
	}
}

int top(Stack* stack)
{
	return stack->item[stack->size];
}

int empty_stack(Stack* stack)
{
	return !stack->size;
}

void print_stack(Stack* stack)
{
	int n = stack->size;
	printf("top| ");
	while(n != 0)
	{
		printf("%d ", stack->item[n]);
		n--;
	}
	printf("|base\n");
}

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
	int key;
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


int main(int argc, char const *argv[])
{

	Stack* stack = create_stack();

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
		freq[ch]++;
		fread(&ch, sizeof(ch), 1, input_file);
	}

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

	print_tree(pq->head);
	printf("\n");
	new_map(pq->head, stack);

	return 0;
}

void new_map(Node* node, Stack* stack){
	if(node){
		if(!node->left && !node->right){
			print_stack(stack);
			printf(" - %c\n", node->value);
			//pop(stack);
		}else{
			push(stack, 0);
			new_map(node->left, stack);
			push(stack, 1);
			new_map(node->right, stack);
		}
	}
	pop(stack);
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
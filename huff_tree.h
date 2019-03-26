typedef struct node
{
	unsigned char value;
	int priority;
	int control;
	struct node *next;
	struct node *left;
	struct node *right;
} Node;

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

Node* create_tree(Node *left, Node *right)
{
	Node *new = malloc(sizeof(Node));
	new->value = '*';
	new->priority = left->priority + right->priority;

	new->left = left;
	new->right = right;

	return new;
}
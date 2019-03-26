typedef struct priority_queue
{
	Node *head;
} Queue;

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
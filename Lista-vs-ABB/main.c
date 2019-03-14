#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct binary_tree binary_tree;
typedef struct node node;

struct binary_tree{
	int item;
	binary_tree *left;
	binary_tree *right;
};

struct node{
	int item;
	node *next;
};

binary_tree* create_empty_binary_tree(){
	return NULL;
}

node* create_linked_list(){
	return NULL;
}

int is_empty(binary_tree *bt){
	return(bt == NULL);
}

int is_empty_linked_list(node *head){
	return(head == NULL);
}

void free_bt(binary_tree *bt) {
    if(!is_empty(bt)) {
        free_bt(bt->left);
        free_bt(bt->right);
        free(bt);
    }
}

void free_linked_list(node *head){
	if(!is_empty_linked_list(head)){
		free_linked_list(head->next);
		free(head);
	}
}

binary_tree* create_binary_tree(int item, binary_tree *left, binary_tree *right){
	binary_tree *new_binary_tree = (binary_tree*) malloc(sizeof(binary_tree));
	new_binary_tree->item = item;
	new_binary_tree->left = left;
	new_binary_tree->right = right;
	return new_binary_tree;
}

node* add_linked_list(node *head, int item){
	node *new_node = (node*) malloc(sizeof(node));
	new_node->item = item;
	new_node->next = head;
	return new_node;
}

binary_tree* add(binary_tree *bt, int item){
	if(bt == NULL){
		bt = create_binary_tree(item, NULL, NULL);
	}else if(bt->item > item){
		bt->left = add(bt->left, item);
	}else{
		bt->right = add(bt->right, item);
	}
	return bt;
}

int search_linked_list(node *head, int item){
	int count = 0;
	while(head != NULL){
		count++;
		if(head->item == item){
			return count;
		}
		head = head->next;
	}
	return -1;
}

binary_tree* search(binary_tree *bt, int item, int *comparisons){
	if((bt == NULL) || (bt->item == item)){
		++*comparisons;
		return bt;
	}else if(bt->item > item){
		++*comparisons;
		return search(bt->left, item, &*comparisons);
	}else{
		++*comparisons;
		return search(bt->right, item, &*comparisons);
	}
}

void swap(int *a, int *b) {
    int aux = *a;
    *a = *b;
    *b = aux;
}

int main(){
	int max_size, each;
	printf("Tamanho maximo:");
	scanf("%d", &max_size);
	printf("Intervalo:");
	scanf("%d", &each);
	FILE *amostra;
	amostra = fopen("amostra.txt", "w");
	int i, j, k, array[max_size], media = 0, comparisons = 0, cont = 0;
	fprintf(amostra, "list bst size\n");
	clock_t tempo;
  	tempo = clock();
	for(i = each; i <= max_size; i+=each){
		for(j = 0; j < i; j++){
			array[j] = j;
		}
		for(j = 0; j < i; j++){
			k = rand() % i;
			swap(&array[j], &array[k]);
		}
		binary_tree* root = create_empty_binary_tree();
		node* list = create_linked_list();
		for(j = 0; j < i; j++){
			list = add_linked_list(list, array[j]);
			if(j == 0){
				root = add(root, array[j]);
			}else{
				add(root, array[j]);
			}
		}
		for(j = 0; j < 100; j++){
			k = rand() % i;
			search(root, k, &comparisons);
			cont += search_linked_list(list, k);
			media += comparisons;
			comparisons = 0;
		}
		printf("%d %d %d\n", (int)cont/100, (int)media/100, i);
		fprintf(amostra, "%d %d %d\n", (int)cont/100, (int)media/100, i);
		media = cont = 0;
		free_bt(root);
		free_linked_list(list);
	}
	printf("Tempo:%f\n",(clock() - tempo) / (double)CLOCKS_PER_SEC);
	fclose(amostra);
	return 0;
}
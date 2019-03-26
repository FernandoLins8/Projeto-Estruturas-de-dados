#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _bt huff_tree;

struct _bt{
    unsigned char item;
    unsigned int control;
    unsigned long long int frequency;
    huff_tree *left;
    huff_tree *right;
    huff_tree *next;
};

huff_tree* create_binary_tree(unsigned char item, unsigned long long int frequency, huff_tree* left, huff_tree* right, unsigned int control){

    huff_tree *new_bt = (huff_tree*)malloc(sizeof(huff_tree));
    new_bt->item = item;
    new_bt->control = control;
    new_bt->frequency = frequency;
    new_bt->left = left;
    new_bt->right = right;

    return new_bt;
}

huff_tree* construct_tree_from_preorder(FILE* file, int tree_size){ 

	unsigned char ch;

	huff_tree *temp;
	
	fread(&ch, sizeof(ch), 1, file);
  
    if(0 > tree_size){ 
        return NULL;
    } 

    if(ch == 92){
    	fread(&ch, sizeof(ch), 1, file);
    	temp = create_binary_tree(ch, 0, NULL, NULL, 1);
    }else{
    	temp = create_binary_tree(ch, 0, NULL, NULL, 0);
    	if(ch == 42){ 
    		temp->left  = construct_tree_from_preorder(file, --tree_size); 
      		temp->right = construct_tree_from_preorder(file, --tree_size); 
    	}
    }
  
    return temp; 
}

int is_bit_i_set(unsigned char c, int i){
	unsigned char mask = 1 << i;
	return mask & c;
}

void decompress(huff_tree* node, FILE* file, FILE* output_file, long int size_file, int trash_size){
	unsigned char ch;
	int i, final = 0;

	huff_tree* temp = node;

	while(size_file){
		fread(&ch, sizeof(ch), 1, file);
		if(size_file-1 == 0){
			final = trash_size;
		}
		for(i = 7; i >= final; --i){
			if(is_bit_i_set(ch, i)){
				temp = temp->right;
			}else{
				temp = temp->left;
			}
			if(!temp->left){
				if(temp->control){
					fwrite(&temp->item, 1, 1, output_file);
				}else{
					fwrite(&temp->item, 1, 1, output_file);
				}
				temp = node;
			}
		}
		size_file--;
	}
}

int main(int argc, char const *argv[]){

	unsigned char ch;
	unsigned char temp = 0;
	unsigned int tree_size = 0;
	unsigned int trash_size = 0;

	if(argc != 3){
		printf("Invalid number of arguments\n");
		exit(1);
	}

	FILE *input_file;
	input_file = fopen(argv[1], "rb");

	FILE *output_file;
	output_file = fopen(argv[2], "wb");

	fseek(input_file, 0L, SEEK_END);
	long int size_file = ftell(input_file);
	rewind(input_file);

	fread(&ch, sizeof(ch), 1, input_file);
	trash_size = ch >> 5;

	temp = ch << 3;
	tree_size = temp >> 3;
	fread(&ch, sizeof(ch), 1, input_file);
	tree_size = tree_size << 8 | ch;

	huff_tree* bt = construct_tree_from_preorder(input_file, tree_size);

	size_file -= 2+tree_size;

	decompress(bt, input_file, output_file, size_file, trash_size);

	return 0;
}
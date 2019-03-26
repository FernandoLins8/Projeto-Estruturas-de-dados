#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huff_tree.h"

Node* construct_tree_from_preorder(FILE* file, int tree_size){ 

	unsigned char ch;

	Node *temp;
	
	fread(&ch, sizeof(ch), 1, file);
  
    if(0 > tree_size){ 
        return NULL;
    } 

    if(ch == 92){
    	fread(&ch, sizeof(ch), 1, file);
    	temp = create_node(ch, 0, NULL, NULL, NULL);
    }else{
    	temp = create_node(ch, 0, NULL, NULL, NULL);
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

void decompress(Node* node, FILE* file, FILE* output_file, long int size_file, int trash_size){
	unsigned char ch;
	int i, final = 0;

	Node* temp = node;

	while(size_file){
		fread(&ch, sizeof(ch), 1, file);
		if(size_file == 1){
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
					fwrite(&temp->value, 1, 1, output_file);
				}else{
					fwrite(&temp->value, 1, 1, output_file);
				}
				temp = node;
			}
		}
		size_file--;
	}
}

void decode(char const *input_name[], char const *output_name[]){

	unsigned char ch;
	unsigned char temp = 0;
	unsigned int tree_size = 0;
	unsigned int trash_size = 0;

	FILE *input_file;
	input_file = fopen(input_name, "rb");

	FILE *output_file;
	output_file = fopen(output_name, "wb");

	fseek(input_file, 0L, SEEK_END);
	long int size_file = ftell(input_file);
	rewind(input_file);

	fread(&ch, sizeof(ch), 1, input_file);
	trash_size = ch >> 5;

	temp = ch << 3;
	tree_size = temp >> 3;
	fread(&ch, sizeof(ch), 1, input_file);
	tree_size = tree_size << 8 | ch;

	Node* bt = construct_tree_from_preorder(input_file, tree_size);

	size_file -= 2+tree_size;

	decompress(bt, input_file, output_file, size_file, trash_size);
}
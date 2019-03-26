#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pre_comp.h"


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
	trash_size = trash_size << 5;

	complete_first = tree_size >> 8;

	first_byte = trash_size | complete_first;
	second_byte = tree_size;

	fwrite(&first_byte, 1, 1, output_file);
	fwrite(&second_byte, 1, 1, output_file);
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
		write_tree(node->left, output_file);
		write_tree(node->right, output_file);
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
	fwrite(&byte, 1, 1, output_file);
}

void compress(FILE *input_file, FILE *output_file, Hash_table *ht, Node* tree)
{
	unsigned short tree_size = get_tree_size(tree),
	data_size = get_data_size(ht);

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
			strcat(byte, ht->table[ch]->byte);
			byte_size += ht->table[ch]->size;
		}
		else if(ht->table[ch]->size + byte_size == 8)
		{
			strcat(byte, ht->table[ch]->byte);
			write_byte(output_file, byte);
			byte[0] = '\0';
			byte_size = 0;
		}
		else
		{
			unsigned char *aux_byte = malloc(sizeof(unsigned char)*ht->table[ch]->size);
			strcpy(aux_byte, ht->table[ch]->byte);
			for(i=0; i < ht->table[ch]->size; i++)
			{
				byte[byte_size] = aux_byte[i];
				byte_size++;
				if(byte_size == 8)
				{
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
	}
}
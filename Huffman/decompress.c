#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[]){

	unsigned char ch;
	unsigned char temp;
	unsigned int tree_size = 0;
	unsigned int trash_size = 0;

	if(argc != 2){
		printf("Invalid number of arguments\n");
		exit(1);
	}

	FILE *input_file;
	input_file = fopen(argv[1], "rb");

	fread(&ch, sizeof(ch), 1, input_file);
	trash_size = ch >> 5;

	temp = ch << 3;
	fread(&ch, sizeof(ch), 1, input_file);
	tree_size = temp >> 8 | ch;

	printf("%d\n", tree_size);

	while(!feof(input_file)){
		fread(&ch, sizeof(ch), 1, input_file);
		printf("%c", ch);
	}

	return 0;
}
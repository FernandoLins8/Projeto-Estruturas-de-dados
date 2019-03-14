#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 256

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
		freq[ch]++;
		fread(&ch, sizeof(ch), 1, input_file);
	}
	return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encode.h"
#include "decode.h"

int main(int argc, char const *argv[]){
	
	if(argc != 4){
		printf("Invalid number of arguments\n");
		printf("Format: <encode/decode> <input file> <output file>\n");
		exit(1);
	}

	if(strcmp(argv[1], "encode") == 0){
		encode(argv[2], argv[3]);
	}else if(strcmp(argv[1], "decode") == 0){
		decode(argv[2], argv[3]);
	}else{
		printf("wrong command\n");
	}

	return 0;
}
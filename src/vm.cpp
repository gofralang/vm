#include <stdio.h>

int main(int argc, char* argv[]){
	printf("[Gofra VM] Welcome to the Gofra VM CLI!\n");
	
	if (argc == 0){
		printf("[Gofra VM] Error! Invalid arguments count (Zero, 0)!\n");
		return 1;
	}
	
	if (argc == 1){
		printf("[Gofra VM] Error! No bytecode file providen!\n");
		return 1;
	}
	
	if (argc == 2){
		char* vm_path = argv[0];
		char* vm_bytecode_path = argv[1];
		
		printf(vm_bytecode_path);
		return 0;
	}
	
	printf("[Gofra VM] Error! Invalid arguments count!\n");
	return 1;
}

#include <stdio.h>
#include <vector>
#include <string>
#include <bits/stdc++.h> 
#include <bits/stl_bvector.h>
#include <stack>
#include <sstream>

// WARN. WORK IN PROGRESS.
// THIS IS TEST CODE.

#define VM_BYTECODE_READ_BUFFER_SIZE 256
#define VM_BYTECODE_BUFFER_SIZE 2048
#define VM_BYTECODE_TOKEN_BUFFER_SIZE 64

using namespace std;

int vm_execute_operation(stack<int>* memory_stack, vector<char*> bytecode, int current_index, char* operation){
	
	if (strcmp(operation, "SP_I") == 0){
		char* operand = bytecode[current_index + 1];
		
		stringstream ss;
		int num;
		ss << operand;
		ss >> num;
		
		memory_stack->push(num);
		return current_index + 2;
	}
	
	if (strcmp(operation, "I+") == 0){
		int operand_a = memory_stack->top();
		memory_stack->pop();
		int operand_b = memory_stack->top();
		memory_stack->pop();
		
		memory_stack->push(operand_b + operand_a);
		return current_index + 1;
	}
	
	if (strcmp(operation, "I_SHOW") == 0){
		int operand = memory_stack->top();
		memory_stack->pop();
		printf("%d\n", operand);
		return current_index + 1;
	}
	
	printf("Error! Got unexpected bytecode VM operation: `%s`\n", operation);
	return ++current_index;
}

void vm_execute_bytecode(vector<char*>* bytecode_ptr){
	fputs("[Gofra VM] Executing bytecode...\n", stdout);
	int current_operation_index = 0;
	int size = bytecode_ptr->size();
	
	
	printf("[Gofra VM] Bytecode has %d operators!\n", size);
	stack<int> memory_stack;
	vector<char*> bytecode = *bytecode_ptr;
	while (current_operation_index < size){
		current_operation_index = vm_execute_operation(&memory_stack, bytecode, current_operation_index, bytecode[current_operation_index]);
	}
	fputs("[Gofra VM] Successfully executed bytecode!\n", stdout);
}

int vm_execute_file(char* bytecode_path){
	fputs("[Gofra VM] Opening bytecode file...\n", stdout);
	
	FILE* bytecode_fp = fopen(bytecode_path, "r");
	if (bytecode_fp == NULL){
		printf("[Gofra VM] Error! Failed to open bytecode file!\n");
		return 1;
	}
	
	vector<char*> bytecode;
	char* token = NULL;
	char line_buffer[VM_BYTECODE_READ_BUFFER_SIZE];
	fputs("[Gofra VM] Reading bytecode file...\n", stdout);
    while (!feof(bytecode_fp)){
    	if (fgets(line_buffer, VM_BYTECODE_READ_BUFFER_SIZE, bytecode_fp) == NULL){
			break;
		}
		
		token = strtok(line_buffer, " \n");
	    while (token != NULL)
	    {
	    	char* bytecode_operator = (char*)malloc(strlen(token) + 1);
	    	strcpy(bytecode_operator, token);
	    	
	    	bytecode.push_back(bytecode_operator);
	        token = strtok(NULL, " \n");
	    }
    }
    
    fputs("[Gofra VM] Successfully readed bytecode file!\n", stdout);
    fputs("------\n", stdout);
	fclose(bytecode_fp);
	
	vm_execute_bytecode(&bytecode);
	//fputs("------\n", stdout);
	return 0;
}

int main(int argc, char* argv[]){
	fputs("[Gofra VM] Welcome to the Gofra VM CLI!\n", stdout);
	fputs("------\n", stdout);
	
	if (argc == 0){
		fputs("[Gofra VM] Error! Invalid arguments count (Zero, 0)!\n", stderr);
		return 1;
	}
	
	if (argc == 1){
		fputs("[Gofra VM] Error! No bytecode file providen!\n", stderr);
		return 1;
	}
	
	if (argc == 2){
		return vm_execute_file(argv[1]);
	}
	
	fputs("[Gofra VM] Error! Invalid arguments count!\n", stderr);
	return 1;
}


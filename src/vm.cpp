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

// VM Operations.
// Math.
void vm_op_plus(stack<int>* s){
	int a = s->top(); s->pop();
	int b = s->top(); s->pop();
	s->push(b + a);
}
void vm_op_minus(stack<int>* s){
	int a = s->top(); s->pop();
	int b = s->top(); s->pop();
	s->push(b - a);
}
void vm_op_dec(stack<int>* s){
	int a = s->top(); s->pop();
	s->push(--a);
}
void vm_op_inc(stack<int>* s){
	int a = s->top(); s->pop();
	s->push(++a);
}
void vm_op_div(stack<int>* s){
	int a = s->top(); s->pop();
	int b = s->top(); s->pop();
	s->push(b / a);
}
void vm_op_mul(stack<int>* s){
	int a = s->top(); s->pop();
	int b = s->top(); s->pop();
	s->push(b * a);
}
void vm_op_eq(stack<int>* s){
	int a = s->top(); s->pop();
	int b = s->top(); s->pop();
	s->push(b == a);
}
void vm_op_ne(stack<int>* s){
	int a = s->top(); s->pop();
	int b = s->top(); s->pop();
	s->push(b != a);
}
void vm_op_gt(stack<int>* s){
	int a = s->top(); s->pop();
	int b = s->top(); s->pop();
	s->push(b > a);
}
void vm_op_lt(stack<int>* s){
	int a = s->top(); s->pop();
	int b = s->top(); s->pop();
	s->push(b < a);
}
void vm_op_geq(stack<int>* s){
	int a = s->top(); s->pop();
	int b = s->top(); s->pop();
	s->push(b >= a);
}
void vm_op_leq(stack<int>* s){
	int a = s->top(); s->pop();
	int b = s->top(); s->pop();
	s->push(b <= a);
}
void vm_op_mod(stack<int>* s){
	int a = s->top(); s->pop();
	int b = s->top(); s->pop();
	s->push(b % a);
}
// Stack.
void vm_op_show(stack<int>* s){
	int a = s->top(); s->pop();
	printf("%d\n", a);
}

// VM Execution.
int vm_execute_operation(stack<int>* memory_stack, vector<char*> bytecode, int current_index, char* operation){
	
	if (strcmp(operation, "I") == 0){
		char* operand = bytecode[current_index + 1];
		
		stringstream ss;
		int num;
		ss << operand;
		ss >> num;
		
		memory_stack->push(num);
		return current_index + 2;
	}
	
	if (strcmp(operation, "+") == 0){
		vm_op_plus(memory_stack); return ++current_index;
	}
	if (strcmp(operation, "-") == 0){
		vm_op_minus(memory_stack); return ++current_index;
	}
	if (strcmp(operation, "*") == 0){
		vm_op_mul(memory_stack); return ++current_index;
	}
	if (strcmp(operation, "/") == 0){
		vm_op_div(memory_stack); return ++current_index;
	}
	if (strcmp(operation, "==") == 0){
		vm_op_eq(memory_stack); return ++current_index;
	}
	if (strcmp(operation, "!=") == 0){
		vm_op_ne(memory_stack); return ++current_index;
	}
	if (strcmp(operation, ">") == 0){
		vm_op_gt(memory_stack); return ++current_index;
	}
	if (strcmp(operation, "<") == 0){
		vm_op_lt(memory_stack); return ++current_index;
	}
	if (strcmp(operation, ">=") == 0){
		vm_op_geq(memory_stack); return ++current_index;
	}
	if (strcmp(operation, "<=") == 0){
		vm_op_leq(memory_stack); return ++current_index;
	}
	if (strcmp(operation, "%") == 0){
		vm_op_mod(memory_stack); return ++current_index;
	}
	if (strcmp(operation, "--") == 0){
		vm_op_dec(memory_stack); return ++current_index;
	}
	if (strcmp(operation, "++") == 0){
		vm_op_inc(memory_stack); return ++current_index;
	}
	if (strcmp(operation, "SH") == 0){
		vm_op_show(memory_stack); return ++current_index;
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

// Entry point.

// CLI flags.
bool silent = false; // Hides messages, except from execution.
bool verbose = false; // Shows system* messages.
	
void readArgs(int argc, char* argv[]){
	for (int argi = 0; argi < argc; argi++){
		char* arg = argv[argi];
			
		if (strcmp(arg, "-s") == 0){
			silent = true;
			continue;
		}
		
		if (strcmp(arg, "-v") == 0){
			verbose = true;
			continue;
		}
	}
}

int main(int argc, char* argv[]){
	readArgs(argc, argv);
	
	if (!silent) fputs("[Gofra VM] Welcome to the Gofra VM CLI!\n", stdout);

	if (argc == 0){
		fputs("[Gofra VM] Error! Invalid arguments count (Zero, 0)!\n", stderr);
		return 1;
	}
	
	if (argc == 1){
		fputs("[Gofra VM] Error! No bytecode file providen!\n", stderr);
		return 1;
	}
	
	return vm_execute_file(argv[1]);
}


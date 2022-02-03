#include <bits/stdc++.h> 
#include <bits/stl_bvector.h>
#include <stack>
#include <sstream>
#include <errno.h>

// WARN. WORK IN PROGRESS.
// THIS IS TEST CODE.

#define VM_BYTECODE_BUFFER_SIZE 2048
#define VM_BYTECODE_TOKEN_BUFFER_SIZE 64

// CLI flags.
bool silent = false; // Hides messages, except from execution.
bool verbose = false; // Shows system* messages.
	
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
	if (!silent) fputs("[Gofra VM] Executing bytecode...\n", stdout);
	int current_operation_index = 0;
	int size = bytecode_ptr->size();
	
	
	if (!silent && verbose) printf("[Gofra VM] Bytecode has %d operators!\n", size);
	stack<int> memory_stack;
	vector<char*> bytecode = *bytecode_ptr;
	while (current_operation_index < size){
		current_operation_index = vm_execute_operation(&memory_stack, bytecode, current_operation_index, bytecode[current_operation_index]);
	}
	if (!silent) fputs("[Gofra VM] Successfully executed bytecode!\n", stdout);
}

// Bytecode.
FILE* bc_open_file(const char* path){
	if (!silent && verbose) fputs("Opening bytecode file...\n", stdout);
	
	FILE* fp = fopen(path, "r");
	if (fp == NULL){
		if (errno == ENOENT){
			fputs("ERROR! Failed to open bytecode file for reading because file not exists!\n", stderr);
			exit(1);
		}
		fputs("ERROR! Failed to open bytecode file for reading!\n", stderr);
		exit(1);
	}
	
	return fp;
}
void bc_read_tokens_from_string(char* string, vector<char*>* bc){
	#define SEPARATORS " \n"
	char* token = strtok(string, SEPARATORS);
    while(token != NULL){
    	char* op = (char*)malloc(strlen(token) + 1);
    	strcpy(op, token);
    	
    	bc->push_back(op);
    	
        token = strtok(NULL, SEPARATORS);
    }
}
void bc_read_tokens_from_file(FILE* fp, vector<char*>* bc){
	if (!silent && verbose) fputs("Reading bytecode file...\n", stdout);
	
	#define BUFFER_SIZE 256
	char buffer[BUFFER_SIZE];
	
    while (!feof(fp)){
    	if (fgets(buffer, BUFFER_SIZE, fp) == NULL) break;
		bc_read_tokens_from_string((char*)&buffer, bc);
    } 
    
    if (!silent && verbose) fputs("Finished reading bytecode file!\n", stdout);
}

// Virtual machine. 
void vm_execute_file(const char* path){
	FILE* fp = bc_open_file(path);
	
	vector<char*>* bytecode = new vector<char*>;
	
	bc_read_tokens_from_file(fp, bytecode);
	fclose(fp);
   
	vm_execute_bytecode(bytecode);
}

// CLI.
void read_args(int argc, char* argv[]){
	for (int argi = 0; argi < argc; argi++){
		char* arg = argv[argi];
			
		if (strcmp(arg, "-s") == 0){
			silent = true; continue;
		}
		if (strcmp(arg, "-v") == 0){
			verbose = true; continue;
		}
	}
}
void usage(int argc, char* argv[]){
	const char* program;
	if (argc > 0){
		program = argv[0];
	}else exit(127);

	fputs("Execute Gofra language bytecode from the CLI.\n\n", stdout);
	fputs("USAGE: \n", stdout);
	
	// "program path [-v] [-v]"
	fputs("\t", stdout); fputs(program, stdout); fputs(" <path> [-v] [-s]\n", stdout);
	fputs("\t  path - Path to the `.gofbc` file with the bytecode.\n\n", stdout);
	
	fputs("FLAGS:\n", stdout);
	fputs("\t[-v] Verbose flag, will show more debug messages,\n", stdout);
	fputs("\t[-s] Silent flag, will supress all messages except execution.\n", stdout);
}

// Entry point.
int main(int argc, char* argv[]){
	if (argc <= 1){
		usage(argc, argv);
		exit(127);
	}
	
	read_args(argc, argv);
	vm_execute_file(argv[1]);
	
	return 0;
}


// Gofra programming language bytecode bytecode.
// https://gofra-lang.github.io/
// https://github.com/gofra-lang/vm
// (c) 2022 Kirill Zhosul.

// WARNING:
// This code is more like test code!

#include <bits/stdc++.h> 

// CLI flags.
bool silent = false;
bool verbose = false;
	
using namespace std;

// Operations.
int vm_op_plus(stack<int>* s, int i){
	int a = s->top(); s->pop();
	int b = s->top(); s->pop();
	s->push(b + a);return ++i;
}
int vm_op_minus(stack<int>* s, int i){
	int a = s->top(); s->pop();
	int b = s->top(); s->pop();
	s->push(b - a); return ++i;
}
int vm_op_dec(stack<int>* s, int i){
	int a = s->top(); s->pop();
	s->push(--a); return ++i;
}
int vm_op_inc(stack<int>* s, int i){
	int a = s->top(); s->pop();
	s->push(++a); return ++i;
}
int vm_op_div(stack<int>* s, int i){
	int a = s->top(); s->pop();
	int b = s->top(); s->pop();
	s->push(b / a); return ++i;
}
int vm_op_mul(stack<int>* s, int i){
	int a = s->top(); s->pop();
	int b = s->top(); s->pop();
	s->push(b * a); return ++i;
}
int vm_op_eq(stack<int>* s, int i){
	int a = s->top(); s->pop();
	int b = s->top(); s->pop();
	s->push(b == a); return ++i;
}
int vm_op_ne(stack<int>* s, int i){
	int a = s->top(); s->pop();
	int b = s->top(); s->pop();
	s->push(b != a); return ++i;
}
int vm_op_gt(stack<int>* s, int i){
	int a = s->top(); s->pop();
	int b = s->top(); s->pop();
	s->push(b > a); return ++i;
}
int vm_op_lt(stack<int>* s, int i){
	int a = s->top(); s->pop();
	int b = s->top(); s->pop();
	s->push(b < a); return ++i;
}
int vm_op_geq(stack<int>* s, int i){
	int a = s->top(); s->pop();
	int b = s->top(); s->pop();
	s->push(b >= a); return ++i;
}
int vm_op_leq(stack<int>* s, int i){
	int a = s->top(); s->pop();
	int b = s->top(); s->pop();
	s->push(b <= a); return ++i;
}
int vm_op_mod(stack<int>* s, int i){
	int a = s->top(); s->pop();
	int b = s->top(); s->pop();
	s->push(b % a); return ++i;
}
int vm_op_show(stack<int>* s, int i){
	printf("%d\n", s->top()); s->pop();
	return ++i;
}
int vm_op_push_integer(stack<int>* s, int i, char* o){
	int integer;
	
	stringstream ss;
	ss << o;
	ss >> integer;
	
	s->push(integer);
	return i + 2;
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
int vm_execute_operation(stack<int>* ms, vector<char*> bc, int index, char* op){
	// Base.
	if (!strcmp(op, "I")) return vm_op_push_integer(ms, index, bc[index + 1]);
	if (!strcmp(op, "SH")) return vm_op_show(ms, index);
	
	// Math.
	if (!strcmp(op, "+")) return vm_op_plus(ms, index);
	if (!strcmp(op, "-")) return vm_op_minus(ms, index);
	if (!strcmp(op, "*")) return vm_op_mul(ms, index);
	if (!strcmp(op, "/")) return vm_op_div(ms, index);
	if (!strcmp(op, ">")) return vm_op_gt(ms, index);
	if (!strcmp(op, "<")) return vm_op_lt(ms, index);
	if (!strcmp(op, "%")) return vm_op_mod(ms, index);
	if (!strcmp(op, "==")) return vm_op_eq(ms, index);
	if (!strcmp(op, "!=")) return vm_op_ne(ms, index);
	if (!strcmp(op, ">=")) return vm_op_geq(ms, index);
	if (!strcmp(op, "<=")) return vm_op_leq(ms, index);
	if (!strcmp(op, "--")) return vm_op_dec(ms, index);
	if (!strcmp(op, "++")) return vm_op_inc(ms, index);
	
	fputs("ERROR! Got unexpected operation `", stderr);
	fputs(op, stderr);
	fputs("`!\n", stderr);
	
	exit(2);
}
void vm_execute_bytecode(vector<char*>* bc){
	if (!silent) fputs("Executing bytecode...\n", stdout);
	
	stack<int>* ms = new stack<int>;
	
	int op_index = 0;
	int op_count = bc->size();
	while (op_index < op_count){
		char* op = (*bc)[op_index];
		op_index = vm_execute_operation(ms, *bc, op_index, op);
	}
	
	if (!silent) fputs("Finished executing bytecode!\n", stdout);
}
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


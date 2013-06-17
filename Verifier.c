/* Verifier.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ClassFileFormat.h"
#include "OpcodeSignatures.h"
#include "TraceOptions.h"
#include "MyAlloc.h"
#include "VerifierUtils.h"
#include "Verifier.h"

#define die(...) do { printf(__VA_ARGS__); fflush(stdout); exit(1); } while (0)

#define got_here printf("got here %d\n", __LINE__);

#define MAX_NUMBER_OF_SLOTS 20
#define MAX_BUFFER_SIZE 200

#define TRUE 1
#define FALSE 0 

#define sizeof_array(x,type) sizeof(x)/sizeof(type)
#define sizeof_int_array(x) sizeof_array(x,int)

unsigned int immediate_var0_codes[] = {0x1a, 0x1e, 0x22, 0x26, 0x2a, 0x3b, 0x3f, 0x43, 0x47, 0x4b};
unsigned int immediate_var1_codes[] = {0x1b, 0x1f, 0x23, 0x27, 0x2b, 0x3c, 0x40, 0x44, 0x48, 0x4c};
unsigned int immediate_var2_codes[] = {0x1c, 0x20, 0x24, 0x28, 0x2c, 0x3d, 0x41, 0x45, 0x49, 0x4d};
unsigned int immediate_var3_codes[] = {0x1d, 0x21, 0x25, 0x29, 0x2d, 0x3e, 0x42, 0x46, 0x4a, 0x4e};
unsigned int return_codes[] = {0Xac, 0Xad, 0Xae, 0Xaf, 0Xb0, 0Xb1, 0Xb2};
unsigned int branch_codes[] = {0X99, 0X9a, 0X9b, 0X9c, 0X9d, 0X9e, 0X9f, 0Xa0, 0Xa1, 0Xa2, 0Xa3, 0Xa4, 0Xa5, 0Xa6};
unsigned int invoke_codes[] = {0Xb6, 0Xb7, 0Xb8, 0Xb9, 0Xba};
unsigned int static_invoke_codes[] = {0Xb8};
unsigned int produces_reference_codes[] = {0X2a, 0X2b, 0X2c, 0X2d}; // needs more
unsigned int store_codes[] = {0X36, 0X37, 0X38, 0X39, 0X3a, 0X3b, 0X3c, 0X3d, 0X3e, 0X3f, 0X40, 0X41, 0X42, 0X43, 0X44, 0X45, 0X46, 0X47, 0X48, 0X49, 0X4a, 0X4b, 0X4c, 0X4d, 0X4e, 0X4f, 0X50, 0X51, 0X52, 0X53, 0X54, 0X55, 0X56};
char wildcard_types[][2] = {"W", "X", "Y", "Z"};

#define number_of_immediate_codes sizeof_int_array(immediate_var0_codes)

int is_in_immediate_codes(int op, unsigned int var_codes[]) {

	int i = 0;

	for (i = 0; i < number_of_immediate_codes; ++i) {

		if (op == var_codes[i]) return TRUE;
	}

	return FALSE;
}

int is_in_instruction_set(int op, unsigned int *instruction_codes, unsigned int array_size) {
	
	int i = 0;
	for (i = 0; i < array_size; ++i) {

		if (op == instruction_codes[i]) return TRUE;
	}

	return FALSE;
}

int is_branch_instruction(int op) {

	return is_in_instruction_set(op, branch_codes, sizeof_int_array(branch_codes));
}

int is_invoke_instruction(int op) {

	return is_in_instruction_set(op, invoke_codes, sizeof_int_array(invoke_codes));
}

int is_static_invoke(int op) {

	return is_in_instruction_set(op, static_invoke_codes, sizeof_int_array(static_invoke_codes));
}

int is_return_instruction(int op) {

	return is_in_instruction_set(op, return_codes, sizeof_int_array(return_codes));
}

int is_produces_reference_instruction(int op) {
	return is_in_instruction_set(op, produces_reference_codes, sizeof_int_array(produces_reference_codes)); 
}

int is_wildcard_type(char* type) {

	int i = 0;
	for (i = 0; i < sizeof_array(wildcard_types, char*); ++i) {

		if (!strcmp(type, wildcard_types[i])) return TRUE;
	}

	return FALSE;
}

int is_store_instruction(int op) {

	return is_in_instruction_set(op, store_codes, sizeof_int_array(store_codes)); 
}



// Output an array of the verifier's type descriptors
static void printTypeCodesArray( char **vstate, method_info *m, char *name ) {
    int i;
    if (name != NULL)
        fprintf(stdout, "\nMethod %s:\n", name);
    else
        fputc('\n', stdout);
    for( i = 0;  i < m->max_locals; i++ ) {
        fprintf(stdout, "  V%d:  %s\n", i, *vstate++);
    }
    for( i = 0;  i < m->max_stack; i++ )
        fprintf(stdout, "  S%d:  %s\n", i, *vstate++);
}

typedef struct {
	int	bytecodePosition;
	int	changed;
	int	stackHeight;
	char	locals[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE];
	char	stack[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE];
} deet;

void print_deet(deet* d, method_info* m) {

	int i = 0;

	printf("[%s | %lu] position: %i, changed: %i, stack height: %i\n",
			opcodes[m->code[d->bytecodePosition]].opcodeName,
			(unsigned long)d,
			d->bytecodePosition,
			d->changed,
			d->stackHeight);

	printf("\tlocals:\n");
	for (i = 0; i < m->max_locals; ++i) {
		printf("\t\t%i:	%s\n", i, d->locals[i]);
	}
	printf("\tstack:\n");
	for (i = 0; i < d->stackHeight; ++i) {
		printf("\t\t%i:	%s\n", i, d->stack[i]);
	}
}

// Deets on instructions are statically initialized to default values
void initialize_deet(int bytecodePosition, deet* d) {

	int i = 0;

	d->bytecodePosition = bytecodePosition;
	d->changed = FALSE;
	d->stackHeight = -1;

	for (i = 0; i < MAX_NUMBER_OF_SLOTS; ++i) {
		strcpy(d->locals[i], "U");
		strcpy(d->stack[i], "-");
	}
}

// First deet is special case which needs to set changed flag
void initialize_first_deet(method_info *m, char** initialTypeList, deet* d) {

	int i = 0;

	for (i = 0; i < m->max_locals; ++i) {

		strcpy(d->locals[i], initialTypeList[i]);
	}

	d->changed = TRUE;
	d->stackHeight = 0;
}

// Opcodes can branch to 1 or more possible branches
void get_next_bytecode_positions(deet* d, method_info* m, int* qs) {

	int op = m->code[d->bytecodePosition];

	// The immediate next instruction
	int numberOfAdditionalBytes = strlen(opcodes[op].inlineOperands);
	qs[0] = d->bytecodePosition + numberOfAdditionalBytes + 1;

	// Branch instructions
	if (is_branch_instruction(op)) {
		int offset = (m->code[d->bytecodePosition + 1] << 8) | (m->code[d->bytecodePosition + 2]);
		qs[1] = d->bytecodePosition + offset;
	}
}

void parse_results(char* returnType, char results[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE], int* resultCount) {
	
	switch (*returnType) {
		case 'I':
			strcpy(results[0], "I");
			*resultCount = 1;
			break;
		case 'L':
			strcpy(results[0], "L");
			strcpy(results[1], "l");
			*resultCount = 2;
			break;
		case 'D':
			strcpy(results[0], "D");
			strcpy(results[1], "d");
			*resultCount = 2;
			break;
		case 'A':
			strcpy(results[0], returnType);
			*resultCount = 1;
			break;
		case '-':
		default:
			// Void - copy nothing
			*resultCount = 0;
			break;
	}
}

void get_reference_type(deet* d, method_info* m, char results[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE]) {
	switch (m->code[d->bytecodePosition]) {
		case 0X2a: // aload_0
			strcpy(results[0], d->locals[0]);
			break;
		case 0X2b: // aload_1
			strcpy(results[0], d->locals[1]);
			break;
		case 0X2c: // aload_2
			strcpy(results[0], d->locals[2]);
			break;
		case 0X2d: // aload_3
			strcpy(results[0], d->locals[3]);
			break;
		default:
			break;
	}
}

void coolLUB(char* type1, char* type2, char*result) {

	int typesMatch;

	switch(*type1) {

		// Simple types
		case 'I': case 'D': case 'd': case 'L': case 'l':
			
			typesMatch = (strcmp(type1, type2) == 0);

			if (typesMatch) {

				strcpy(result, type1);
			}

			else {

				strcpy(result, "X");
			}

			break;

		// Reference types
		case 'A':
			if (*type2 != 'A') {

				strcpy(result, "X");
			}

			else {

				char	*type1_copy	= SafeStrdup(type1),
					*type2_copy	= SafeStrdup(type2),
					*lub		= LUB(type1_copy, type2_copy);

				strcpy(result, lub);

				if (strcmp(lub, "X")) {
					
					SafeFree(lub);
				}
			}

			break;

		default:
			strcpy(result, "X");
	}
}

int types_match(char* desired_type, char* supplied_type) {

	char result[MAX_BUFFER_SIZE];
	coolLUB(desired_type, supplied_type, result);

	return (strcmp(desired_type, result) == 0);
}

void assign_to_local(char* type, int localIndex, char locals[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE]) {

	// If the type of the local is unkown, do a strict assignment
	if (!strcmp("U", locals[localIndex])) {

		strcpy(locals[localIndex], type);
	}

	// Otherwise, write the LUB (which might return X)
	else {

		char result[MAX_BUFFER_SIZE];
		coolLUB(type, locals[localIndex], result);
		strcpy(locals[localIndex], result);
	}
}

void store(deet* d, method_info *m, char locals[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE]) {

	int op = m->code[d->bytecodePosition];

	switch (op) {
	case 0X36: // "istore",  
		break;
	case 0X37: // "lstore",  
		break;
	case 0X38: // "fstore",  
		break;
	case 0X39: // "dstore",  
		break;
	case 0X3a: // "astore",  
		break;
	case 0X3b: // "istore_0",
		assign_to_local("I", 0, locals);
		break;
	case 0X3c: // "istore_1",
		assign_to_local("I", 1, locals);
		break;
	case 0X3d: // "istore_2",
		assign_to_local("I", 2, locals);
		break;
	case 0X3e: // "istore_3",
		assign_to_local("I", 3, locals);
		break;
	case 0X3f: // "lstore_0",
		break;
	case 0X40: // "lstore_1",
		break;
	case 0X41: // "lstore_2",
		break;
	case 0X42: // "lstore_3",
		break;
	case 0X43: // "fstore_0",
		break;
	case 0X44: // "fstore_1",
		break;
	case 0X45: // "fstore_2",
		break;
	case 0X46: // "fstore_3",
		break;
	case 0X47: // "dstore_0",
		break;
	case 0X48: // "dstore_1",
		break;
	case 0X49: // "dstore_2",
		break;
	case 0X4a: // "dstore_3",
		break;
	case 0X4b: // "astore_0",
		break;
	case 0X4c: // "astore_1",
		break;
	case 0X4d: // "astore_2",
		break;
	case 0X4e: // "astore_3",
		break;
	case 0X4f: // "iastore", 
		break;
	case 0X50: // "lastore", 
		break;
	case 0X51: // "fastore", 
		break;
	case 0X52: // "dastore", 
		break;
	case 0X53: // "aastore", 
		break;
	case 0X54: // "bastore", 
		break;
	case 0X55: // "castore", 
		break;
	case 0X56: // "sastore", 
		break;
	}
}

// Verify the bytecode of one method m from class file cf
static void verifyMethod( ClassFile *cf, method_info *m ) {
    char *name = GetCPItemAsString(cf, m->name_index);
    char *retType;
    int numSlots = m->max_locals + m->max_stack;

    // initState is an array of strings, it has numSlots elements
    // retType describes the result type of this method
    char **initState = MapSigToInitState(cf, m, &retType);

    if (tracingExecution & TRACE_VERIFY)
    	printTypeCodesArray(initState, m, name);

    // Your code to verify the bytecode of the current method
    // begins here.

    // Feel free to define and use as many extra functions as
    // are appropriate for this problem.

    // Do include tracing output statements in your code
    // controlled by the tracingExecution variable to help
    // you debug the verification algorithm.
    
    // Scratch variables
    int i = 0;

    // Initialize D to <0, 1, initial stack height, initial typecode list>;
    deet* deets = (deet*)malloc(sizeof(deet) * m->code_length);
    for (i = 0; i < m->code_length; ++i) {
	    initialize_deet(i, deets + i);
    }

    initialize_first_deet(m, initState, deets);

    // while D contains an entry with a set change bit do
    int indexOfChangedDeet = -1;

    for (;;) {

	    // Look for a changed deet
	    int indexOfDeet = 0;
	    indexOfChangedDeet = -1;

	    for (indexOfDeet = 0; indexOfDeet < m->code_length; ++indexOfDeet) {

		    if ((deets + indexOfDeet)->changed) {

			    indexOfChangedDeet = indexOfDeet;
			    break;
		    }
	    }

	    if (indexOfChangedDeet < 0) break; // No changed deet - we're done!
	    deet* d = (deets + indexOfChangedDeet);

	    // set change bit to 0 in this entry;
	    d->changed = FALSE;
	    print_deet(d, m);

	    // p = bytecode position in this entry;
	    int p = d->bytecodePosition;

	    // h = stack height in this entry;
	    int h = d->stackHeight;

	    // t = typecode list in this entry;
	    char	locals[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE],
			stack[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE];

	    for (i = 0; i < m->max_locals; ++i) strcpy(locals[i], d->locals[i]);
	    for (i = 0; i < h; ++i) strcpy(stack[i], d->stack[i]);

	    // op = opcode at position p in byecode;
	    unsigned int op = m->code[p];

	    // if op comtains any immediate operands then
	    //         check that any operands which are indexes are in range;
	    if (is_in_immediate_codes(op, immediate_var0_codes))
		    if (m->max_locals < 1) die("required local 0 and there are only %i locals\n", m->max_locals);
	    if (is_in_immediate_codes(op, immediate_var1_codes))
		    if (m->max_locals < 2) die("required local 1 and there are only %i locals\n", m->max_locals);
	    if (is_in_immediate_codes(op, immediate_var2_codes))
		    if (m->max_locals < 3) die("required local 2 and there are only %i locals\n", m->max_locals);
	    if (is_in_immediate_codes(op, immediate_var3_codes))
		    if (m->max_locals < 4) die("required local 3 and there are only %i locals\n", m->max_locals);

	    // Get the lhs and rhs for the operator
	    char	operands[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE],
			results[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE];
	    int		operandCount, resultsCount;

	    // Invoke instructions:
	    if (is_invoke_instruction(op)) {

		    char	*retType		= NULL;
		    int 	invokeTarget		= (m->code[p+1] << 8) | (m->code[p+2]);
		    char	**invokeAnalysisResult	= AnalyzeInvoke(cf, invokeTarget, is_static_invoke(op), &retType, &operandCount);

		    // Parse operands
		    for (i = 0; i < operandCount; ++i) {

			    strcpy(operands[i], invokeAnalysisResult[i]);
		    }

		    // Parse results
		    parse_results(retType, results, &resultsCount);
		    FreeTypeDescriptor(retType);
	    }

	    // Regular instructions:
	    else {
		    char	signature[MAX_BUFFER_SIZE],
				lhs[MAX_BUFFER_SIZE] = {'\0'},
				rhs[MAX_BUFFER_SIZE] = {'\0'};
		    int		lhsSize;

		    // Copy the signature for manipulation
		    strcpy(signature, opcodes[op].signature);

		    // Copy the lhs characters
		    // Since we're reading these from the opcodes, each stack value should be one character (reference is just an A)
		    lhsSize = strcspn(signature, ">");
		    strncpy(lhs, signature, lhsSize);
		    for (i = 0; i < lhsSize; ++i) {
			    sprintf(operands[i], "%c", lhs[0]);
		    }
		    operandCount = lhsSize;

		    // Copy the rhs characters. Also one character to one stack value.
		    strcpy(rhs, (signature + lhsSize + 1));
		    parse_results(rhs, results, &resultsCount);

		    if (is_produces_reference_instruction(op)) {
		    	get_reference_type(d, m, results);
			resultsCount = 1;
		    }
	    }

	    // foreach stack operand accessed by op do
	    int operandIndex;
	    for (operandIndex = 0; operandIndex < operandCount; ++operandIndex) {

		   // decrement h and check that stack does not underflow;
		   --h;
		   if (h < 0) die("stack underflow\n");

		    // pop stack t and check datatype compatibility of that operand;
		    if (!is_wildcard_type(operands[operandIndex])) {

			    if (!types_match(operands[operandIndex], stack[h]))
				    die("type mismatch when popping stack (expected %s, got %s)\n", operands[operandIndex], stack[h]);

			    // TODO check instruction results
			    // TODO reference store instructions
		    }
	    }

	    // foreach stack result generated by op do
	    for (operandIndex = 0; operandIndex < resultsCount; ++operandIndex) {

		    // push typecode of result onto the stack t;
		    sprintf(stack[h], "%s", results[operandIndex]);

		    // increment h and check that stack does not overflow;
		    ++h;
		    if (h > m->max_stack) die("stack overflow (%i/%i with %s)\n", h, m->max_stack, opcodes[op].opcodeName);
	    }

	    // Check the results of store instructions
	    if (is_store_instruction(op)) {

		    store(d, m, locals);
	    }

	    // only check next position if instruction is not a return
	    if (is_return_instruction(op)) continue;

	    // for q = the bytecode position of each instruction that
	    //         can execute immediately after op at position p do
	    int q, qIndex, qs[] = {-1, -1};
	    get_next_bytecode_positions(d, m, qs);
	    for (qIndex = 0; qIndex < sizeof(qs) / sizeof(int); ++qIndex) {

		    q = qs[qIndex];
		    if (q < 0) break;

		    // if any incompatibilities were found then
		    //         report an error;
		    int	previousH		= deets[q].stackHeight,
		        stackHeightWasSet	= (previousH >= 0);

		    if (stackHeightWasSet && previousH != h) die("stack mismatch (old: %i; new: %i; at %i)\n", previousH, h, p);
		    // TODO check types

		    // merge h and t with the entry in D, updating that entry;
		    deets[q].stackHeight = h;

		    for (i = 0; i < m->max_locals; ++i)	strcpy(deets[q].locals[i], locals[i]);
		    for (i = 0; i < h; ++i)		strcpy(deets[q].stack[i], stack[i]);

		    // if anything changed in that entry then
		    //         set its change bit;
		    if (!stackHeightWasSet) {

			    deets[q].changed = TRUE;
		    }
	    }
    }

    free(deets);
    FreeTypeDescriptorArray(initState, numSlots);
    SafeFree(name);
}


// Verify the bytecode of all methods in class file cf
void Verify( ClassFile *cf ) {
    int i;

    for( i = 0;  i < cf->methods_count;  i++ ) {
        method_info *m = &(cf->methods[i]);
	    verifyMethod(cf, m);
    }
    if (tracingExecution & TRACE_VERIFY)
    	fprintf(stdout, "Verification of class %s completed\n\n", cf->cname);
}


// Initialize this module
void InitVerifier(void) {
#ifndef NDEBUG
    // perform integrity check on the opcode table
    CheckOpcodeTable();
#endif
    // any initialization of local data structures can go here
}

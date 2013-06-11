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

unsigned int immediate_var0_codes[] = {0x1a, 0x1e, 0x22, 0x26, 0x2a, 0x3b, 0x3f, 0x43, 0x47, 0x4b};
unsigned int immediate_var1_codes[] = {0x1b, 0x1f, 0x23, 0x27, 0x2b, 0x3c, 0x40, 0x44, 0x48, 0x4c};
unsigned int immediate_var2_codes[] = {0x1c, 0x20, 0x24, 0x28, 0x2c, 0x3d, 0x41, 0x45, 0x49, 0x4d};
unsigned int immediate_var3_codes[] = {0x1d, 0x21, 0x25, 0x29, 0x2d, 0x3e, 0x42, 0x46, 0x4a, 0x4e};
unsigned int return_codes[] = {0Xac, 0Xad, 0Xae, 0Xaf, 0Xb0, 0Xb1, 0Xb2};

#define number_of_immediate_codes sizeof(immediate_var0_codes)/sizeof(int)

int is_in_immediate_codes(int op, unsigned int var_codes[]) {

	int i = 0;

	for (i = 0; i < number_of_immediate_codes; ++i) {

		if (op == var_codes[i]) return TRUE;
	}

	return FALSE;
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

	printf("[deet %lu] position: %i, name: %s, changed: %i, stack height: %i\n",
			(unsigned long)d,
			d->bytecodePosition,
			opcodes[m->code[d->bytecodePosition]].opcodeName,
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

void initialize_deet(int bytecodePosition, deet* d) {

	int i = 0;

	d->bytecodePosition = bytecodePosition;
	d->changed = FALSE;
	d->stackHeight = -1;

	for (i = 0; i < MAX_NUMBER_OF_SLOTS; ++i) {
		strcpy(d->locals[i], "-");
		strcpy(d->stack[i], "-");
	}
}

void initialize_first_deet(method_info *m, char** initialTypeList, deet* d) {

	int i = 0;

	for (i = 0; i < m->max_locals; ++i) {

		strcpy(d->locals[i], initialTypeList[i]);
	}

	d->changed = TRUE;
	d->stackHeight = 0;
}

void get_next_bytecode_positions(int bytecodePosition, int op, int* qs) {

	// TODO branches
	
	int numberOfAdditionalBytes = strlen(opcodes[op].inlineOperands);
	qs[0] = bytecodePosition + numberOfAdditionalBytes + 1;
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
    
    // Sratch variables
    int i = 0;

    // Initialize D to <0, 1, intial stack height, initial typecode list>;
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

	    // p = bytecode position in this entry;

	    // h = stack height in this entry;
	    int h = d->stackHeight;

	    // t = typecode list in this entry;
	    char	locals[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE],
			stack[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE];

	    for (i = 0; i < m->max_locals; ++i) strcpy(locals[i], d->locals[i]);
	    for (i = 0; i < h; ++i) strcpy(stack[i], d->stack[i]);

	    // op = opcode at position p in byecode;
	    unsigned int op = m->code[d->bytecodePosition];

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
	    char	signature[MAX_BUFFER_SIZE],
			lhs[MAX_BUFFER_SIZE],
			rhs[MAX_BUFFER_SIZE];
	    int		lhsSize,
			rhsSize;

	    // Copy the signature for manipulation
	    strcpy(signature, opcodes[op].signature);

	    // Copy the lhs characters
	    lhsSize = strcspn(signature, ">");
	    strncpy(lhs, signature, lhsSize);
	    lhs[lhsSize] = '\0';

	    // Copy the rhs characters
	    rhsSize = strlen(signature) - lhsSize - 1;
	    strcpy(rhs, (signature + lhsSize + 1));

	    // foreach stack operand accessed by op do
	    int operandIndex;
	    for (operandIndex = 0; operandIndex < lhsSize; ++operandIndex) {

		   // decrement h and check that stack does not underflow;
		   --h;
		   if (h < 0) die("stack underflow\n");

		    // pop stack t and check datatype compatibility of that operand;
	    }

	    // foreach stack result generated by op do
	    for (operandIndex = 0; operandIndex < rhsSize; ++operandIndex) {

		    // push typecode of result onto the stack t;
		    sprintf(stack[h], "%c", rhs[operandIndex]);

		    // increment h and check that stack does not overflow;
		    ++h;
		    if (h > m->max_stack) die("stack overflow (%i/%i)\n", h, m->max_stack);
	    }

	    // only check next position if instruction is not a return
	    int isReturnCode = FALSE;
	    for (i = 0; i < sizeof(return_codes)/sizeof(int); ++i) {
		    
		    if (op == return_codes[i]) {
			    
			    isReturnCode = TRUE;
			    break;
		    }
	    }

	    if (isReturnCode) continue;

	    // for q = the bytecode position of each instruction that
	    //         can execute immediately after op at position p do
	    int q, qIndex, qs[] = {-1, -1};
	    get_next_bytecode_positions(d->bytecodePosition, op, qs);
	    for (qIndex = 0; qIndex < sizeof(qs); ++qIndex) {

		    q = qs[qIndex];
		    if (q < 0) break;

		    // if any incompatibilities were found then
		    //         report an error;
		    int	previousH		= deets[q].stackHeight,
		        stackHeightWasSet	= (previousH >= 0);

		    if (stackHeightWasSet && previousH != h) die("stack mismatch\n");
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

	    print_deet(d, m);
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

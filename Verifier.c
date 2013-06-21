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
#include "Utils.h"
#include "VerifierData.h"
#include "InstructionUtils.h"
#include "StoreInstructions.h"
#include "LoadInstructions.h"
#include "TypeUtils.h"

unsigned int immediate_var0_codes[] = {0x1a, 0x1e, 0x22, 0x26, 0x2a, 0x3b, 0x3f, 0x43, 0x47, 0x4b};
unsigned int immediate_var1_codes[] = {0x1b, 0x1f, 0x23, 0x27, 0x2b, 0x3c, 0x40, 0x44, 0x48, 0x4c};
unsigned int immediate_var2_codes[] = {0x1c, 0x20, 0x24, 0x28, 0x2c, 0x3d, 0x41, 0x45, 0x49, 0x4d};
unsigned int immediate_var3_codes[] = {0x1d, 0x21, 0x25, 0x29, 0x2d, 0x3e, 0x42, 0x46, 0x4a, 0x4e};
unsigned int return_codes[] = {0Xac, 0Xad, 0Xae, 0Xaf, 0Xb0, 0Xb1};
unsigned int branch_codes[] = {0X99, 0X9a, 0X9b, 0X9c, 0X9d, 0X9e, 0X9f, 0Xa0, 0Xa1, 0Xa2, 0Xa3, 0Xa4, 0Xa5, 0Xa6, 0xc6, 0xc7};
unsigned int invoke_codes[] = {0Xb6, 0Xb7, 0Xb8, 0Xb9, 0Xba};
unsigned int static_invoke_codes[] = {0Xb8};
unsigned int produces_reference_codes[] = {0X2a, 0X2b, 0X2c, 0X2d}; // needs more

#define number_of_immediate_codes sizeof_int_array(immediate_var0_codes)

int is_in_immediate_codes(int op, unsigned int var_codes[]) {

	int i = 0;

	for (i = 0; i < number_of_immediate_codes; ++i) {

		if (op == var_codes[i]) return TRUE;
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

// Opcodes can branch to 0, 1, or 2 instructions
void get_next_bytecode_positions(deet* d, method_info* m, int* qs) {

	int op = m->code[d->bytecodePosition];

	// Return statements have no next instructions
	if (is_return_instruction(op)) return;

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
		case 'F':
			strcpy(results[0], "F");
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

int nextBytecode_i(deet* d, method_info* m) {

	return m->code[d->bytecodePosition + 1];
}

int nextBytecode_ii(deet* d, method_info* m) {

	return (m->code[d->bytecodePosition + 1] << 8) | (m->code[d->bytecodePosition + 2]);
}


// Verify the bytecode of one method m from class file cf
static void verifyMethod( ClassFile *cf, method_info *m ) {
	printf("\n\nstarting method verification\n");
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
	    //print_deet(d, m);

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
		    int 	invokeTarget		= nextBytecode_ii(d, m);
		    char	**invokeAnalysisResult	= AnalyzeInvoke(cf, invokeTarget, is_static_invoke(op), &retType, &operandCount);

		    // Parse operands
		    for (i = 0; i < operandCount; ++i) {

			    strcpy(operands[i], invokeAnalysisResult[i]);
		    }

		    // Parse results
		    parse_results(retType, results, &resultsCount);
		    FreeTypeDescriptor(retType);
	    }

	    // Execute (and validate) load instructions
	    else if (is_load_instruction(op)) {

		    load(d, m, locals, stack, operands, &operandCount, results, &resultsCount);
	    }

	    // Store
	    else if (is_store_instruction(op)) {

		    store(d, m, locals, stack, h, operands, &operandCount, results, &resultsCount);
	    }

	    // TODO handle returns better

	    // getstatic >X
	    // TODO putstatic, putfield, getfield
	    else if (op == 0Xb2) {

		    operandCount = 0;

		    int index = nextBytecode_ii(d, m);
		    parse_results(FieldTypeCode(cf, index), results, &resultsCount);
	    }

	   // ldc
	   // TODO ldc_w, ldc2_w
	    else if (op == 0x12) {

		    operandCount = 0;

		    int index = nextBytecode_i(d, m);
		    char typeCode[MAX_BUFFER_SIZE];
		    get_cpitem_type(cf, index, typeCode);
		    parse_results(typeCode, results, &resultsCount);
	    }

	    // new
	    // TODO newarray, anewarray
	    else if (op == 0Xbb) {

		    operandCount = 0;
		    resultsCount = 1;

		    char *typeCode = GetCPItemAsString(cf, nextBytecode_ii(d, m));
		    sprintf(results[0], "AL%s", typeCode);
		    if (typeCode) SafeFree(typeCode);
	    }

	    // dup
	    // TODO dup_x1, dup_x2, dup2, dup2_x1, dup2_x2
	    else if (op == 0X59) {

		    operandCount = 1;
		    resultsCount = 2;

		    if (h <= 0) die("no stack to duplicate\n");

		    strcpy(operands[0], "*");
		    strcpy(results[0], stack[h-1]);
		    strcpy(results[1], stack[h-1]);
	    }

	    // pop
	    else if (op == 0X57) {

		    operandCount = 1;
		    resultsCount = 0;

		    strcpy(operands[0], "*");
	    }

	    // pop2
	    else if (op == 0X58) {

		    operandCount = 2;
		    resultsCount = 0;

		    strcpy(operands[0], "*");
		    strcpy(operands[1], "*");

	    }

	    // swap
	    else if (op == 0X5f) {

		    operandCount = 2;
		    resultsCount = 2;

		    if (h <= 1) die("not enough stack to swap\n");

		    strcpy(operands[0], stack[h-2]);
		    strcpy(operands[1], stack[h-1]);
		    strcpy(results[0], stack[h-1]);
		    strcpy(results[1], stack[h-2]);

	    }

	    // athrow
	    else if (op == 0Xb0) {

		    operandCount = 1;
		    resultsCount = 0;

		    strcpy(operands[0], "ALjava/lang/Object");
	    }

	    // TODO wide :(
	    // TODO multianewarray
	    // TODO jsr_w

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
		    // TODO think about instructions which take *any* reference type (arraylength, instanceof, checkcast, monitor*, ifnull, ifnonnull)
		    lhsSize = strcspn(signature, ">");
		    strncpy(lhs, signature, lhsSize);
		    for (i = 0; i < lhsSize; ++i) {
			    sprintf(operands[i], "%c", lhs[0]);
		    }
		    operandCount = lhsSize;

		    // Copy the rhs characters. Also one character to one stack value.
		    // TODO think about instructions which produce *any* reference type
		    strcpy(rhs, (signature + lhsSize + 1));
		    parse_results(rhs, results, &resultsCount);
	    }

	    // foreach stack operand accessed by op do
	    for (i = operandCount - 1; i >= 0; --i) {

		   // decrement h and check that stack does not underflow;
		   --h;
		   if (h < 0) die("stack underflow\n");

		    // pop stack t and check datatype compatibility of that operand;
		    if (strcmp(operands[i], "*")) { // TODO is * what we want for the wildcard type?

			    if (!types_match(operands[i], stack[h]))
				    die("type mismatch when popping stack (expected %s, got %s)\n", operands[i], stack[h]);
		    }
	    }

	    // foreach stack result generated by op do
	    for (i = 0; i < resultsCount; ++i) {

		    // push typecode of result onto the stack t;
		    sprintf(stack[h], "%s", results[i]);

		    // increment h and check that stack does not overflow;
		    ++h;
		    if (h > m->max_stack) die("stack overflow (%i/%i with %s)\n", h, m->max_stack, opcodes[op].opcodeName);
	    }

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
	

		    char result[MAX_BUFFER_SIZE];
		    if (strcmp(deets[q].locals[i],"U")) { // local is defined
			    for (i = 0;i < m->max_locals; ++i) {
				coolLUB(locals[i], deets[q].locals[i], result);
				
				if (!strcmp(result,"X")) die("locals mismatch on merge: local:%s; deet.local:%s\n", locals[i], deets[q].locals[i]);

				if (strcmp(result, deets[q].locals[i])) {
					deets[q].changed = TRUE;
					strcpy(deets[q].locals[i], result);
				}
			    }
		    }
		    else {
			strcpy(deets[q].locals[i], locals[i]);
			deets[q].changed = TRUE;
		    }

		    if (strcmp(deets[q].stack[i],"-")) { // stack is defined
			    for (i = 0; i < h; ++i) {
				coolLUB(stack[i], deets[q].stack[i], result);

				if (!strcmp(result, "X")) die("stack mismatch on merge: stack:%s; deet.stack:%s\n", stack[i], deets[q].stack[i]);

				if (strcmp(result, deets[q].stack[i])) {
					deets[q].changed = TRUE;
					strcpy(deets[q].stack[i], result);
				}
			    }
		    }
		    else {
			deets[q].changed = TRUE;
			strcpy(deets[q].stack[i], result);
		    }

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

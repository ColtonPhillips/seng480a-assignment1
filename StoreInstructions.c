#include "StoreInstructions.h"
#include "InstructionUtils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

unsigned int store_codes[] = {0X36, 0X37, 0X38, 0X39, 0X3a, 0X3b, 0X3c, 0X3d, 0X3e, 0X3f, 0X40, 0X41, 0X42, 0X43, 0X44, 0X45, 0X46, 0X47, 0X48, 0X49, 0X4a, 0X4b, 0X4c, 0X4d, 0X4e};

int is_store_instruction(int op) {

	return is_in_instruction_set(op, store_codes, sizeof_int_array(store_codes)); 
}

void store(
		deet* d, method_info* m,
		char locals[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE],
		char stack[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE],
		int stackHeight,
		char operands[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE],
		int* operandCount,
		char results[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE],
		int* resultsCount) {

	*resultsCount = 0;

	int	op		= m->code[d->bytecodePosition],
	    	localIndex	= -1;

	switch (op) {
	case 0X36: // "istore",  
	case 0X37: // "lstore",  
	case 0X38: // "fstore",  
	case 0X39: // "dstore",  
	case 0X3a: // "astore",  
		localIndex = m->code[d->bytecodePosition + 1];
		break;

	case 0X3b: // "istore_0",
	case 0X3f: // "lstore_0",
	case 0X43: // "fstore_0",
	case 0X47: // "dstore_0",
	case 0X4b: // "astore_0",
		localIndex = 0;
		break;

	case 0X3c: // "istore_1",
	case 0X40: // "lstore_1",
	case 0X44: // "fstore_1",
	case 0X48: // "dstore_1",
	case 0X4c: // "astore_1",
		localIndex = 1;
		break;

	case 0X3d: // "istore_2",
	case 0X41: // "lstore_2",
	case 0X45: // "fstore_2",
	case 0X49: // "dstore_2",
	case 0X4d: // "astore_2",
		localIndex = 2;
		break;

	case 0X3e: // "istore_3",
	case 0X42: // "lstore_3",
	case 0X46: // "fstore_3",
	case 0X4a: // "dstore_3",
	case 0X4e: // "astore_3",
		localIndex = 3;
		break;
	}

	switch (op) {
	case 0X36: // "istore",  
	case 0X3b: // "istore_0",
	case 0X3c: // "istore_1",
	case 0X3d: // "istore_2",
	case 0X3e: // "istore_3",
		strcpy(locals[localIndex], "I");
		*operandCount = 1;
		strcpy(operands[0], "I");
		break;

	case 0X37: // "lstore",  
	case 0X3f: // "lstore_0",
	case 0X40: // "lstore_1",
	case 0X41: // "lstore_2",
	case 0X42: // "lstore_3",
		strcpy(locals[localIndex], "Ll");
		*operandCount = 2;
		strcpy(operands[0], "L");
		strcpy(operands[1], "l");
		break;

	case 0X38: // "fstore",  
	case 0X43: // "fstore_0",
	case 0X44: // "fstore_1",
	case 0X45: // "fstore_2",
	case 0X46: // "fstore_3",
		strcpy(locals[localIndex], "F");
		*operandCount = 1;
		strcpy(operands[0], "F");
		break;

	case 0X39: // "dstore",  
	case 0X47: // "dstore_0",
	case 0X48: // "dstore_1",
	case 0X49: // "dstore_2",
	case 0X4a: // "dstore_3",
		strcpy(locals[localIndex], "Dd");
		*operandCount = 2;
		strcpy(operands[0], "D");
		strcpy(operands[1], "d");
		break;

	case 0X3a: // "astore",  
	case 0X4b: // "astore_0",
	case 0X4c: // "astore_1",
	case 0X4d: // "astore_2",
	case 0X4e: // "astore_3",
		if (stackHeight <= 0) die("No reference to store");
		*operandCount = 1;
		strcpy(locals[localIndex], stack[stackHeight - 1]);
		strcpy(operands[0], "ALjava/lang/Object");
		break;
	}
}

#include "LoadInstructions.h"
#include "InstructionUtils.h"
#include "Utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "TypeUtils.h"

unsigned int load_codes[] = {0X15, 0X16, 0X17, 0X18, 0X19, 0X1a, 0X1b, 0X1c, 0X1d, 0X1e, 0X1f, 0X20, 0X21, 0X22, 0X23, 0X24, 0X25, 0X26, 0X27, 0X28, 0X29, 0X2a, 0X2b, 0X2c, 0X2d, 0X2e, 0X2f, 0X30, 0X31, 0X32, 0X33, 0X34, 0X35};

int is_load_instruction(int op) {

	return is_in_instruction_set(op, load_codes, sizeof_int_array(load_codes)); 
}

void load(
		deet* d, method_info* m,
		char locals[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE],
		char stack[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE],
		char operands[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE],
		int* operandCount,
		char results[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE],
		int* resultsCount) {

	int	localIndex	= -1,
		op		= m->code[d->bytecodePosition];
	char	expectedType[MAX_BUFFER_SIZE];

	*operandCount = 0;

	switch (op) {
	case 0X1a: // "iload_0"
	case 0X1e: // "lload_0"
	case 0X22: // "fload_0"
	case 0X26: // "dload_0"
	case 0X2a: // "aload_0"
		localIndex = 0;
		break;

	case 0X1b: // "iload_1"
	case 0X1f: // "lload_1"
	case 0X23: // "fload_1"
	case 0X27: // "dload_1"
	case 0X2b: // "aload_1"
		localIndex = 1;
		break;

	case 0X1c: // "iload_2"
	case 0X20: // "lload_2"
	case 0X24: // "fload_2"
	case 0X28: // "dload_2"
	case 0X2c: // "aload_2"
		localIndex = 2;
		break;

	case 0X1d: // "iload_3"
	case 0X21: // "lload_3"
	case 0X25: // "fload_3"
	case 0X29: // "dload_3"
	case 0X2d: // "aload_3"
		localIndex = 3;
		break;

	case 0X15: // "iload", 
	case 0X16: // "lload", 
	case 0X17: // "fload", 
	case 0X18: // "dload", 
	case 0X19: // "aload", 
		// TODO load from next byte(s)
		die("unsupported load\n");
		break;

	case 0X2e: // "iaload",
	case 0X2f: // "laload",
	case 0X30: // "faload",
	case 0X31: // "daload",
	case 0X32: // "aaload",
	case 0X33: // "baload",
	case 0X34: // "caload",
	case 0X35: // "saload",
		// TODO who knows
		die("unsupported load\n");
		break;

	}

	switch (op) {
	case 0X15: // "iload", 
	case 0X1a: // "iload_0"
	case 0X1b: // "iload_1"
	case 0X1c: // "iload_2"
	case 0X1d: // "iload_3"
		strcpy(expectedType, "I");
		*resultsCount = 1;
		strcpy(results[0], "I");
		break;

	case 0X16: // "lload", 
	case 0X1e: // "lload_0"
	case 0X1f: // "lload_1"
	case 0X20: // "lload_2"
	case 0X21: // "lload_3"
		strcpy(expectedType, "Ll");
		*resultsCount = 2;
		strcpy(results[0], "L");
		strcpy(results[1], "l");
		break;

	case 0X17: // "fload", 
	case 0X22: // "fload_0"
	case 0X23: // "fload_1"
	case 0X24: // "fload_2"
	case 0X25: // "fload_3"
		strcpy(expectedType, "F");
		*resultsCount = 1;
		strcpy(results[0], "F");
		break;

	case 0X18: // "dload", 
	case 0X26: // "dload_0"
	case 0X27: // "dload_1"
	case 0X28: // "dload_2"
	case 0X29: // "dload_3"
		strcpy(expectedType, "Dd");
		*resultsCount = 2;
		strcpy(results[0], "D");
		strcpy(results[1], "d");

		break;

	case 0X19: // "aload", 
	case 0X2a: // "aload_0"
	case 0X2b: // "aload_1"
	case 0X2c: // "aload_2"
	case 0X2d: // "aload_3"
		strcpy(expectedType, "ALjava/lang/Object");
		*resultsCount = 1;
		strcpy(results[0], locals[localIndex]);
		break;

	case 0X2e: // "iaload",
	case 0X2f: // "laload",
	case 0X30: // "faload",
	case 0X31: // "daload",
	case 0X32: // "aaload",
	case 0X33: // "baload",
	case 0X34: // "caload",
	case 0X35: // "saload",
		die("unsupported load instruction\n");
		break;
	}

	if (!types_match(expectedType, locals[localIndex]))
		die("type mismatch when retrieving local (expected %s, was %s)\n", expectedType, locals[localIndex]);
}

/*
switch (op) {
	case 0X15: // "iload", 
		break;
	case 0X16: // "lload", 
		break;
	case 0X17: // "fload", 
		break;
	case 0X18: // "dload", 
		break;
	case 0X19: // "aload", 
		break;
	case 0X1a: // "iload_0"
		break;
	case 0X1b: // "iload_1"
		break;
	case 0X1c: // "iload_2"
		break;
	case 0X1d: // "iload_3"
		break;
	case 0X1e: // "lload_0"
		break;
	case 0X1f: // "lload_1"
		break;
	case 0X20: // "lload_2"
		break;
	case 0X21: // "lload_3"
		break;
	case 0X22: // "fload_0"
		break;
	case 0X23: // "fload_1"
		break;
	case 0X24: // "fload_2"
		break;
	case 0X25: // "fload_3"
		break;
	case 0X26: // "dload_0"
		break;
	case 0X27: // "dload_1"
		break;
	case 0X28: // "dload_2"
		break;
	case 0X29: // "dload_3"
		break;
	case 0X2a: // "aload_0"
		break;
	case 0X2b: // "aload_1"
		break;
	case 0X2c: // "aload_2"
		break;
	case 0X2d: // "aload_3"
		break;
	case 0X2e: // "iaload",
		break;
	case 0X2f: // "laload",
		break;
	case 0X30: // "faload",
		break;
	case 0X31: // "daload",
		break;
	case 0X32: // "aaload",
		break;
	case 0X33: // "baload",
		break;
	case 0X34: // "caload",
		break;
	case 0X35: // "saload",
		break;
	}
*/

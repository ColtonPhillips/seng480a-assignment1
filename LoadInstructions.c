#include "LoadInstructions.h"
#include "InstructionUtils.h"
#include "Utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

unsigned int load_codes[] = {0X15, 0X16, 0X17, 0X18, 0X19, 0X1a, 0X1b, 0X1c, 0X1d, 0X1e, 0X1f, 0X20, 0X21, 0X22, 0X23, 0X24, 0X25, 0X26, 0X27, 0X28, 0X29, 0X2a, 0X2b, 0X2c, 0X2d, 0X2e, 0X2f, 0X30, 0X31, 0X32, 0X33, 0X34, 0X35};

int is_load_instruction(int op) {

	return is_in_instruction_set(op, load_codes, sizeof_int_array(load_codes)); 
}

void load(deet* d, method_info* m, char locals[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE], char stack[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE]) {

	int	typesMatch	= TRUE,
		op		= m->code[d->bytecodePosition];

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
		typesMatch = !strcmp(locals[1], "F");
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

	if (!typesMatch) die("type mismatch when retrieving local\n");
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

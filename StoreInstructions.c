#include "StoreInstructions.h"
#include "InstructionUtils.h"

unsigned int store_codes[] = {0X36, 0X37, 0X38, 0X39, 0X3a, 0X3b, 0X3c, 0X3d, 0X3e, 0X3f, 0X40, 0X41, 0X42, 0X43, 0X44, 0X45, 0X46, 0X47, 0X48, 0X49, 0X4a, 0X4b, 0X4c, 0X4d, 0X4e, 0X4f, 0X50, 0X51, 0X52, 0X53, 0X54, 0X55, 0X56};

int is_store_instruction(int op) {

	return is_in_instruction_set(op, store_codes, sizeof_int_array(store_codes)); 
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

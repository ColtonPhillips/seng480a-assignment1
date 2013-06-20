/* LoadInstructions.h */

#ifndef VERIFYLOADH
#define VERIFYLOADH

#include "ClassFileFormat.h"  // for ClassFile
#include "VerifierData.h"

int is_load_instruction(int op);
void load(
		deet* d, method_info* m,
		char locals[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE],
		char stack[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE],
		char operands[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE],
		int* operandCount,
		char results[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE],
		int* resultsCount);

#endif

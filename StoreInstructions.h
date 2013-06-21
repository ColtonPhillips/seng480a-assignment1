/* VerifyStore.h */

#ifndef VERIFYSTOREH
#define VERIFYSTOREH

#include "ClassFileFormat.h"  // for ClassFile
#include "VerifierData.h"

int is_store_instruction(int op);
void store(
		deet* d, method_info* m,
		char locals[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE],
		char stack[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE],
		int stackHeight,
		char operands[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE],
		int* operandCount,
		char results[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE],
		int* resultsCount);


#endif

#include "InstructionUtils.h"
#include <string.h>
#include "TypeUtils.h"

int is_in_instruction_set(int op, unsigned int *instruction_codes, unsigned int array_size) {
	
	int i = 0;
	for (i = 0; i < array_size; ++i) {

		if (op == instruction_codes[i]) return TRUE;
	}

	return FALSE;
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

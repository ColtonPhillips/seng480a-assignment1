// Tom Gibson
// Colton Phillips
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

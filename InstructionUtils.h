/* InstructionUtils.h */

#ifndef INSTRUCTIONUTILSH
#define INSTRUCTIONUTILSH

#include "ClassFileFormat.h"  // for ClassFile
#include "Utils.h"

int is_in_instruction_set(int op, unsigned int *instruction_codes, unsigned int array_size);
void assign_to_local(char* type, int localIndex, char locals[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE]);

#endif

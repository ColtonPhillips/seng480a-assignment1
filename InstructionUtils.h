// Tom Gibson
// Colton Phillips
/* InstructionUtils.h */

#ifndef INSTRUCTIONUTILSH
#define INSTRUCTIONUTILSH

#include "ClassFileFormat.h"  // for ClassFile
#include "Utils.h"

int is_in_instruction_set(int op, unsigned int *instruction_codes, unsigned int array_size);

#endif

// Tom Gibson
// Colton Phillips
/* VerifyData.h */

#ifndef VERIFYDATAH
#define VERIFYDATAH

#include "ClassFileFormat.h"  // for ClassFile and method_info type definitions
#include "Utils.h"

typedef struct {
	int	bytecodePosition;
	int	changed;
	int	stackHeight;
	char	locals[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE];
	char	stack[MAX_NUMBER_OF_SLOTS][MAX_BUFFER_SIZE];
} deet;


void print_deet(deet* d, method_info* m);
void initialize_deet(int bytecodePosition, deet* d);
void initialize_first_deet(method_info *m, char** initialTypeList, deet* d);

#endif

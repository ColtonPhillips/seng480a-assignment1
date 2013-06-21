// Tom Gibson
// Colton Phillips
/* VerifierData.c */
#include "VerifierData.h"
#include <stdio.h>
#include "OpcodeSignatures.h"
#include <string.h>

void print_deet(deet* d, method_info* m) {

	int i = 0;

	printf("[%s] position: %i, changed: %i, stack height: %i\n",
			opcodes[m->code[d->bytecodePosition]].opcodeName,
			d->bytecodePosition,
			d->changed,
			d->stackHeight);

	printf("\tlocals:\n");
	for (i = 0; i < m->max_locals; ++i) {
		printf("\t\t%i:	%s\n", i, d->locals[i]);
	}
	printf("\tstack:\n");
	for (i = 0; i < d->stackHeight; ++i) {
		printf("\t\t%i:	%s\n", i, d->stack[i]);
	}
}

// Deets on instructions are statically initialized to default values
void initialize_deet(int bytecodePosition, deet* d) {

	int i = 0;

	d->bytecodePosition = bytecodePosition;
	d->changed = FALSE;
	d->stackHeight = -1;

	for (i = 0; i < MAX_NUMBER_OF_SLOTS; ++i) {
		strcpy(d->locals[i], "U");
		strcpy(d->stack[i], "-");
	}
}

// First deet is special case which needs to set changed flag
void initialize_first_deet(method_info *m, char** initialTypeList, deet* d) {

	int i = 0;

	for (i = 0; i < m->max_locals; ++i) {

		strcpy(d->locals[i], initialTypeList[i]);
	}

	d->changed = TRUE;
	d->stackHeight = 0;
}

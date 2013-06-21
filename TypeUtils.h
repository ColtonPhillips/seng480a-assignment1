/* TypeUtils.h */

#ifndef TYPEUTILSH
#define TYPEUTILSH

#include "Utils.h"
#include "ClassFileFormat.h"

void coolLUB(char* type1, char* type2, char*result);
int types_match(char* desired_type, char* supplied_type);
void get_cpitem_type(ClassFile* cf, int index, char result[MAX_BUFFER_SIZE]);
void get_primitive_type(int type, char result[MAX_BUFFER_SIZE]);

#endif

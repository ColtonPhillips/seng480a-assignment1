#include "TypeUtils.h"
#include <string.h>
#include "VerifierUtils.h"
#include "MyAlloc.h"
#include "Utils.h"

void coolLUB(char* type1, char* type2, char*result) {

	int typesMatch;

	switch(*type1) {

		// Simple types
		case 'I': case 'D': case 'd': case 'L': case 'l':
			
			typesMatch = (strcmp(type1, type2) == 0);

			if (typesMatch) {

				strcpy(result, type1);
			}

			else {

				strcpy(result, "X");
			}

			break;

		// Reference types
		case 'A':
			if (*type2 != 'A') {

				strcpy(result, "X");
			}

			else {

				char	*type1_copy	= SafeStrdup(type1),
					*type2_copy	= SafeStrdup(type2),
					*lub		= LUB(type1_copy, type2_copy);

				strcpy(result, lub);

				if (strcmp(lub, "X")) {
					
					SafeFree(lub);
				}
			}

			break;

		default:
			strcpy(result, "X");
	}
}

int types_match(char* desired_type, char* supplied_type) {

	char result[MAX_BUFFER_SIZE];
	coolLUB(desired_type, supplied_type, result);

	return (strcmp(desired_type, result) == 0);
}

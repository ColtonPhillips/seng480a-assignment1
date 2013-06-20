#include "TypeUtils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
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

void get_cpitem_type(ClassFile* cf, int ix, char result[MAX_BUFFER_SIZE]) {

	if (ix <= 0 || ix > cf->constant_pool_count)
		return;

	switch(cf->cp_tag[ix]) {
		case CP_UTF8:
			sprintf(result, "ALjava/lang/String");
			break;
		case CP_Integer:  // PRId32 is defined in inttypes.h
			sprintf(result, "I");
			break;
		case CP_Float:
			sprintf(result, "F");
			break;
		case CP_Long:  // PRId64 is defined in inttypes.h
			sprintf(result, "Ll");
			break;
		case CP_Double:
			sprintf(result, "Dd");
			break;
		case CP_String:
			sprintf(result, "ALjava/lang/String");
			break;
		case CP_Class:
		case CP_Field:
		case CP_Method:
		case CP_Interface:
		case CP_NameAndType:
		default:
			die("Unsupported constant type\n");
			break;
	}
}


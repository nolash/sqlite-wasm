#include <string.h>

#include "opcodemap.h"
#include "_opcodemap.h"

int getOpcode(char *c) {
	int i;

	for (i = 0; i < opcodecount; i++) {
		if (!strcmp(c, opcodemap[i])) {
			return i;
		}
	}
	return -1;
}


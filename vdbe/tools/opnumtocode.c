#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../_opcodemap.h"

int main(int argc, char **argv) {
	char buf[256];
	FILE *f;
	int r;
	int c;

	f = fopen(*(argv+1), "r");
	c = 0;
	while (!feof(f)) {
		fread(&buf[c], 1, 1, f);
		if (buf[c] == 0x0a) {
			buf[c] = 0;
			char *p;
			p = strchr(buf, 0x2c);
			*p = 0;
			printf("%s,%s\n", opcodemap[atoi(buf)], p+1);
			memset(buf, 0, c);
			c = 0;
		} else {
			c++;
		}
	}
	fclose(f);
}

#include <stdio.h>
#include <string.h>

#include <sqliteInt.h>
#include <vdbe.h>
#include <sqlite3.h>

#include "./opcodes.h"

#define BUFSIZE 1024

int main(int argc, char **argv) {
	FILE *f;
	Vdbe *v;
	Parse *p;
	sqlite3 *s;
	int r;
	char *buf;
	size_t bufsize = BUFSIZE;

	// initalize
	r = sqlite3_initialize();
	s = sqlite3MallocZero(sizeof(sqlite3));
	p = sqlite3MallocZero(sizeof(Parse));
	s->mutex = 0;
	s->aLimit[SQLITE_LIMIT_VDBE_OP] = 250000000;
	p->db = s;
	buf = malloc(sizeof(char) * BUFSIZE);

	// create the virtual machine
	v = sqlite3VdbeCreate(p);

	// open the opcode textfile
	f = fopen(*(argv+1), "r");
	if (!f) {
		exit(1);
	}
	r = -1;
	while (r) {
		char *p;
		int i;

		r = getline(&buf, &bufsize, f);
		if (r == EOF) {
			break;
		}
		p = strtok(buf, ",");
		i = 0;
		while (p) {
			char *pn;
			char c[128];
			int n;

			pn = strtok(NULL, ",");
			if (!pn) {
				if (i == 4) {
					printf(":");
				}
				pn = strtok(NULL, "\n");
				*(p+strlen(p)-1)=0;
				printf("%s",p);
				break;
			}
			n = (int)(pn-p);
			memcpy(c, p, n);
			*(c+n)=0;
			
			printf("%s:", c);	
			p = pn;
			i++;
		}
		printf("\n");
	}	
	fclose(f);
	
	return 0;
}

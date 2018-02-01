#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sqliteInt.h>
#include <vdbe.h>
#include <vdbeInt.h>
#include <sqlite3.h>

#include "opcodemap.h"

char opcodemap[1024][32];
int opcodecount;

#define BUFSIZE 1024
#define MAXOPS 250000000;

int main(int argc, char **argv) {
	FILE *f;
	Parse *p;
	sqlite3 *s;
	int r;
	char *buf;
	int count;
	size_t bufsize = BUFSIZE;
	int i;
	Vdbe *v;

	// initalize
	r = sqlite3_initialize();
	//s = sqlite3MallocZero(sizeof(sqlite3));
	r = sqlite3_open("db", &s);
	p = sqlite3MallocZero(sizeof(Parse));
	s->mutex = 0;
	s->aLimit[SQLITE_LIMIT_VDBE_OP] = MAXOPS;
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
	count = 1; // sqlite3VdbeCreate already set init
	while (r) {
		char *p;
		int i;
		int op[5];

		r = getline(&buf, &bufsize, f);
		if (r == EOF) {
			break;
		}
		p = strtok(buf, ",");
		i = 0;
		fprintf(stderr, "%s\n", buf);
		while (p) {
			char *pn;
			char c[128];
			int n;
			long int l;

			pn = strtok(NULL, ",");
			if (!pn) {
				int ii;

				ii = i;
				if (i == 4) {
					ii++;
					op[ii] = 0;
				}
				pn = strtok(NULL, "\n");
				*(p+strlen(p)-1)=0;
				l = strtol(p, NULL, 10);
				assert(l != LONG_MAX && l != LONG_MIN);
				op[ii] = l;
				break;
			}
			n = (int)(pn-p);
			memcpy(c, p, n);
			*(c+n)=0;
			if (!strcmp(c, "Init")) {
				op[0] = -1;
				break;
			}

			if (i == 0) {
				op[i] = getOpcode(c);
				assert(op[i] >= 0);
			} else {
				l = strtol(c, NULL, 10);
				assert(l != LONG_MAX && l != LONG_MIN);
				op[i] = l;
			}	
			p = pn;
			i++;
		}
		if (op[0] < 0) {
			continue;
		}
		if (i == 4) {
			r = sqlite3VdbeAddOp3(v, op[0], op[1], op[2], op[3]);
		} else {
			r = sqlite3VdbeAddOp4(v, op[0], op[1], op[2], op[3], (const char*)op[4], P4_INT32);
		}
		assert(r > 0);
		count++;
	}	
	fclose(f);

	for (i = 0; i < count; i++) {
		VdbeOp *op;
		op = sqlite3VdbeGetOp(v, i);
		fprintf(stderr, "%d: [%d] %d, %d, %d, %i\n", i, op->opcode, op->p1, op->p2, op->p3, op->p4);
	}

	sqlite3VdbeMakeReady(v, p);
	sqlite3VdbeRunOnlyOnce(v);
	sqlite3VdbeUsesBtree(v, 0);
	v->nCursor = 1;

	// stealing from static sqlite3Step
	s->u1.isInterrupted = 0;
	s->nVdbeActive++;
	v->pc = 0;
	s->nVdbeExec++;
	r = sqlite3VdbeExec(v);
	s->nVdbeExec--;
	
	return 0;
}

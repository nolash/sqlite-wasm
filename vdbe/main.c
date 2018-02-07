#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#include <sqliteInt.h>
#include <vdbe.h>
#include <vdbeInt.h>
#include <sqlite3.h>

#include "opcodemap.h"

char opcodemap[1024][32];
int opcodecount;

#include "memhack.h"

sqlite3_mem_methods wasm_mem_methods;
void *wasm_debugmem;

#define BUFSIZE 1024
#define MAXOPS 250000000
#define TMPMEMCELLSIZE 1024 * 1024

int main(int argc, char **argv) {
	FILE *f;
	Parse *p;
	sqlite3 s;
	int r;
	char *buf;
	int count;
	size_t bufsize;
	int i;
	Vdbe *v;

	// initalize
	bufsize =  BUFSIZE;
	sqlite3_config(SQLITE_CONFIG_MEMSTATUS, 0); // we will not resize memory so we need not keep track of it 
	//sqlite3_config(SQLITE_CONFIG_MALLOC, &wasm_mem_methods);

	r = sqlite3MallocInit();
	assert(r == SQLITE_OK);
	memset(&s, 0, sizeof(sqlite3));
	p = sqlite3MallocZero(sizeof(Parse));

	// initial growOp when calling ..AddOp fails if we don't manually set bounds
	s.aLimit[SQLITE_LIMIT_VDBE_OP] = MAXOPS; 
	s.enc = SQLITE_UTF8;

	// create the virtual machine:
	// * allocate memory for the Vdbe
	// * set vdbe db to parse db
	// * link vdbe parse to parse
	// * link parse vdbe to vdbe
	// * add init instruction (which is why we skip it in the import)
	p->db = &s;
	v = sqlite3VdbeCreate(p);

	// open the opcode textfile
	f = fopen(*(argv+1), "r");
	if (!f) {
		exit(1);
	}

	// inject opcodes
	buf = malloc(sizeof(char) * BUFSIZE);
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
		//fprintf(stderr, "%s\n", buf);
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
		//fprintf(stderr, "inserted op #%d\n", r);
		count++;
	}	
	fclose(f);
	v->nOp = count;

	// TODO: actual value comparison
	for (i = 0; i < count; i++) {
		VdbeOp *op;
		op = sqlite3VdbeGetOp(v, i);
		//fprintf(stderr, "%d: [%d] %d, %d, %d, %i\n", i, op->opcode, op->p1, op->p2, op->p3, op->p4);
	}

	// Prepare vdbe for first run
	// * allocate memory for cursors, argumentsa
	// * rewinds vdbe to the start of the instruction list
	p->nMem = 1; // specify how many memory cells we need, one per cursor
	sqlite3VdbeMakeReady(v, p);

	// TODO: Optimize.
	r = sqlite3_open_v2("db", &v->db, SQLITE_OPEN_READONLY, 0x0);
	if (r != SQLITE_OK) {
		raise(SIGABRT);
	}

	// TODO: Optimize. (sqlite3InitOne is local by default make and can't be called directly)
	// Load schema for database and create internal representation
	v->db->pVdbe = v;
	r = sqlite3Init(v->db, &buf);
	if (r != SQLITE_OK) {
		raise(SIGABRT);
	}

	// Strictly correct but doesn't affect the simplest example
	// sqlite3VdbeRunOnlyOnce(v);
	
	// manually implement necessary calls from static method sqlite3Step
	v->pc = 0;
	v->db->nVdbeActive++;
	v->db->nVdbeExec++;
	i = 0;
	while (r != SQLITE_DONE) {
		r = sqlite3VdbeExec(v->db->pVdbe);
		if (r != SQLITE_ROW && r != SQLITE_DONE) {
			raise(SIGABRT);
		}
		if (v->pResultSet != 0) {
			fprintf(stdout, "Row #%d: k: %d, v: %s\n", i, v->pResultSet->u.i, (v->pResultSet+1)->z);
		}
		i++;
	}
	v->db->nVdbeExec--;
	v->db->nVdbeActive--;
	
	return 0;
}

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
#include "vfshack.h"

sqlite3_mem_methods wasm_mem_methods;
void *wasm_debugmem;

#define BUFSIZE 1024
#define MAXOPS 64
#define MAXSQL 1024
#define MAXLEN 4096
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
	VdbeCursor vc;
	Btree *btree;
	sqlite3_vfs *vfs;
	unsigned int dbmeta[9];
	unsigned char dbmetaitem;
	int rootpage;

	// initalize
	bufsize =  BUFSIZE;
	rootpage = -1;
	sqlite3_config(SQLITE_CONFIG_MEMSTATUS, 0); // we will not resize memory so we need not keep track of it 
	sqlite3_config(SQLITE_CONFIG_MALLOC, &wasm_mem_methods);
	r = sqlite3_enable_shared_cache(0);
	if (r != SQLITE_OK) {
		raise(SIGABRT);
	}

	r = sqlite3MallocInit();
	if (r != 0) {
		raise(SIGABRT);
	}
	assert(r == SQLITE_OK);
	memset(&s, 0, sizeof(sqlite3));
	p = sqlite3MallocZero(sizeof(Parse));

	// initial growOp when calling ..AddOp fails if we don't manually set bounds
	s.aLimit[SQLITE_LIMIT_VDBE_OP] = MAXOPS; 

	// set bounds for sql statement lengths
	s.aLimit[SQLITE_LIMIT_SQL_LENGTH] = MAXSQL; 
	s.aLimit[SQLITE_LIMIT_LENGTH] = MAXLEN; 

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
		if (op[0] == 104) { // OpenRead
			rootpage = op[2];
		}	
		assert(r > 0);
		//fprintf(stderr, "inserted op #%d\n", r);
		count++;
	}	
	fclose(f);

	if (rootpage < 0) {
		raise(SIGABRT);
	}

	v->nOp = count;

	// TODO: actual value comparison
//	for (i = 0; i < count; i++) {
//		VdbeOp *op;
//		op = sqlite3VdbeGetOp(v, i);
//		fprintf(stderr, "%d: [%d] %d, %d, %d, %i\n", i, op->opcode, op->p1, op->p2, op->p3, op->p4);
//	}

	// Prepare vdbe for first run
	// * allocate memory for cursors, argumentsa
	// * rewinds vdbe to the start of the instruction list
	p->nMem = 1; // specify how many memory cells we need, one per cursor
	sqlite3VdbeMakeReady(v, p);

	// sqlite3->aDbStatic holds the main db pointer. get the db btree from it
	btree = s.aDbStatic->pBt;

	// one for main, one for temp
	s.aDb = sqlite3MallocZero(sizeof(Db*));
	s.aDb = s.aDbStatic;

	// vfs hack is shortcircuit to in-memory data store
	vfs = sqlite3MallocZero(sizeof(sqlite3_vfs));
	wasmvfs_load("/mnt/data/src/jaak/sqlite-truebit/vdbe/db", &vfs);

	// this throws an internal switch we haven't found yet, crashes without
	sqlite3_vfs_register(vfs, 0);

	// opens the database
	r = sqlite3BtreeOpen(vfs, "db", &s, &btree, 0, SQLITE_OPEN_READONLY | SQLITE_OPEN_MAIN_DB);
	if (r != SQLITE_OK) {
		raise(SIGABRT);
	}
	s.magic = SQLITE_MAGIC_OPEN;

	// hack the database pointers
	s.aDb[0].zDbSName = "main";
	s.aDb[0].pSchema = sqlite3MallocZero(sizeof(Schema));
	s.aDb[0].pBt = btree;
	s.aDb[0].safety_level = SQLITE_DEFAULT_SYNCHRONOUS + 1;
	s.aDb[1].zDbSName = "temp";
	s.aDb[1].pSchema = sqlite3SchemaGet(&s, 0);
	s.aDb[1].safety_level = PAGER_SYNCHRONOUS_OFF;

	// retrieve and set metadata from main database
	// see prepare.c:sqlite3InitOne
	sqlite3BtreeEnter(s.aDb[0].pBt);
	if (!sqlite3BtreeIsInReadTrans(s.aDb[0].pBt)) {
		sqlite3BtreeBeginTrans(s.aDb[0].pBt, 0);
	}
	for (i = 0; i < 9; i++) {
		sqlite3BtreeGetMeta(s.aDb[0].pBt, i+1, (unsigned int*)&dbmeta[i]);
	}

	// .. set schema
	s.aDb[0].pSchema->schema_cookie = dbmeta[BTREE_SCHEMA_VERSION-1];

	// .. set encoding
	dbmetaitem = (u8)dbmeta[BTREE_TEXT_ENCODING-1] & 3;
	if (dbmetaitem == 0) {
		dbmetaitem = SQLITE_UTF8;	
	}
	s.enc = dbmetaitem;

	// .. set fileformat
	dbmetaitem = (u8)dbmeta[BTREE_FILE_FORMAT-1];
	if (dbmetaitem == 0) {
		dbmetaitem = 1;
	}
	s.aDb[0].pSchema->file_format = dbmetaitem;

	// reset the schema
	DbClearProperty(&s, 0, DB_Empty);
	s.init.iDb = 0;
	sqlite3ResetAllSchemasOfConnection(&s);


	// create cursor
	v->aMem[0].xDel = wasm_mem_methods.xFree;
	v->aMem[0].db = &s;
	v->db->pVdbe = v;

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
	sqlite3_free(&s);
	return 0;
}

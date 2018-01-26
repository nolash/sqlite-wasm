#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "sqlite3.h"
#include "sqliteInt.h"
#include "vdbe.h"
#include "parse.h"
#include "hello.h"

int main() {
	Parse *p;
	Vdbe *v;
	sqlite3 *db;
	char *filename;

	bzzvfs_register();
	p = (Parse*)sqlite3ParserAlloc((void*)malloc); 	
	filename = tmpnam(NULL);	
	sqlite3_open_v2(filename, &db, SQLITE_OPEN_READONLY, "bzz");
	p->db = db;
	v = sqlite3VdbeCreate(p);
	sqlite3_close(db);
	unlink(filename);
	return 0;
}

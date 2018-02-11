#ifndef SQLITE_WASM_MEM_H_
#define SQLITE_WASM_MEM_H_

#include <signal.h>

#include "memhack.h"

#define SQLITE_WASM_PAGESIZE 64

#ifndef SQLITE_WASM_PAGECOUNT
#define SQLITE_WASM_PAGECOUNT 102400 // apx 6.4MB
#endif

char wasm_mem[SQLITE_WASM_PAGESIZE * SQLITE_WASM_PAGECOUNT]; 
int wasm_memsize;
int wasm_memcrsr;
void *wasm_debugmem;

void *wasm_malloc(int c) {
	void *ptr;
	if (c + wasm_memcrsr > wasm_memsize) {
		return 0;
	}
	ptr = &wasm_mem[wasm_memcrsr];
	wasm_memcrsr += c;
	return ptr;
}

void wasm_free(void *v) {
	return;
}

void *wasm_realloc(void *v, int c) {
	raise(SIGABRT);
}

int wasm_size(void *v) {
	return SQLITE_WASM_PAGESIZE * SQLITE_WASM_PAGECOUNT;
}

int wasm_roundup(int c) {
	if (c > wasm_memsize) {
		return 0;
	}
	return wasm_memsize;
}

int wasm_init(void *m) {
	wasm_memsize = SQLITE_WASM_PAGESIZE * SQLITE_WASM_PAGECOUNT;
	wasm_debugmem = &wasm_mem;
	wasm_memcrsr = 0;
	return 0;
}

void wasm_shutdown(void *m) {
	return;	
}

sqlite3_mem_methods wasm_mem_methods = {
	wasm_malloc,
	wasm_free,
	wasm_realloc,
	wasm_size,
	wasm_roundup,
	wasm_init,
	wasm_shutdown,
	0x0 // what should we put here?
};

#endif

#ifndef SQLITE_WASM_MEM_H_
#define SQLITE_WASM_MEM_H_

#include <signal.h>
#include <stdlib.h>

#include "memhack.h"

#define SQLITE_WASM_PAGESIZE 64

#ifndef SQLITE_WASM_PAGECOUNT
#define SQLITE_WASM_PAGECOUNT 102400 // apx 6.4MB
#endif

#define SQLITE_WASM_ALLOCSIZE (SQLITE_WASM_PAGESIZE * 1000)

char wasm_mem[SQLITE_WASM_PAGESIZE * SQLITE_WASM_PAGECOUNT]; 
int wasm_memsize;
int wasm_memcrsr;
void *wasm_debugmem;

int wasm_allocations_count;
void *wasm_allocations_ptr[1024];
int wasm_allocations_idx[1024];
int wasm_allocations_sz[1024];

static void *getPtr(void *p, int *idx, int *sz) {
	int i;
	for (i = 0; i < wasm_allocations_count; i++) {
		if (p == wasm_allocations_ptr[i]) {
			*idx = wasm_allocations_idx[i];
			*sz = wasm_allocations_sz[i];
			return wasm_allocations_ptr[i];
		}
	}
	return 0;
}

void *addPtr(void *p, int idx, int sz) {
	wasm_allocations_ptr[wasm_allocations_count] = p;
	wasm_allocations_idx[wasm_allocations_count] = idx,
	wasm_allocations_sz[wasm_allocations_count] = sz;
	wasm_allocations_count++;
	return p;
}

void *wasm_malloc(int c) {
	void *ptr;
	if (c + wasm_memcrsr > wasm_memsize) {
		return 0;
	}
	ptr = addPtr(&wasm_mem[wasm_memcrsr], wasm_memcrsr, c);
	wasm_memcrsr += c;

	return ptr;
}

// noop
void wasm_free(void *v) {
	return;
}

// crude, orphans existing memory
void *wasm_realloc(void *v, int c) {
	void *ptr;

	if (v == NULL) {
		raise(SIGSEGV);
	}

	if (wasm_memsize < c + wasm_memcrsr) {
		raise(SIGSEGV);
	}
	
	ptr = addPtr(&wasm_mem[wasm_memcrsr], wasm_memcrsr, c);

	wasm_memcrsr+=c;
	return ptr;
}

int wasm_size(void *v) {
	void *ptr;
	int idx;
	int sz;

	ptr = getPtr(v, &idx, &sz);
	if (ptr == NULL) {
		return -1;
	}
	return sz;
}

int wasm_roundup(int c) {
	int d;
	int i;

	i = c / SQLITE_WASM_ALLOCSIZE;
	if (c % SQLITE_WASM_ALLOCSIZE) {
		i++;
	}

	if (i * SQLITE_WASM_ALLOCSIZE + wasm_memcrsr > wasm_memsize) {
		return 0;
	}

	return i * SQLITE_WASM_ALLOCSIZE;
}

int wasm_init(void *m) {
	wasm_memsize = SQLITE_WASM_PAGESIZE * SQLITE_WASM_PAGECOUNT;
	wasm_debugmem = &wasm_mem;
	wasm_memcrsr = 0;

	wasm_allocations_count = 0;

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

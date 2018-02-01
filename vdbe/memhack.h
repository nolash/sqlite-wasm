#ifndef SQLITE_WASM_MEMHACK_H_
#define SQLITE_WASM_MEMHACK_H_ 1

#include "sqlite3.h"

extern sqlite3_mem_methods wasm_mem_methods;
extern void *wasm_debugmem;

#endif

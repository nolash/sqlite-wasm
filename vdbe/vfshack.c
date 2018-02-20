#include <string.h>
#include <stdio.h>
#include <time.h>

#include <sqlite3.h>

#define SQLITE_WASM_SECTORSIZE 4096
#define SQLITE_WASM_STORESIZE SQLITE_WASM_SECTORSIZE * 3

#define SQLITE_WASM_VFSMAXPATHSIZE 512
#define SQLITE_WASM_VFSID "wasm"

char wasm_store[SQLITE_WASM_STORESIZE];
long long int wasm_store_used;

static int wasmClose(sqlite3_file *file) {
	return SQLITE_OK;
}

static int wasmRead(sqlite3_file *file, void *p, int iAmt, sqlite3_int64 iOfst) {
	memcpy(p, wasm_store+iOfst, iAmt);
	return SQLITE_OK;
}

static int wasmWrite(sqlite3_file *file, const void *p, int iAmt, sqlite3_int64 iOfst) {
	return SQLITE_IOERR_WRITE;
}

static int wasmTruncate(sqlite3_file *file, sqlite3_int64 size) {
	return SQLITE_IOERR_TRUNCATE;
}

static int wasmSync(sqlite3_file *file, int flags) {
	return SQLITE_IOERR_FSYNC;
}

static int wasmFileSize(sqlite3_file *file, sqlite3_int64 *o_size) {
	*o_size = wasm_store_used;
	return SQLITE_OK;
}

static int wasmLock(sqlite3_file *file, int l) {
	return SQLITE_OK;
}

static int wasmUnlock(sqlite3_file *file, int l) {
	return SQLITE_OK;
}

static int wasmCheckReservedLock(sqlite3_file *file, int *o_res) {
	return SQLITE_IOERR_LOCK;
}

static int wasmFileControl(sqlite3_file *file, int op, void *o_arg) {
	sqlite3_int64 sz;
	switch (op) {
		case 18:
			//sz = SQLITE_WASM_SECTORSIZE;
			//memcpy(o_arg, &sz, sizeof(sqlite3_int64));
			fprintf(stderr, "fctrl mmapsize (%d): returning %d!\n", op, *((sqlite3_int64*)o_arg));
			break;
		default:
			fprintf(stderr, "fctrl %d!\n", op);
	}
	return SQLITE_OK;
}

static int wasmSectorSize(sqlite3_file *file) {
	return SQLITE_WASM_SECTORSIZE;
}

static int wasmDeviceCharacteristics(sqlite3_file *file) {
	fprintf(stderr, "devchr\n");
	return SQLITE_IOCAP_ATOMIC4K | SQLITE_IOCAP_SEQUENTIAL;
}

static int wasmShmMap(sqlite3_file *file, int iPg, int pgsz, int n, void volatile **v) {
	fprintf(stderr, "shmmap\n");
	return SQLITE_OK;
}

static int wasmShmLock(sqlite3_file *file, int offset, int n, int flags) {
	return SQLITE_OK;
}

static void wasmShmBarrier(sqlite3_file *file) {
	return;
}

static int wasmShmUnmap(sqlite3_file *file, int deleteFlag) {
	return SQLITE_OK;
}

static int wasmFetch(sqlite3_file *file, sqlite3_int64 iOfst, int iAmt, void **pp) {
	fprintf(stderr, "fetch\n");
	return SQLITE_OK;
}

static int wasmUnfetch(sqlite3_file *file, sqlite3_int64 iOfst, void *p) {
	fprintf(stderr, "unfetch\n");
	return SQLITE_OK;
}

int wasmOpen(sqlite3_vfs *vfs, const char *zName, sqlite3_file *file, int flags, int *outflags) {
	static sqlite3_io_methods wasmIO = {
		3,
		wasmClose,
		wasmRead,
		wasmWrite,
		wasmTruncate,
		wasmSync,
		wasmFileSize,
		wasmLock,
		wasmUnlock,
		wasmCheckReservedLock,
		wasmFileControl,
		wasmSectorSize,
		wasmDeviceCharacteristics,
		wasmShmMap,
		wasmShmLock,
		wasmShmBarrier,
		wasmShmUnmap,
		wasmFetch,
		wasmUnfetch
	};
	memset(file, 0, sizeof(sqlite3_file));
	file->pMethods = &wasmIO;
	return SQLITE_OK;
}


static int wasmDelete(sqlite3_vfs *vfs, const char *zName, int syncDir) {
	fprintf(stderr, "delete\n");
	return SQLITE_OK;
}

static int wasmAccess(sqlite3_vfs *vfs, const char *zName, int flags, int *pResOut) {
	fprintf(stderr, "access\n");
	*pResOut = 1;
	return SQLITE_OK;
}

static int wasmFullPathname(sqlite3_vfs *vfs, const char *zName, int nOut, char *zOut) {
	fprintf(stderr, "fullpath\n");
	size_t l = strlen(zName);
	strncpy(zOut, zName, l);
	return SQLITE_OK;
}

static void* wasmDlOpen(sqlite3_vfs *vfs, const char *zName) {
	fprintf(stderr, "dlopen\n");
	return 0;
}

static void wasmDlError(sqlite3_vfs *vfs, int nByte, char *zErrMsg) {
	sqlite3_snprintf(nByte, zErrMsg, "not supported\0");
	return;
}

static void (*wasmDlSym(sqlite3_vfs *vfs, void *foo, const char *zSymbol))(void) {
	return 0;
}

static void wasmDlClose(sqlite3_vfs *vfs, void *foo) {
	return;
}

static int wasmRandomness(sqlite3_vfs *vfs, int nByte, char *zByte) {
	fprintf(stderr, "random\n");
	return SQLITE_OK;
}

static int wasmSleep(sqlite3_vfs *vfs, int us) {
	fprintf(stderr, "sleep\n");
	return us;
}

static int wasmCurrentTime(sqlite3_vfs *vfs, double *o_t) {
	fprintf(stderr, "curtime\n");
	time_t t = time(0);
	o_t = (double*)&t;
	return SQLITE_OK;
}

static int wasmGetLastError(sqlite3_vfs *vfs, int n, char *s) {
	fprintf(stderr, "lasterr\n");
	return SQLITE_OK;
}

static int wasmCurrentTimeInt64(sqlite3_vfs *vfs, sqlite3_int64 *o_t) {
	fprintf(stderr, "curtime64\n");
	return SQLITE_OK;
}

static int wasmSetSystemCall(sqlite3_vfs *vfs, const char *zName, sqlite3_syscall_ptr pSys) {
	fprintf(stderr, "setsys\n");
	return SQLITE_OK;
}

static sqlite3_syscall_ptr wasmGetSystemCall(sqlite3_vfs *vfs, const char *zName) {
	fprintf(stderr, "getsys\n");
	return 0;
}

static const char* wasmNextSystemCall(sqlite3_vfs *vfs, const char *zName) {
	fprintf(stderr, "nextsys\n");
	return "\0";
}

int wasmvfs_register() {
	int r;

//	r = sqlite3_initialize();
//	if (r != SQLITE_OK) {
//		return r;
//	}
//
	
}

int wasmvfs_load(char *name, sqlite3_vfs **vfs) {
	FILE *f;
	int c;

	f = fopen(name, "r");
	wasm_store_used = 0;
	while (!feof(f)) {
		c = fread(wasm_store+wasm_store_used, 1, SQLITE_WASM_SECTORSIZE, f);
		if (c != SQLITE_WASM_SECTORSIZE) {
			if (feof(f)) 
				break;
			fprintf(stderr, "shortread %d/%d\n", c, SQLITE_WASM_SECTORSIZE);
		}
		wasm_store_used += c;
	}
	fclose(f);

	sqlite3_vfs initvfs = {
		3,
		sizeof(sqlite3_file),
		SQLITE_WASM_VFSMAXPATHSIZE,
		0,
		SQLITE_WASM_VFSID,
		0,
		wasmOpen,
		wasmDelete,
		wasmAccess,
		wasmFullPathname,
		wasmDlOpen,
		wasmDlError,
		wasmDlSym,
		wasmDlClose,
		wasmRandomness,
		wasmSleep,
		wasmCurrentTime,
		wasmGetLastError,
		wasmCurrentTimeInt64,
		wasmSetSystemCall,
		wasmGetSystemCall,
		wasmNextSystemCall
	};
	memcpy(*vfs, &initvfs, sizeof(sqlite3_vfs));
	return SQLITE_OK;
	//return sqlite3_vfs_register(&wasmvfs, 0);
}

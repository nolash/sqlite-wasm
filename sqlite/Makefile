# Before running this make:
#
# Use emsdk sdk-1.37.27-64bit (https://github.com/juj/emsdk)
# * be sure to run the envvar script so the emcc invocation in this makefile uses the right compiler version
#
# Use swarm-vfs commit 406f77c (https://github.com/nolash/swarm-sqlite-vfs)
#
# Use sqlite 3.21.0 with configure - https://sqlite.org/2017/sqlite-autoconf-3210000.tar.gz - (sha1 ebe33c20d37a715db95288010c1009cd560f2452)
# * in SQLITEDIR/tool/mksqlite3c.tcl, remove all SQLITE_PRIVATE macros
# * in SQLITEDIR/libtool, set always_export_symbols=yes
# * from builddir, do configure --enable-debug --disable-tcl --disable-amalgamation && make

# our work dir
CURDIR=$(shell pwd)

# sqlite root dir
SQLITEDIR=/mnt/data/src/misc/sqlite-src-3210000

# sqlite build subdir
SQLITEBUILDSUBDIR=build-3

# include lines for sqlite
INCLUDE=-I${SQLITEDIR} -I${SRC} -I${SQLITEDIR}/src -I${SQLITEDIR}/${SQLITEBUILDSUBDIR} -I${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc 

# swarm vfs repo root
VFSDIR=/home/lash/data/src/swarm-sqlite-vfs

#EMCCDIR=/mnt/data/src/ext/emscripten
#BINARYENDIR=/mnt/data/src/ext/binaryen/build/bin
#LDDIR=/mnt/data/src/ext/llvm/lld-5.0.0.src/build/bin
#LLVMDIR=/mnt/data/src/ext/llvm/llvm-5.0.0.src/build/bin

emsdk:
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/rowset.c -o rowset.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/btree.c -o btree.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/vdbe.c -o vdbe.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/btmutex.c -o btmutex.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/vdbeaux.c -o vdbeaux.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/vdbemem.c -o vdbemem.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/malloc.c -o malloc.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/global.c -o global.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/status.c -o status.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/trigger.c -o trigger.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/expr.c -o expr.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/callback.c -o callback.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/hash.c -o hash.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/fault.c -o fault.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/util.c -o util.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/build.c -o build.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/select.c -o select.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/where.c -o where.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/wherecode.c -o wherecode.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/insert.c -o insert.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/walker.c -o walker.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/prepare.c -o prepare.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/delete.c -o delete.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/resolve.c -o resolve.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/auth.c -o auth.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/fkey.c -o fkey.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/vtab.c -o vtab.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/main.c -o main_sq.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/ctime.c -o ctime.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/pragma.c -o pragma.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/tokenize.c -o tokenize.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/parse.c -o parse.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/printf.c -o printf.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/update.c -o update.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/vacuum.c -o vacuum.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/attach.c -o attach.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/vdbetrace.c -o vdbetrace.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/analyze.c -o analyze.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/pager.c -o pager.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/wal.c -o wal.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/os_unix.c -o os_unix.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/os.c -o os.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/pcache.c -o pcache.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/memjournal.c -o memjournal.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/pcache1.c -o pcache1.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/backup.c -o backup.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/bitvec.c -o bitvec.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/vdbeapi.c -o vdbeapi.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/alter.c -o alter.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/legacy.c -o legacy.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/random.c -o random.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/vdbesort.c -o vdbesort.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/func.c -o func.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/utf.c -o utf.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/threads.c -o threads.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/date.c -o date.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/vdbeblob.c -o vdbeblob.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/table.c -o table.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/complete.c -o complete.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/whereexpr.c -o whereexpr.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/treeview.c -o treeview.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/loadext.c -o loadext.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/opcodes.c -o opcodes.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/mem1.c -o mem1.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/mutex_unix.c -o mutex_unix.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/mutex_noop.c -o mutex_noop.bc
	EMMAKEN_CFLAGS="${INCLUDE}" emcc ${SQLITEDIR}/${SQLITEBUILDSUBDIR}/tsrc/mutex.c -o mutex.bc

main: 
	EMMAKEN_CFLAGS="-I${VFSDIR}/demo/hello ${INCLUDE}" emcc ${VFSDIR}/demo/main_hello.c -o main.bc -DTEST_HELLO=1

links: emsdk main
	EMMAKEN_CFLAGS="-I${VFSDIR}/demo/hello ${INCLUDE}" emcc -o hello.bc ${VFSDIR}/demo/hello/hello.c -DTEST_HELLO=1
	emcc hello.bc main.bc rowset.bc btree.bc vdbe.bc btmutex.bc vdbeaux.bc vdbemem.bc malloc.bc global.bc status.bc trigger.bc expr.bc callback.bc hash.bc fault.bc util.bc build.bc select.bc where.bc wherecode.bc insert.bc walker.bc prepare.bc delete.bc resolve.bc auth.bc fkey.bc vtab.bc main_sq.bc ctime.bc pragma.bc tokenize.bc parse.bc printf.bc update.bc vacuum.bc attach.bc vdbetrace.bc analyze.bc pager.bc wal.bc os_unix.bc os.bc pcache.bc memjournal.bc pcache1.bc backup.bc bitvec.bc vdbeapi.bc alter.bc legacy.bc random.bc vdbesort.bc func.bc utf.bc threads.bc date.bc vdbeblob.bc table.bc complete.bc whereexpr.bc treeview.bc loadext.bc opcodes.bc mem1.bc mutex_unix.bc mutex.bc mutex_noop.bc -s WASM=1 


clean:
	rm -f *.o *.js *.s *.bc *.map *.html *.wasm *.wast.* *.a

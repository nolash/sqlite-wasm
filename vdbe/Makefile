SQLITEROOT=/mnt/data/src/misc/sqlite-src-3210000
SQLITEBUILD=$(SQLITEROOT)/build_local
SQLITESRC=$(SQLITEBUILD)/tsrc
INCLUDE=-I$(SQLITESRC) -I.
LIB=-Wl,-rpath -Wl,$(SQLITEBUILD)/lib
CC=gcc
CFLAGS=-g3

all: opcodes sqlite obj
	$(CC) $(INCLUDE) $(CFLAGS) -c  new.c -o new.o
	$(CC) $(INCLUDE) $(CFLAGS) -DNDEBUG -DSQLITE_DEBUG $(LIB) -o new opcodemap.o new.o  -lsqlite3
obj: 
	$(CC) $(INCLUDE) $(CFLAGS) -c opcodemap.c -o  opcodemap.o
sqlite:
	$(CC) $(INCLUDE) $(CFLAGS) -c $(SQLITESRC)/vdbe.c -o vdbe.o
opcodes:
	-! [ -f ./_opcodemap.h ] && cat $(SQLITEBUILD)/opcodes.h | sh ./opcode.sh > _opcodemap.h

.PHONY: clean
clean:
	rm *.o
	rm _opcodemap.h

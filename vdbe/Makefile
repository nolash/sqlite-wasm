SQLITEROOT=/mnt/data/src/misc/sqlite_hack
SQLITEBUILD=$(SQLITEROOT)/build_dumpop
SQLITESRC=$(SQLITEBUILD)/tsrc
INCLUDE=-I$(SQLITESRC) -I.
LIB=-Wl,-rpath $(SQLITEBUILD)/lib
CC=gcc
CFLAGS=-gdwarf-4 -O0

all: opcodes obj
	$(CC) $(INCLUDE) $(CFLAGS) -c  main.c -o main.o
	$(CC) $(INCLUDE) $(CFLAGS) $(LIB) -o main main.o opcodemap.o memhack.o vfshack.o -lsqlite3
obj: 
	$(CC) $(INCLUDE) $(CFLAGS) -c opcodemap.c -o  opcodemap.o
	$(CC) $(INCLUDE) $(CFLAGS) -c memhack.c -o memhack.o
	$(CC) $(INCLUDE) $(CFLAGS) -c vfshack.c -o vfshack.o
sqlite:
	$(CC) $(INCLUDE) $(CFLAGS) -c $(SQLITESRC)/vdbe.c -o vdbe.o
opcodes:
	-! [ -f ./_opcodemap.h ] && cat $(SQLITEBUILD)/opcodes.h | sh ./tools/opcode.sh > _opcodemap.h

.PHONY: vanilla
vanilla: 
	$(CC) $(INCLUDE) $(CFLAGS) $(LIB) vanilla.c -o vanilla -lsqlite3

.PHONY: clean
clean:
	-rm *.o main
	-rm _opcodemap.h

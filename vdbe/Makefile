SQLITEROOT=/mnt/data/src/misc/sqlite-src-3210000
SQLITESRC=$(SQLITEROOT)/build/tsrc
INCLUDE=-I$(SQLITESRC) -I.
LIB=-Wl,-rpath -Wl,$(SQLITEROOT)/build/lib
CC=gcc
CFLAGS=-g3

all: sqlite obj
	$(CC) $(INCLUDE) $(CFLAGS) -c  new.c -o new.o
	$(CC) $(INCLUDE) $(CFLAGS) -DSQLITE_DEBUG $(LIB) -o new opcodemap.o new.o  -lsqlite3
obj: 
	$(CC) $(INCLUDE) $(CFLAGS) -c opcodemap.c -o  opcodemap.o
sqlite:
	$(CC) $(INCLUDE) $(CFLAGS) -c $(SQLITESRC)/vdbe.c -o vdbe.o

.PHONY: clean
clean:
	rm *.o

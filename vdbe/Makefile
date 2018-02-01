SQLITEROOT=/mnt/data/src/misc/sqlite-src-3210000
SQLITESRC=$(SQLITEROOT)/build/tsrc
INCLUDE=-I$(SQLITESRC)
LIB=-Wl,-rpath -Wl,$(SQLITEROOT)/build/lib

new: obj
	gcc $(INCLUDE) -c -g3 new.c
	gcc $(INCLUDE) -DSQLITE_DEBUG $(LIB) -o new new.o vdbe.o -lsqlite3
obj:
	gcc $(INCLUDE) -c -g3 $(SQLITESRC)/vdbe.c

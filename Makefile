COMMON=-O2 -std=c99 -mavx -pthread -Wall -pedantic -g
LIB=-Imemory_lib/ -Ilogcsv_lib/

all: lib test_memory test_csv

### Prototype / Dev Items ###
test_memory: lib memory_test/memory_test.c 
	gcc $(COMMON) $(LIB) memory_test/memory_test.c memory_lib/debug_memory.o -o memory_test/memory_test.bin

test_csv: lib logcsv_test/logcsv_test.c
	gcc $(COMMON) $(LIB) logcsv_test/logcsv_test.c logcsv_lib/logcsv.o -o logcsv_test/logcsv_test.bin

### Libraries ###
lib:  memory_lib/debug_memory.o logcsv_lib/logcsv.o

memory_lib/debug_memory.o: memory_lib/debug_memory.c memory_lib/debug_memory.h
	gcc $(COMMON) -c memory_lib/debug_memory.c -o memory_lib/debug_memory.o 

logcsv_lib/logcsv.o: logcsv_lib/logcsv.c logcsv_lib/logcsv.h
	gcc $(COMMON) -c logcsv_lib/logcsv.c -o logcsv_lib/logcsv.o

clean:
	rm memory_lib/*.o
	rm memory_test/*.bin
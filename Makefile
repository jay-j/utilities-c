COMMON=-O2 -std=c99 -mavx -pthread -Wall -pedantic -g
LIB=-Imemory_lib/

all: lib test_memory

### Prototype / Dev Items ###
test_memory: lib memory_test/memory_test.c 
	gcc $(COMMON) $(LIB) memory_test/memory_test.c memory_lib/debug_memory.o -o memory_test/memory_test.bin

### Libraries ###
lib:  memory_lib/debug_memory.o

memory_lib/debug_memory.o: memory_lib/debug_memory.c memory_lib/debug_memory.h
	gcc $(COMMON) -c memory_lib/debug_memory.c -o memory_lib/debug_memory.o 

clean:
	rm memory_lib/*.o
	rm memory_test/*.bin
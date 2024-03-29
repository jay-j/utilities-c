COMMON=-O2 -std=c99 -mavx -pthread -Wall -pedantic -g
LIB=-Imemory_lib/ -Ilogcsv_lib/ -Ihash_lib/ -Iprofile_lib/

all: lib tests

### Prototype / Dev Items ###
tests: test_memory test_csv test_hash test_trap test_smoothdelay profile_comparison

profile: lib profile_comparison
	./profile_test/profile_comparison.bin > data.csv

test_memory: lib memory_test/memory_test.c 
	gcc $(COMMON) $(LIB) memory_test/memory_test.c memory_lib/debug_memory.o -o memory_test/memory_test.bin

test_csv: lib logcsv_test/logcsv_test.c
	gcc $(COMMON) $(LIB) logcsv_test/logcsv_test.c logcsv_lib/logcsv.o -o logcsv_test/logcsv_test.bin

test_hash: lib hash_test/hashtable_test.c
	gcc $(COMMON) $(LIB) hash_test/hashtable_test.c hash_lib/hashtable.o -o hash_test/hashtable_test.bin

test_trap: lib profile_test/trap_test.c
	gcc $(COMMON) $(LIB) profile_test/trap_test.c profile_lib/trap.o -o profile_test/trap_test.bin

test_smoothdelay: lib profile_test/smoothdelay_test.c
	gcc $(COMMON) $(LIB) profile_test/smoothdelay_test.c profile_lib/smooth_delay.o -o profile_test/smoothdelay_test.bin

profile_comparison: lib profile_test/profile_comparison.c
	gcc $(COMMON) $(LIB) profile_test/profile_comparison.c profile_lib/trap.o profile_lib/smooth_delay.o profile_lib/profile_ema.o -o profile_test/profile_comparison.bin -lm

### Libraries ###
lib:  memory_lib/debug_memory.o logcsv_lib/logcsv.o hash_lib/hashtable.o profile_lib/trap.o profile_lib/smooth_delay.o profile_lib/profile_ema.o

memory_lib/debug_memory.o: memory_lib/debug_memory.c memory_lib/debug_memory.h
	gcc $(COMMON) -c memory_lib/debug_memory.c -o memory_lib/debug_memory.o 

logcsv_lib/logcsv.o: logcsv_lib/logcsv.c logcsv_lib/logcsv.h
	gcc $(COMMON) -c logcsv_lib/logcsv.c -o logcsv_lib/logcsv.o

hash_lib/hashtable.o: hash_lib/hashtable.c hash_lib/hashtable.h
	gcc $(COMMON) -c hash_lib/hashtable.c -o hash_lib/hashtable.o


profile_lib/profile_ema.o: profile_lib/profile_ema.c profile_lib/profile_ema.h
	gcc $(COMMON) -c profile_lib/profile_ema.c -o profile_lib/profile_ema.o

profile_lib/trap.o: profile_lib/trap.c profile_lib/trap.h
	gcc $(COMMON) -c profile_lib/trap.c -o profile_lib/trap.o

profile_lib/smooth_delay.o: profile_lib/smooth_delay.c profile_lib/smooth_delay.h
	gcc $(COMMON) -c profile_lib/smooth_delay.c -o profile_lib/smooth_delay.o

clean:
	rm */*.o
	rm */*.bin

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

struct MemoryEvent{ // 16 bytes
    int type; // -1=free, 0=realloc(), 1=malloc()     4 bytes
    uint32_t line; // line number of the event        4 bytes 
    char* file;    // pointer to the file string      8 bytes
};

struct MemoryAllocation{ // 24 bytes
    void* ptr;                  // 8 bytes
    size_t size;                // 8 bytes
    struct MemoryEvent* events; // 8 bytes (pointer!)
    size_t event_count;         // 8 bytes
};


// setup a data structure to record allocations of memory
void debug_mem_init();

void* debug_mem_malloc(size_t size, const char* file, uint32_t line);
void* debug_mem_realloc(void* ptr, size_t new_size, const char* file, uint32_t line);
void  debug_mem_free(void* ptr, const char* file, uint32_t line);

size_t debug_mem_find_variable(void* ptr);

void debug_mem_print_events();
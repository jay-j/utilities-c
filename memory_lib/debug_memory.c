#include "debug_memory.h"

// https://youtu.be/443UNeGrFoM?t=3009
// TODO some kind of tracking system to connect mallocs with frees
// build a list of the allocated memory

struct MemoryAllocation* debug_memory_array;
size_t debug_memory_array_length;
size_t debug_memory_array_count;


void debug_mem_init(){
    debug_memory_array = (struct MemoryAllocation*) malloc(1 * sizeof *debug_memory_array);
    debug_memory_array_count = 0;
    debug_memory_array_length = 1;
    printf("first time setup\n");
}


void* debug_mem_malloc(size_t size, const char* file, uint32_t line){
    printf("allocating %ld bytes in %s : %d ... \n", size, file, line);

    debug_memory_array_count++;

    // expand the array of stored memory if needed
    if (debug_memory_array_count > debug_memory_array_length){
        debug_memory_array_length *= 2;
        size_t next_size = debug_memory_array_length * sizeof *debug_memory_array;
        debug_memory_array = (struct MemoryAllocation*) realloc(debug_memory_array, next_size);
    }

    // actually allocate
    void* ptr =  malloc(size);
    if (ptr == NULL){
        printf("[ERROR] MEMORY ALLOCATION FAILED!\n");
    }

    // log the allocation
    size_t id = debug_memory_array_count-1;
    debug_memory_array[id].ptr = ptr;
    debug_memory_array[id].size = size;
    debug_memory_array[id].events = (struct MemoryEvent*) malloc(1 * sizeof (struct MemoryEvent));
    debug_memory_array[id].event_count = 1;
    debug_memory_array[id].events[0].type = 1;
    debug_memory_array[id].events[0].line = line; 
    debug_memory_array[id].events[0].size_prev = 0;
    debug_memory_array[id].events[0].size_new = size;
    debug_memory_array[id].events[0].file = (char*) malloc(64 * sizeof (char));
    strcpy(debug_memory_array[id].events[0].file, __FILE__);

    return ptr;
}


void* debug_mem_realloc(void* ptr, size_t new_size, const char* file, uint32_t line){
    printf("realloc %p to size %ld in %s : %d ... \n ", ptr, new_size, file, line);

    // find the variable in the leger 
    size_t id = debug_mem_find_variable(ptr);

    // register the realloc attempt
    debug_memory_array[id].event_count++;
    debug_memory_array[id].events = (struct MemoryEvent*) realloc(debug_memory_array[id].events,  debug_memory_array[id].event_count * sizeof (struct MemoryEvent));
    size_t event_id = debug_memory_array[id].event_count-1;
    debug_memory_array[id].events[event_id].type = 0;
    debug_memory_array[id].events[event_id].line = line;
    debug_memory_array[id].events[event_id].size_prev = debug_memory_array[id].size;
    debug_memory_array[id].events[event_id].size_new = new_size;
    debug_memory_array[id].events[event_id].file = (char*) malloc(64 * sizeof (char));
    strcpy(debug_memory_array[id].events[event_id].file, __FILE__);

    // actually realloc
    ptr = realloc(ptr, new_size);
    if (ptr == NULL){
        printf("[ERROR] REALLOC FAILED\n");
    }

    // update the pointer
    debug_memory_array[id].ptr = ptr;
    debug_memory_array[id].size = new_size;

    return ptr;
}


void debug_mem_free(void* ptr, const char* file, uint32_t line){
    printf("freeing in %s : %d ... \n", file, line);

    // find the variable in the leger 
    size_t id = debug_mem_find_variable(ptr);

    // register the free attempt
    debug_memory_array[id].event_count++;
    debug_memory_array[id].events = (struct MemoryEvent*) realloc(debug_memory_array[id].events,  debug_memory_array[id].event_count * sizeof (struct MemoryEvent));
    size_t event_id = debug_memory_array[id].event_count-1;
    debug_memory_array[id].events[event_id].type = -1;
    debug_memory_array[id].events[event_id].line = line;
    debug_memory_array[id].events[event_id].size_prev = debug_memory_array[id].size;
    debug_memory_array[id].events[event_id].size_new = 0;
    debug_memory_array[id].events[event_id].file = (char*) malloc(64 * sizeof (char));
    strcpy(debug_memory_array[id].events[event_id].file, __FILE__);


    // actually free
    free(ptr);
}

// 
size_t debug_mem_find_variable(void* ptr){
    size_t id;
    for (id = 0; id < debug_memory_array_count; id++){
        if (debug_memory_array[id].ptr == ptr){
            break;
        }
    }
    if (id == debug_memory_array_count){
        printf("[ERROR] VARIABLE NOT FOUND!\n");
    }
    return id;
}


void debug_mem_print_events(){
    for (size_t id=0; id < debug_memory_array_count; id++){
        printf("%p  size: %ld\n", debug_memory_array[id].ptr, debug_memory_array[id].size);

        for (size_t j=0; j < debug_memory_array[id].event_count; j++){
            struct MemoryEvent event = debug_memory_array[id].events[j];
            printf("  event: %+d  in  %s : %d  resized (bytes): %ld --> %ld\n",event.type, event.file, event.line, event.size_prev, event.size_new);
        }
    }
}


// free all the memory used for logging
void debug_mem_cleanup(){
    for (size_t id=0; id < debug_memory_array_count; id++){
        for (size_t j=0; j < debug_memory_array[id].event_count; j++){
            free(debug_memory_array[id].events[j].file);
        }
        free(debug_memory_array[id].events);
    }
}
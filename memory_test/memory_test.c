#include "debug_memory.h"
#include "stdio.h"

#define MODE_MEMORY_DEBUG
#ifdef MODE_MEMORY_DEBUG
#define malloc(n) debug_mem_malloc(n, __FILE__, __LINE__)
#define realloc(p, n) debug_mem_realloc(p, n, __FILE__, __LINE__)
#define free(p) debug_mem_free(p, __FILE__, __LINE__)
#endif


int main(int argc, const char** argv){
	debug_mem_init();

	int* array1;
	array1 = (int*) malloc(3 * (sizeof *array1));
	array1[0] = 2;
	array1[1] = 3;
	array1[2] = 7;

	array1 =  realloc(array1, 12 * (sizeof *array1));

	double* array2;
	array2 = (double*) malloc(3 * (sizeof  *array2));
	array2 = realloc(array2, 12 * (sizeof  *array2));
	array2 = realloc(array2, 24 * (sizeof  *array2));


	
	free(array2);	
	free(array1);

	debug_mem_print_events();
	debug_mem_cleanup();

	printf("test complete\n");

	return 0;
}

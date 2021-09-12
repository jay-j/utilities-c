#include <hashtable.h>

int main(){

    HashTable* ht = hash_table_create(128);

    // create and add some dummy data into
    printf("Add data1 as abc\n");
    double* data1 = (double*) malloc( 3*sizeof(double));
    data1[0] = 3;
    data1[1] = 99.4;
    data1[2] = 15;
    hash_table_insert(ht, "abc", data1);
    hash_table_print(ht);

    // add a second one
    printf("add data2 as bcd\n");
    double* data2 = (double*) malloc( 4*sizeof(double));
    hash_table_insert(ht, "bcd", data2);
    hash_table_print(ht);

    // add a third one
    printf("add data3 as creek\n");
    double* data3 = (double*) malloc( 12*sizeof(double));
    hash_table_insert(ht, "creek", data3);
    hash_table_print(ht);

    // get a pointer, test modifications
    printf("access data1, confirm data change\n");
    double* data1_access = (double*) hash_table_get(ht, "abc");
    data1_access[2] = 32;
    printf("confirm this is 32 == %lf\n", data1[2]);

    // try adding something that's already there
    printf("try adding something else at abc\n");
    double* data4 = (double*) malloc(3 * sizeof(double));
    hash_table_insert(ht, "abc", data4);
    hash_table_print(ht);

    // try removing something
    printf("remove bcd\n");
    hash_table_remove(ht, "bcd");
    hash_table_print(ht);

    // end it all
    hash_table_destroy(ht);

    return 0;
}

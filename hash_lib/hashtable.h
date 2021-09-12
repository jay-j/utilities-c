#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

typedef struct {
    char* key;
    void* value;
} HashTableItem;

typedef struct {
    size_t size;
    size_t count;
    HashTableItem* data;
} HashTable;

size_t hash(void* key, int length);
size_t hash_string_find_length(char* str);
size_t hash_string(char* str, size_t length);
HashTable* hash_table_create(size_t size);
int hash_table_insert(HashTable* ht, char* str, void* data);
int hash_table_insert_collision(HashTable* ht, char* str, void* data, size_t index_start);
void* hash_table_get(HashTable* ht, char* key);
void hash_table_remove(HashTable* ht, char* key);
void hash_table_destroy(HashTable* ht);
void hash_table_print(HashTable* ht);

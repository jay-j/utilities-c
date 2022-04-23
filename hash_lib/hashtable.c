#include "hashtable.h"

// compute hash based on the raw bytes
size_t hash(void* key, int length){
    uint8_t* key_bytes = (uint8_t*) key;
    size_t result = 5381;
    int c = *key_bytes;

    for(size_t i=0; i<length; i++){
        result = ((result << 5) + result) + c;
        key_bytes++;
        c = *key_bytes;
    }
    return result;
}


size_t hash_string_find_length(char* str){
    // find the end of the string
    size_t length=0;
    while (str[length] != '\0'){
        length++;
    }
    assert(length > 0);
    assert(length < 128);
    return length;
}


// take a string input
size_t hash_string(char* str, size_t length){
    return hash((void*) str, length);
}


// create data structures and allocate memory for them
HashTable* hash_table_create(size_t size, uint64_t flags){
    HashTable* ht;
    ht = (HashTable*) malloc( sizeof(HashTable) );
    ht->size = size;
    ht->count = 0;
    ht->flags = flags;

    HashTableItem* data = (HashTableItem*) malloc( size * sizeof(HashTableItem) );
    ht->data = data;
    for(size_t i=0; i<size; i++){
        ht->data[i].value = NULL;
    }
    return ht;
}

// TODO how to handle if the same object is added twice with different keys?
int hash_table_insert(HashTable* ht, char* str, void* data){
    size_t string_length = hash_string_find_length(str);
    size_t index = hash_string(str, string_length) % ht->size;
    int result;

    if (ht->data[index].value == NULL){
        // allocate memory to store the key string long term
        char* key = (char*) malloc( (string_length) * sizeof(char));
        strcpy(key, str);

        // now store information into the hash table
        ht->data[index].value = data;
        ht->data[index].key = key;
        ht->count++;
        result = EXIT_SUCCESS;
    }
    else{
        printf(" index %ld (key: %s) already points to %p\n", index, ht->data[index].key, ht->data[index].value);
        // printf(" comparing keys: %s to %s\n", str, ht->data[index].key);
        
        if (strcmp(str, ht->data[index].key) == 0){
            printf("[WARNING] the requested key is the same, updating the data, freeing the old one\n");
            free(ht->data[index].value);
            ht->data[index].value = data;
            result = EXIT_SUCCESS;
        }
        else{
            result = hash_table_insert_collision(ht, str, data, index);
        }
    }
    return result;
}


// search in the table forwards until a place to put the data is found
int hash_table_insert_collision(HashTable* ht, char* str, void* data, size_t index_start){
    printf("Running collision insert function.\n");
    size_t index = index_start;
    int found = 0;
    while (found==0){
        index = (index + 1) % ht->size;
        if (ht->data[index].value == NULL){
            size_t string_length = hash_string_find_length(str);
            char* key = (char*) malloc ((string_length) * sizeof(char));
            strcpy(key, str);

            ht->data[index].value = data;
            ht->data[index].key = str;
            ht->count++;
            return EXIT_SUCCESS;
        }
        if (index == index_start){
            break;
        }
    }
    return EXIT_FAILURE;
}


// return a pointer to the object stored at key
// will return null if no data is present
void* hash_table_get(HashTable* ht, char* key){
    size_t length = hash_string_find_length(key);
    size_t index = hash_string(key, length) % ht->size;
    void* result = ht->data[index].value;
    return result;
}


void hash_table_remove(HashTable* ht, char* key){
    size_t length = hash_string_find_length(key);
    size_t index = hash_string(key, length) % ht->size;
    if ((ht->flags & HT_FREE_DATA) > 0){
        free(ht->data[index].value);
    }
    if ((ht->flags & HT_FREE_KEY) > 0){
        free(ht->data[index].key);
    }
 
    ht->data[index].value = NULL;
    ht->count--;
}


void hash_table_destroy(HashTable* ht){
    // remove any objects remaining in the table
    for( size_t i=0; i<ht->size; i++){
        if (ht->data[i].value != NULL){
            if ((ht->flags & HT_FREE_DATA) > 0){
              free(ht->data[i].value);
            }
            if ((ht->flags & HT_FREE_KEY) > 0){
              free(ht->data[i].key);
            }
        }
    }

    // remove the table itself
    free(ht->data);
    free(ht);
}


void hash_table_print(HashTable* ht){
    printf("contents of hashtable index:\n");
    for(size_t i=0; i<ht->size; i++){
        if (ht->data[i].value != NULL){
            printf("\t%ld\t%s\t=\t%p\n", i, ht->data[i].key, ht->data[i].value);
        }
    }
}

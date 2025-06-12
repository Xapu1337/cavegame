#ifndef OOGABOOGA_HASH_TABLE_H
#define OOGABOOGA_HASH_TABLE_H

#include <stdint.h>
#include "base.h"

typedef struct Hash_Table Hash_Table;

Hash_Table make_hash_table_raw(uint64_t key_size, uint64_t value_size, Allocator allocator);
Hash_Table make_hash_table_reserve_raw(uint64_t key_size, uint64_t value_size, uint64_t capacity_count, Allocator allocator);
void hash_table_reset(Hash_Table *t);
void hash_table_destroy(Hash_Table *t);
void hash_table_add_raw(Hash_Table *t, uint64_t hash, void *key, void *value, uint64_t key_size, uint64_t value_size);
void *hash_table_find_raw(Hash_Table *t, uint64_t hash);

#endif

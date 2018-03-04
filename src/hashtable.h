#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <stddef.h>
#include <stdint.h>

#define HASHTABLE_KEY_TYPE uint32_t
#define HASHTABLE_VALUE_TYPE void*

// A hash code function, converting a key to a 32b integer.
typedef uint32_t (*hashtable_hash_code)(HASHTABLE_KEY_TYPE);

// A function checking for equality of keys.
typedef int (*hashtable_key_check)(HASHTABLE_KEY_TYPE, HASHTABLE_KEY_TYPE);

// A single node (either bucket or overflow) of the hashtable.
typedef struct hashtable_node_ts
{
  // Invariants:
  // State is either 0 (absent) or 1 (present).
  // The follow pointers make a null-terminated chain.
  // Each node in a chain except for the first one does not need to have its
  // state set to anything meaningful.
  uint8_t state;
  HASHTABLE_KEY_TYPE key;
  HASHTABLE_VALUE_TYPE val;
  struct hashtable_node_ts* follow;
} hashtable_node_t;

// The hashtable itself.
typedef struct
{
  // Table data.
  size_t size;
  size_t capacity;
  size_t capacity_log;
  hashtable_node_t* backend;

  // Overflow management.
  hashtable_node_t* overflow_heap;
  size_t overflow_heap_ptr;

  // Hash code function.
  hashtable_hash_code hash_code_fun;

  // Key equality check.
  hashtable_key_check key_check_fun;
} hashtable_t;

// Creates a hash table.
// The hash code function may be NULL if the HASHTABLE_KEY_TYPE can be converted
// to a uint32_t.
// If the key check is NULL, simple == equality will be checked.
void ht_create(hashtable_t*, hashtable_hash_code, hashtable_key_check);

// Destroys a hash table.
void ht_destroy(hashtable_t*);

// Insert a key into the table. Overwrites the key if already present.
// Might trigger a rehash.
void ht_insert(hashtable_t*, HASHTABLE_KEY_TYPE, HASHTABLE_VALUE_TYPE);

// Finds an entry in the hash table. Returns 0 on failure.
HASHTABLE_VALUE_TYPE ht_find(hashtable_t*, HASHTABLE_KEY_TYPE);

// Checks whether an entry is contained in the hash table.
int ht_contains(hashtable_t*, HASHTABLE_KEY_TYPE);

// Removes an entry from the hash table.
void ht_remove(hashtable_t*, HASHTABLE_KEY_TYPE);

// Fetches the size.
size_t ht_size(hashtable_t*);

#endif  // HASHTABLE_H_
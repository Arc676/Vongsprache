#include <hashtable.h>

#include <math.h>
#include <stdlib.h>

// Capacities need to be a power of two.
static const size_t INITIAL_CAPACITY = 16;
static const size_t INITIAL_CAP_LOG = 4;

// Multiplicative method factor A from [Knuth; The Art of Comp. Progr. Vol. 3]
// s = floor((1 << 32) * A)
static const uint32_t hash_s = 2654435769;

uint32_t ht_identity(uint32_t k)
{
  // Use uint32_t for the parameter to let the compiler catch mistakes.
  return k;
}

int ht_equals(HASHTABLE_KEY_TYPE x, HASHTABLE_KEY_TYPE y)
{
  return x == y;
}

// Creates a hash table.
// The hash code function may be NULL if the HASHTABLE_KEY_TYPE can be converted
// to a uint32_t.
void ht_create(hashtable_t* ht, hashtable_hash_code fun,
  hashtable_key_check chk)
{
  // INVARIANT: ht is uninitialized.
  // Setup table data.
  ht->size = 0;
  ht->capacity = INITIAL_CAPACITY;
  ht->capacity_log = INITIAL_CAP_LOG;
  ht->backend = malloc(sizeof(hashtable_node_t) * ht->capacity);
  for (size_t k = 0; k < ht->capacity; k++) {
    ht->backend[k].state = 0;
  }

  // Setup overflow management.
  ht->overflow_heap_ptr = 0;
  ht->overflow_heap = malloc(sizeof(hashtable_node_t) * ht->capacity);

  // Setup hash code function.
  if (fun == NULL) {
    ht->hash_code_fun = ht_identity;
  } else {
    ht->hash_code_fun = fun;
  }

  // Setup key check function.
  if (chk == NULL) {
    ht->key_check_fun = ht_equals;
  } else {
    ht->key_check_fun = chk;
  }
  // INVARIANT: ht is initialized.
}

// Destroys a hash table.
void ht_destroy(hashtable_t* ht)
{
  // INVARIANT: ht is initialized.
  ht->size = 0;
  ht->capacity = 0;
  free(ht->backend);
  free(ht->overflow_heap);
  ht->backend = NULL;
  ht->overflow_heap = NULL;
  // INVARIANT: ht is uninitialized.
}

size_t ht_hash(hashtable_t* ht, uint32_t key)
{
  // fwr = key * hash_s
  // Then take the capacity_log most significant bits for the hash.
  return (key * hash_s) >> (32 - ht->capacity_log);
}

size_t ht_keyfind(hashtable_t* ht, HASHTABLE_KEY_TYPE key)
{
  return ht_hash(ht, ht->hash_code_fun(key));
}

void ht_maybe_rehash(hashtable_t* ht)
{
  // INVARIANT: ht is initialized.
  // Check LoadFactor < 0.75
  if (4 * ht->size < 3 * ht->capacity) {
    // LoadFactor OK
    // Check the size of the remaining overflow heap.
    if (ht->overflow_heap_ptr < ht->capacity) {
      // Overflow heap OK
      // No rehash needed.
      return;
    }
  }

  // Rehash.
  hashtable_node_t* old_backend = ht->backend;
  hashtable_node_t* old_overflow_heap = ht->overflow_heap;
  size_t old_cap = ht->capacity;

  // Reset the table data.
  // Only increase capacity if needed. Otherwise just replenish the overflow
  // heap. For this check, a load factor exceeding 0.5 is enough.
  if (2 * ht->size >= ht->capacity) {
    ht->capacity = 2 * ht->capacity;
    ht->capacity_log++;
  }
  ht->size = 0;
  ht->backend = malloc(sizeof(hashtable_node_t) * ht->capacity);
  for (size_t k = 0; k < ht->capacity; k++) {
    ht->backend[k].state = 0;
  }

  // Reset the overflow heap.
  ht->overflow_heap_ptr = 0;
  ht->overflow_heap = malloc(sizeof(hashtable_node_t) * ht->capacity);

  // Reinsert old entries.
  for (size_t k = 0; k < old_cap; k++) {
    if (old_backend[k].state == 0) {
      continue;
    }
    hashtable_node_t* ptr = &(old_backend[k]);
    ht_insert(ht, ptr->key, ptr->val);
    while (ptr->follow != NULL) {
      ptr = ptr->follow;
      ht_insert(ht, ptr->key, ptr->val);
    }
  }

  // Free old backend and overflow heap.
  free(old_backend);
  free(old_overflow_heap);
  // INVARIANT: ht is initialized, there are empty nodes on the overflow heap.
}

// Insert a key into the table. Overwrites the key if already present.
// Might trigger a rehash.
void ht_insert(hashtable_t* ht, HASHTABLE_KEY_TYPE key,
  HASHTABLE_VALUE_TYPE val)
{
  // INVARIANT: ht is initialized.
  ht_maybe_rehash(ht);
  // INVARIANT: ht is initialized, there are empty nodes on the overflow heap.
  size_t hash = ht_keyfind(ht, key);

  // Navigate to the end of the overflow chain for that bucket, then insert.
  hashtable_node_t* ptr = &(ht->backend[hash]);
  if (ptr->state == 1) {
    // Need to check for already present.
    if (ht->key_check_fun(key, ptr->key)) {
      // First node in chain is the same: Just overwrite the value.
      ptr->val = val;
    } else {
      // Check every overflow node for key equality.
      while (ptr->follow != NULL) {
        ptr = ptr->follow;
        if (ht->key_check_fun(key, ptr->key)) {
          break;
        }
      }
      if (ht->key_check_fun(key, ptr->key)) {
        // Any other node in chain is the same: Just overwrite the value.
        ptr->val = val;
      } else {
        // No node in the chain is the same: Add a new overflow node.
        ptr->follow = &(ht->overflow_heap[ht->overflow_heap_ptr]);
        ht->overflow_heap_ptr++;
        ptr = ptr->follow;
        ptr->key = key;
        ptr->val = val;
        ptr->follow = NULL;
        ht->size++;
      }
    }
  } else {
    // Bucket is empty: Can directly insert.
    ptr->state = 1;
    ptr->key = key;
    ptr->val = val;
    ptr->follow = NULL;
    ht->size++;
  }
  // INVARIANT: ht is initialized.
}

// Finds an entry in the hash table. Returns 0 on failure.
HASHTABLE_VALUE_TYPE ht_find(hashtable_t* ht, HASHTABLE_KEY_TYPE key)
{
  // INVARIANT: ht initialized.
  size_t hash = ht_keyfind(ht, key);

  // Navigate to the correct node.
  hashtable_node_t* ptr = &(ht->backend[hash]);
  if (ptr->state == 0) {
    // Empty bucket -> not found.
    return 0;
  }
  // Advance either to the end or to the correct node.
  while (ptr-> follow != NULL && !ht->key_check_fun(key, ptr->key)) {
    ptr = ptr->follow;
  }
  if (ht->key_check_fun(key, ptr->key)) {
    return ptr->val;
  }
  return 0;
}

// Checks whether an entry is contained in the hash table.
int ht_contains(hashtable_t* ht, HASHTABLE_KEY_TYPE key)
{
  // INVARIANT: ht initialized.
  size_t hash = ht_keyfind(ht, key);

  // Navigate to the correct node.
  hashtable_node_t* ptr = &(ht->backend[hash]);
  if (ptr->state == 0) {
    // Empty bucket -> not found.
    return 0;
  }
  // Advance either to the end or to the correct node.
  while (ptr-> follow != NULL && !ht->key_check_fun(key, ptr->key)) {
    ptr = ptr->follow;
  }
  if (ht->key_check_fun(key, ptr->key)) {
    return 1;
  }
  return 0;
}

// Removes an entry from the hash table.
void ht_remove(hashtable_t* ht, HASHTABLE_KEY_TYPE key)
{
  // INVARIANT: ht initialized.
  size_t hash = ht_keyfind(ht, key);

  // Navigate to the correct node.
  hashtable_node_t* ptr = &(ht->backend[hash]);
  if (ptr->state == 0) {
    // Key not present.
    return;
  }

  // Special case: First node is target.
  if (ht->key_check_fun(key, ptr->key)) {
    if (ptr->follow == NULL) {
      // Just mark the node as not present.
      ptr->state = 0;
    } else {
      ptr->key = ptr->follow->key;
      ptr->val = ptr->follow->val;
      ptr->follow = ptr->follow->follow;
      // The second chain node is now abandoned until the next rehash.
    }
    ht->size--;
  }

  // Special case: There is no overflow list.
  if (ptr->follow == NULL) {
    // Key not present.
    return;
  }

  // The target node is in the overflow list.
  hashtable_node_t* prev = ptr;
  ptr = ptr->follow;
  while (ptr->follow != NULL && !ht->key_check_fun(key, ptr->key)) {
    ptr = ptr->follow;
    prev = prev->follow;
  }

  if (!ht->key_check_fun(key, ptr->key)) {
    // Key not present.
    return;
  }
  // Abandon the node at 'ptr'.
  prev->follow = ptr->follow;
  ht->size--;
}

size_t ht_size(hashtable_t* ht)
{
  return ht->size;
}
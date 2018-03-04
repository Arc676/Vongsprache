//Vongsprache interpreter
// Copyright (c) 2018 LordKorea
//Copyright (C) 2018 Arc676/Alessandro Vinciguerra <alesvinciguerra@gmail.com>

//Permission is hereby granted, free of charge, to any person obtaining
//a copy of this software and associated documentation files (the "Software"),
//to deal in the Software without restriction, including without limitation the
//rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
//sell copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:

//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
//FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
//COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
//IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
//CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "hashtable.h"

uint32_t ht_identity(uint32_t x) {
    return x;
}

void ht_create(hashtable_t* ht, hash_code hc) {
    // Invariant: ht not initialized.
    ht->size = 0;
    ht->capacity = 17;

    // Reserve space for the overflow heap.
    ht->overflow_ptr = 0;
    ht->overflow = malloc(sizeof(hashtable_node_t) * ht->capacity);

    // Reserve and initialize the backing array.
    ht->backend = malloc(sizeof(hashtable_node_t) * ht->capacity);
    for (size_t k = 0; k < ht->capacity; k++) {
        ht->backend[k].state = 0;
    }

    if (hc == NULL) {
        // Default hash code. Note that this requires uint32_t keys.
        ht->code_fun = ht_identity;
    } else {
        ht->code_fun = hc;
    }
}

void ht_destroy(hashtable_t* ht) {
    // Invariant: ht initialized.
    ht->size = 0;
    ht->capacity = 0;

    // Free allocated memory.
    free(ht->backend);
    free(ht->overflow);

    ht->backend = NULL;
    ht->overflow = NULL;
}

size_t ht_hash(hashtable_t* ht, uint32_t key) {
    // Invariant: ht initialized.
    return key % ht->capacity;
}

size_t ht_keyfind(hashtable_t* ht, KEY_TYPE key) {
    // Invariant: ht initialized.
    return ht_hash(ht, ht->code_fun(key));
}

void ht_maybe_rehash(hashtable_t* ht) {
    // Invariant: ht initialized.
    // Load factor 0.75 check.
    if (4 * ht->size < 3 * ht->capacity) {
        // Overflow heap remaining size check.
        if (ht->overflow_ptr < ht->capacity) {
            return;
        }
    }

    // Rehash needed.
    hashtable_node_t* old_backend = ht->backend;
    hashtable_node_t* old_overflow = ht->overflow;
    size_t old_cap = ht->capacity;

    ht->size = 0;
    ht->capacity = 2 * ht->capacity + 1;
    ht->backend = malloc(sizeof(hashtable_node_t) * ht->capacity);
    ht->overflow = malloc(sizeof(hashtable_node_t) * ht->capacity);
    ht->overflow_ptr = 0;
    for (size_t k = 0; k < ht->capacity; k++) {
        ht->backend[k].state = 0;
    }

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
    free(old_overflow);
}

void ht_insert(hashtable_t* ht, KEY_TYPE key, VALUE_TYPE value) {
    // Invariant: ht initialized.
    ht_maybe_rehash(ht);
    size_t hash = ht_keyfind(ht, key);

    // Navigate to the end of the pointer structure.
    hashtable_node_t* ptr = &(ht->backend[hash]);
    if (ptr->state == 1) {
        while (ptr->follow != NULL) {
            // All follow-nodes are guaranteed to have state 1.
            ptr = ptr->follow;
        }
    }

    // If the target is occupied add an overflow node.
    if (ptr->state == 1) {
        // Take a block from the overflow heap and move the pointer to it.
        ptr->follow = &(ht->overflow[ht->overflow_ptr]);
        ht->overflow_ptr++;
        ptr = ptr->follow;
    }

    // Put the entry into the table.
    ptr->state = 1;
    ptr->key = key;
    ptr->val = value;
    ptr->follow = NULL;

    ht->size++;
}

VALUE_TYPE ht_find(hashtable_t* ht, KEY_TYPE key) {
    // Invariant: ht initialized.
    size_t hash = ht_keyfind(ht, key);

    // Navigate to the correct node.
    hashtable_node_t* ptr = &(ht->backend[hash]);
    if (ptr->state == 0) {
        return 0;
    }

    while (ptr->follow != NULL && ptr->key != key) {
        // Follow-nodes all have state 1.
        ptr = ptr->follow;
    }

    if (ptr->key != key) {
        return 0;
    }
    return ptr->val;
}

// Checks whether an entry is contained in the hash table.
int ht_contains(hashtable_t* ht, KEY_TYPE key)
{
    // Invariant: ht initialized.
    size_t hash = ht_keyfind(ht, key);

    // Navigate to the correct node.
    hashtable_node_t* ptr = &(ht->backend[hash]);
    if (ptr->state == 0) {
        return 0;
    }

    while (ptr->follow != NULL && ptr->key != key) {
        // Follow-nodes all have state 1.
        ptr = ptr->follow;
    }

    if (ptr->key != key) {
        return 0;
    }
    return 1;
}

void ht_remove(hashtable_t* ht, KEY_TYPE key) {
    // Invariant: ht initialized.
    size_t hash = ht_keyfind(ht, key);

    // Navigate to the correct node.
    hashtable_node_t* ptr = &(ht->backend[hash]);
    if (ptr->state == 0) {
        // Key not present.
        return;
    }

    // Special case: First node is target.
    if (ptr->key == key) {
        if (ptr->follow == NULL) {
            ptr->state = 0;
        } else {
            ptr->key = ptr->follow->key;
            ptr->val = ptr->follow->val;
            ptr->follow = ptr->follow->follow;
            // ptr->follow is now abandoned until the next rehash happens.
        }
        ht->size--;
        return;
    }

    // Special case: There is no overflow list.
    if (ptr->follow == NULL) {
        // Key not present.
        return;
    }

    // The target is in the overflow list.
    hashtable_node_t* prev = ptr;
    ptr = ptr->follow;

    while (ptr->follow != NULL && ptr->key != key) {
        // Follow-nodes all have state 1.
        ptr = ptr->follow;
        prev = prev->follow;
    }

    if (ptr->key != key) {
        // Key not present.
        return;
    }

    // ptr should be deleted. This is done by just abandoning it.
    prev->follow = ptr->follow;
    ht->size--;
}

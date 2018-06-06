//Vongsprache interpreter
//Copyright (C) 2018  Arc676/Alessandro Vinciguerra <alesvinciguerra@gmail.com>
//Adapted from code by LordKorea released into public domain

///Permission is hereby granted, free of charge, to any person obtaining
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

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define HASHTABLE_KEY_TYPE void*
#define HASHTABLE_VALUE_TYPE void*

// A hash code function, converting a key to a 32b integer.
typedef uint32_t (*hashtable_hash_code)(HASHTABLE_KEY_TYPE);

// A function checking for equality of keys.
typedef int (*hashtable_key_check)(HASHTABLE_KEY_TYPE, HASHTABLE_KEY_TYPE);

// A single node (either bucket or overflow) of the hashtable.
typedef struct hashtable_node_ts {
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
typedef struct {
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

#endif

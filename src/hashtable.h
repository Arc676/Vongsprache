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

#include "tokendata.h"

#define HASHTABLE_KEY_TYPE void*
#define HASHTABLE_VALUE_TYPE void*

/**
 * A function that converts a key to an integer
 * @param key Key value
 * @return 32 bit integer hash of that key
 */
typedef uint32_t (*hashtable_hash_code)(HASHTABLE_KEY_TYPE key);

/**
 * Key equality checking function
 * @param k1 First key
 * @param k2 Second key
 * @return Whether the keys are equal
 */
typedef int (*hashtable_key_check)(HASHTABLE_KEY_TYPE k1, HASHTABLE_KEY_TYPE k2);

/**
 * A single node (either bucket or overflow) of the hashtable.
 */
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

/**
 * The hashtable itself.
 */
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

/**
 * Initializes a hash table
 * @param ht Hashtable object
 * @param hashfun Hashing function to be used by hashtable (can be NULL if the
 *                  key type is castable to uint32_t)
 * @param chkfun Equality checking function for keys (if NULL, uses !strcmp)
 */
void ht_create(hashtable_t* ht, hashtable_hash_code hashfun,
    hashtable_key_check chkfun);

/**
 * Destroys a hash table
 * @param ht Hashtable to destroy
 */
void ht_destroy(hashtable_t* ht);

/**
 * Insert a key into the table, overwriting the key if already present;
 * might trigger a rehash
 * @param ht Hashtable object
 * @param k Key value
 * @param v Value to store
 */
void ht_insert(hashtable_t* ht, HASHTABLE_KEY_TYPE k, HASHTABLE_VALUE_TYPE v);

/**
 * Converts the given token data type to string form and stores it in
 * the hashtable
 * @param ht Hashtable object
 * @param dt Data type
 * @param v Value
 */
void ht_insert_token(hashtable_t* ht, TokenDataType dt, HASHTABLE_VALUE_TYPE v);

/**
 * Finds an entry in the hashtable by the key
 * @param ht Hashtable object
 * @param k Key value
 * @return Value in the hashtable associated with the data type, 0 on failure
 */
HASHTABLE_VALUE_TYPE ht_find(hashtable_t* ht, HASHTABLE_KEY_TYPE k);

/**
 * Finds an entry in the hashtable by the token data type
 * @param ht Hashtable object
 * @param dt Data type
 * @return Value in the hashtable associated with the data type, 0 on failure
 */
HASHTABLE_VALUE_TYPE ht_find_token(hashtable_t* ht, TokenDataType dt);

/**
 * Removes an entry from the hash table
 * @param ht Hashtable object
 * @param k Key value
 */
void ht_remove(hashtable_t* ht, HASHTABLE_KEY_TYPE k);

/**
 * Determines the size of a hashtable
 * @param ht Hashtable object
 * @return Size of the hashtable
 */
size_t ht_size(hashtable_t* ht);

#endif

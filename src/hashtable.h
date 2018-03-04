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

// Hash table. Expects KEY_TYPE to be a value type.
// Adaption for non-value KEY_TYPEs should be easy.

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

// Provide defaults for key and value type.
#ifndef KEY_TYPE
#define KEY_TYPE uint32_t
#endif
#ifndef VALUE_TYPE
#define VALUE_TYPE void*
#endif

// A hash code function.
typedef uint32_t (*hash_code)(KEY_TYPE);

// A node of the table.
typedef struct hashtable_node_ts
{
    // 0: Not present 1: Present
    uint8_t state;
    KEY_TYPE key;
    VALUE_TYPE val;
    struct hashtable_node_ts* follow;
} hashtable_node_t;

// A hash table.
typedef struct
{
    // Table data.
    size_t size;
    size_t capacity;
    hashtable_node_t* backend;

    // Overflow management.
    hashtable_node_t* overflow;
    size_t overflow_ptr;

    // Hash code function.
    hash_code code_fun;
} hashtable_t;

void ht_insert(hashtable_t*, KEY_TYPE, VALUE_TYPE);

// Identity function, default hash code.
uint32_t ht_identity(uint32_t x);

// Creates a hash table.
void ht_create(hashtable_t* ht, hash_code hc);

// Destroys a hash table.
void ht_destroy(hashtable_t* ht);

// Hash function.
size_t ht_hash(hashtable_t* ht, uint32_t key);

// Finds a bucket for the given key.
size_t ht_keyfind(hashtable_t* ht, KEY_TYPE key);

// Rehashs the table if necessary.
void ht_maybe_rehash(hashtable_t* ht);

// Inserts something into the hash table.
void ht_insert(hashtable_t* ht, KEY_TYPE key, VALUE_TYPE value);

// Finds an entry in the hash table. Returns 0 on failure.
VALUE_TYPE ht_find(hashtable_t* ht, KEY_TYPE key);

// Removes an entry from the hash table.
void ht_remove(hashtable_t* ht, KEY_TYPE key);

#undef KEY_TYPE
#undef VALUE_TYPE

#endif

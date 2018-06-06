//Vongsprache interpreter
//Copyright (C) 2018  Arc676/Alessandro Vinciguerra <alesvinciguerra@gmail.com>

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

#ifndef HASHTABLE_EXTENSION_H
#define HASHTABLE_EXTENSION_H

#include <string.h>
#include "hashtable.h"

/**
 * Utility function for hashtable extension to determine in two char* keys
 * are equal using strcmp
 * @param x first key
 * @param y second key
 * @return whether the keys are equal
 */
int ht_char_equals(char* x, char* y);

/**
 * Utility hashing function for hashtable extension to convert a char* key
 * into a uint32_t to be used as a key in the hashtable
 * @param k the desired key
 * @return a uint32_t representation of the key
 */
uint32_t ht_char_hash(char* k);

#endif

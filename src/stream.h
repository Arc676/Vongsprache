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

#ifndef STREAM_H
#define STREAM_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "error.h"

/**
 * Number of line being parsed
 */
int currentlyParsingLine;

/**
 * Number of column being parsed
 */
int currentlyParsingCol;

/**
 * Peek at the next character in the stream without
 * removing it
 * @param fp FILE* from which to peek
 * @return The character at the current position in the file as an integer
 */
int peek(FILE* fp);

/**
 * Obtain the next character in the stream and remove it
 * @param fp FILE* from which to read
 * @return The next character in the stream
 */
char next(FILE* fp);

/**
 * Determine whether the file is done being read
 * @param fp FILE* to check
 * @return Whether the current character is EOF
 */
int eof(FILE* fp);

#endif

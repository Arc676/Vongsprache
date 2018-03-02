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

#ifndef PARSER_H
#define PARSER_H

#include <string>

/**
 * Number of line being parsed
 */
int currentlyParsingLine = 1;

/**
 * Number of column being parsed
 */
int currentlyParsingCol = 0;

/**
 * Peek at the next character in the stream without
 * removing it
 * @param fp FILE* from which to peek
 * @return the character at the current position in the file
 */
char peek(FILE* fp);

/**
 * Obtain the next character in the stream and remove it
 * @param fp FILE* from which to read
 * @return the next character in the stream
 */
char next(FILE* fp);

/**
 * Determine whether the file is done being read
 * @param fp FILE* to check
 * @return whether the current character is EOF
 */
int eof(FILE* fp);

/**
 * Raise an error
 * @param message error message
 */
void err(char* message);

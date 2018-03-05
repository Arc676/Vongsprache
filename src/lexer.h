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

#ifndef LEXER_H
#define LEXER_H

#include "stream.h"
#include "token.h"

/**
 * Parses the stream for the next token
 * @param fp FILE* from which to read
 * @return parsed token
 */
Token readNext(FILE* fp);

/**
 * Skips a comment in the source file by reading until newline
 * @param fp FILE* from which to read
 */
void skipComment(FILE* fp);

/**
 * Reads a string literal from the source file
 * @param fp FILE* from which to read
 * @return string literal token
 */
Token readString(FILE* fp);

/**
 * Reads a numerical literal from the source file
 * @param fp FILE* from which to read
 * @return numerical literal token
 */
Token readNumber(FILE* fp);

/**
 * Reads an identifier from the source file
 * @param fp FILE* from which to read
 * @return identifier token
 */
Token readIdentifier(FILE* fp);

#endif

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

#include <stdlib.h>

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

/**
 * Utility function for initializing tokens
 * @param type the type of token
 */
Token createToken(TokenType type);

/**
 * Utility function for creating token data
 * @param dataType the data type stored in the token
 * @param floatVal float value to store, if any
 * @param charVal string value to store, if any
 * @param tokenVal token value to store, if any
 * @return a token with the specified data
 */
TokenData* createTokenData(TokenDataType dataType, float floatVal,
    char* charVal, Token* tokenVal);

/**
 * Reads from the stream until the end character is reached without
 * being escaped by a backslash
 * @param fp FILE* from which to read
 * @param end character to end the read
 * @return the text read from the stream
 */
char* readEscaped(FILE* fp, char end);

/**
 * Determine whether a character is whitespace
 * @param c character to check
 * @return whether the character is a space, tab, or newline
 */
int isWhitespace(char c);

/**
 * Determine whether a character is a digit
 * @param c character to check
 * @return whether the character matches [0-9]
 */
int isDigit(char c);

#endif

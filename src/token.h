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

#ifndef TOKEN_H
#define TOKEN_H

#include <stdlib.h>
#include <stdio.h>

#include "hashtable.h"
#include "tokendata.h"

typedef enum TokenType {
    PUNCTUATION,
    NUMBER,
    STRING,
    KEYWORD,
    IDENTIFIER,
	OPERATOR,
    CALL,
    BINARY,
    IF,
    LOOP,
    ASSIGN,
    INIT,
	PROGRAM
} TokenType;

typedef struct {
    TokenType type;
    hashtable_t* tokenData;
} Token;

typedef union {
    float floatVal;
    char* charVal;
} TokenData;

/**
 * Utility function for initializing tokens
 * @param type The type of token
 * @return An initialized token of the given type
 */
Token* createToken(TokenType type);

/**
 * Utility function for creating token data
 * @param type The token type
 * @param floatVal Float value to store, if any
 * @param charVal String value to store, if any
 * @return A token with the specified data
 */
TokenData* createTokenData(TokenType type, float floatVal,
    char* charVal);

/**
 * Obtain a human readable string for a token type
 * @param desired Token type
 * @return Human readable name for given type
 */
char* tokenTypeToString(TokenType type);

/**
 * Stores a human readable description of a token in a given string
 * @param token Relevant token
 * @param str String into which the description should be stored
 */
 void tokenToString(Token* token, char* str);

#endif

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

typedef enum : int {
    PUNCTUATION,
    NUMBER,
    STRING,
    KEYWORD,
    IDENTIFIER,
    CALL,
    BINARY,
    IF,
    ASSIGN
} TokenType;

typedef enum : uint32_t {
    VALUE,
    ARGUMENTS,
    FUNCTION_BODY,
    FUNCTION_CALL,
    CONDITION,
    THEN_BLOCK,
    ELSE_BLOCK,
    OPERATOR,
    LEFT_VAR,
    RIGHT_VAR,
    PROG
} TokenDataType;

typedef union {
    float floatVal;
    char* charVal;
    Token* tokenVal;
} TokenData;

typedef struct {
    TokenType type;
    hashtable_t* tokenData;
} Token;

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
TokenData* createTokenData(TokenType dataType, float floatVal,
    char* charVal, Token* tokenVal);

#endif

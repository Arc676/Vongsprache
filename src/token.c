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

#include "token.h"

TokenData* createTokenData(TokenDataType type, float floatVal,
    char* charVal, Token* tokenVal) {
    TokenData* data = (TokenData*)malloc(sizeof(TokenData));
    switch (dataType) {
        case NUMBER:
            data->floatVal = floatVal;
            break;
        case STRING:
        case KEYWORD:
        case PUNCTUATION:
        case IDENTIFIER:
            data->charVal = charVal;
            break;
        case BINARY:
        case CALL:
        case ASSIGN:
        case IF:
            data->tokenVal = tokenVal;
            break;
    }
    return data;
}

Token createPunctuationToken(char punct) {
    char str[2] = {punct, 0};
    TokenData* data = createTokenData(PUNCTUATION, NULL, str, NULL);
    Token token;
    initializeToken(&token, PUNCTUATION);
    ht_insert(token.tokenData, VALUE, data);
    return token;
}

void initializeToken(Token* token, TokenType type) {
    token->type = type;
    ht_create(token->tokenData, NULL, NULL);
}

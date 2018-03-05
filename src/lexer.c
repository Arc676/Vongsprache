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

#include "lexer.h"

Token readNext(FILE* fp) {
    char current = peek(fp);
    while (isWhitespace(current)) {
        next(fp);
        current = peek(fp);
    }
    if (eof(fp)) {
        return NULL;
    }
    if (current == '#') {
        skipComment(fp);
        return readNext(fp);
    }
    if (current == '"') {
        return readString(fp);
    }
    if (isDigit(current)) {
        return readNumber(fp);
    }
    char msg[30];
    sprintf(msg, "Parsing failed on character %c", current);
    err(msg);
}

Token readString(FILE* fp) {
    char* literal = readEscaped(fp, '"');
    TokenData* data = createTokenData(STRING, NULL, literal, NULL);
    Token token;
    initializeToken(&token, STRING);
    ht_insert(token.tokenData, VALUE, data);
    return token;
}

Token readNumber(FILE* fp) {
    int hasDot = 0;
    char* literal = malloc(100);
    memset(literal, 0, sizeof(literal));
    int pos = 0;
    char c = peek(fp);
    while (1) {
        if (c == '.') {
            if (hasDot) {
                break;
            }
            hasDot = 1;
            literal[pos++] = c;
        } else if (isDigit(c)) {
            literal[pos++] = c;
        }
    }
    float number = (float)strtol(literal, (char**)NULL, 0);
    free(literal);
    TokenData* data = createTokenData(NUMBER, number, NULL, NULL);
    Token token;
    initializeToken(&token, NUMBER);
    ht_insert(token.tokenData, VALUE, data);
    return token;
}

TokenData* createTokenData(TokenDataType dataType, float floatVal,
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
        case VARIABLE:
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

void initializeToken(Token* token, TokenType type) {
    token->type = type;
    ht_create(token->tokenData, NULL, NULL);
}

void skipComment(FILE* fp) {
    char c = peek(fp);
    while (c != '\n') {
        next(fp);
        c = peek(fp);
    }
    next(fp);
}

int isWhitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}

int isDigit(char c) {
    return c >= '0' && c <= '9';
}

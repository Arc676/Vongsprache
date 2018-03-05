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

const char** keywords = {
    "i",
    "bims",
    "vong",
    "bis",
    "mit",
    "her",
    "Funktionigkeit",
    "am",
    "Wahrigkeit",
    "Sonstigkeit",
    "solange",
};

const char* punctuation = "()[]{},"

Token readNext(FILE* fp) {
    char current = peek(fp);
    while (isWhitespace(current)) {
        next(fp);
        current = peek(fp);
    }
    if (eof(fp)) {
        return NULL;
    }
    next(fp);
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
    if (isValidIDStart(current)) {
        return readIdentifier(fp);
    }
    if (isPunc(current)) {
        return createPunctuationToken(current);
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

Token readIdentifier(FILE* fp) {
    char* str = malloc(100);
    memset(str, 0, sizeof(str));
    int pos = 0;
    while (!eof(fp)) {
        char c = next(fp);
        if (isValidIDChar(c)) {
            str[pos++] = c;
        }
    }
    TokenType type = isKeyword(str) ? KEYWORD : IDENTIFIER;
    TokenData* data = createTokenData(type, 0, str, NULL);
    Token token;
    initializeToken(&token, type);
    ht_insert(token.tokenData, VALUE, data);
    return token;
}

char* readEscaped(FILE* fp, char end) {
    int escaped = 0;
    char* str = malloc(100);
    int pos = 0;
    while (!eof(fp)) {
        char c = next(fp);
        if (pos >= sizeof(str)) {
            str = realloc(sizeof(str) + 100);
        }
        if (escaped) {
            str[pos++] = c;
        } else if (c == '\\') {
            escaped = 1;
        } else if (c == end) {
            break;
        } else {
            str[pos++] = c;
        }
    }
    str[pos] = 0;
    return str;
}

void skipComment(FILE* fp) {
    char c = peek(fp);
    while (c != '\n') {
        next(fp);
        c = peek(fp);
    }
    next(fp);
}

Token lexer_peek(FILE* fp) {
    return current || (current = readNext(fp));
}

Token lexer_next(FILE* fp) {
    Token token = current;
    current = NULL;
    return token || readNext(fp);
}

int lexer_eof(FILE* fp) {
    return lexer_peek(fp) == NULL;
}

int isKeyword(char* str) {
    for (int i = 0; i < KEYWORD_COUNT; i++) {
        if (!strcmp(str, keywords[i])) {
            return 1;
        }
    }
    return 0;
}

int isValidIDStart(char c) {
    return c == '_' || isalpha(c);
}

int isValidIDChar(char c) {
    return isValidIDStart(c) || isDigit(c);
}

int isWhitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}

int isDigit(char c) {
    return c >= '0' && c <= '9';
}

int isPunc(char c) {
    for (int i = 0; i < strlen(punctuation); i++) {
        if (c == punctuation[i]) {
            return 1;
        }
    }
    return 0;
}

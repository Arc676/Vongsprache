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

const char* keywords[KEYWORD_COUNT] = {
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
	"bidde"
};

const char* punctuation = "()[]{},";
const char* opChars = "%+-*/=&|<>!";
Token* currentToken = NULL;

Token* readNext(FILE* fp) {
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
    if (isValidIDStart(current)) {
        return readIdentifier(fp);
    }
    if (isPunc(current)) {
        char str[2] = {current, 0};
        TokenData* data = createTokenData(PUNCTUATION, 0, str, NULL);
        Token* token = createToken(PUNCTUATION);
        ht_insert_token(token->tokenData, VALUE, data);
        return token;
    }
    if (isOpChar(current)) {
        char* operator = (char*)malloc(5);
        readWhile(fp, operator, 5, isOpChar);
        TokenData* data = createTokenData(OPERATOR, 0, operator, NULL);
        Token* token = createToken(OPERATOR);
        ht_insert_token(token->tokenData, VALUE, data);
        return token;
    }
    char msg[30];
    sprintf(msg, "Parsing failed on character %c", current);
    err(msg, PARSE_ERROR);
	return NULL;
}

Token* readString(FILE* fp) {
    char* literal = readEscaped(fp, '"');
    TokenData* data = createTokenData(STRING, 0, literal, NULL);
    Token* token = createToken(STRING);
    ht_insert_token(token->tokenData, VALUE, data);
    return token;
}

Token* readNumber(FILE* fp) {
    int hasDot = 0;
    char* literal = (char*)malloc(100);
    memset(literal, 0, 100);
    int pos = 0;
    while (1) {
    	char c = next(fp);
        if (c == '.') {
            if (hasDot) {
                break;
            }
            hasDot = 1;
            literal[pos++] = c;
        } else if (isDigit(c)) {
            literal[pos++] = c;
        } else {
			break;
		}
    }
    float number = strtof(literal, (char**)NULL);
    free(literal);
    TokenData* data = createTokenData(NUMBER, number, NULL, NULL);
    Token* token = createToken(NUMBER);
    ht_insert_token(token->tokenData, VALUE, data);
    return token;
}

Token* readIdentifier(FILE* fp) {
    char* str = (char*)malloc(100);
    readWhile(fp, str, 100, isValidIDChar);
    TokenType type = isKeyword(str) ? KEYWORD : IDENTIFIER;
    TokenData* data = createTokenData(type, 0, str, NULL);
    Token* token = createToken(type);
    ht_insert_token(token->tokenData, VALUE, data);
    return token;
}

char* readEscaped(FILE* fp, char end) {
    next(fp);
    int escaped = 0;
	size_t size = 100;
    char* str = (char*)malloc(size);
    int pos = 0;
    while (!eof(fp)) {
        char c = next(fp);
        if (pos >= size) {
			size += 100;
            str = realloc(str, size);
        }
        if (escaped) {
            str[pos++] = c;
			escaped = 0;
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

void readWhile(FILE* fp, char* str, size_t size, int (*valid)(char)) {
    memset(str, 0, size);
    int pos = 0;
    while (!eof(fp) && valid(peek(fp))) {
        str[pos++] = next(fp);
    }
}

void skipComment(FILE* fp) {
    char c = peek(fp);
    while (c != '\n') {
        next(fp);
        c = peek(fp);
    }
    next(fp);
}

Token* lexer_peek(FILE* fp) {
	if (currentToken) {
		return currentToken;
	}
	currentToken = readNext(fp);
    return currentToken;
}

Token* lexer_next(FILE* fp) {
    Token* token = currentToken;
    currentToken = NULL;
	if (token) {
		return token;
	}
    return readNext(fp);
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
    return charInString(c, punctuation);
}

int isOpChar(char c) {
    return charInString(c, opChars);
}

int charInString(char c, const char* str) {
    for (int i = 0; i < strlen(str); i++) {
        if (c == str[i]) {
            return 1;
        }
    }
    return 0;
}

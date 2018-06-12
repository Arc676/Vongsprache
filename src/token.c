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

TokenData* createTokenData(TokenType type, float floatVal,
    char* charVal) {
    TokenData* data = (TokenData*)malloc(sizeof(TokenData));
    switch (type) {
        case NUMBER:
            data->floatVal = floatVal;
            break;
        case STRING:
        case KEYWORD:
        case PUNCTUATION:
        case IDENTIFIER:
    	case OPERATOR:
            data->charVal = charVal;
            break;
    	default:
            break;
    }
    return data;
}

Token* createToken(TokenType type) {
	Token* token = (Token*)malloc(sizeof(Token));
    token->type = type;
	token->tokenData = (hashtable_t*)malloc(sizeof(hashtable_t));
    ht_create(token->tokenData, NULL, NULL);
	return token;
}

char* tokenTypeToString(TokenType type) {
	switch (type) {
		case PUNCTUATION:
			return "Satzzeichen";
	    case NUMBER:
			return "Zahl";
	    case STRING:
			return "Zeichenfolge";
	    case KEYWORD:
			return "Schlüsselwort";
	    case IDENTIFIER:
			return "Identifikator";
		case OPERATOR:
			return "Operator";
	    case CALL:
			return "Funktionanruf";
	    case BINARY:
			return "Binarischer Ausdruck";
	    case IF:
			return "Bedingte Anweisungsblock";
        case LOOP:
            return "Schleife";
	    case ASSIGN:
			return "Zuweisung";
        case INIT:
            return "Initialisierung";
        case RETURN:
            return "Rückkehranweisung";
	case INCLUDE:
		return "Einfügung";
		case PROGRAM:
			return "Anweisungfolge";
	}
}

void tokenToString(Token* token, char* str) {
    char s[50];
	sprintf(s, "%s-Token", tokenTypeToString(token->type));
	TokenData* data = ht_find_token(token->tokenData, VALUE);
	switch (token->type) {
        case NUMBER:
            sprintf(str, "%s (%f)", s, data->floatVal);
            break;
        case STRING:
        case KEYWORD:
        case PUNCTUATION:
        case IDENTIFIER:
		case OPERATOR:
            sprintf(str, "%s (\"%s\")", s, data->charVal);
            break;
    	default:
            sprintf(str, "%s", s);
    		break;
    }
}

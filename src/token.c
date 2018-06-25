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
#include "util.h"

extern int currentCol;
extern int currentLine;

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
    // allocate memory for new token
	Token* token = (Token*)malloc(sizeof(Token));
    // initialize token type and hashtable
    token->type = type;
	token->tokenData = (hashtable_t*)malloc(sizeof(hashtable_t));
    ht_create(token->tokenData, NULL, NULL);
    // store line and column number at which token was created
    TokenData* line = createTokenData(NUMBER, currentLine, NULL);
    TokenData* col = createTokenData(NUMBER, currentCol, NULL);
    ht_insert_token(token->tokenData, LINE_NUM, line);
    ht_insert_token(token->tokenData, COL_NUM, col);
	return token;
}

Token* copyToken(Token* token) {
    if (!token) {
        return NULL;
    }
    Token* copy = createToken(token->type);
    TokenData* original = ht_find_token(token->tokenData, VALUE);
    TokenData* dataCopy;
    switch (token->type) {
        case STRING:
        case KEYWORD:
        case PUNCTUATION:
        case IDENTIFIER:
        case OPERATOR:
            dataCopy = createTokenData(token->type, 0, copyString(original->charVal));
            break;
        default:
            dataCopy = createTokenData(token->type, original->floatVal, NULL);
            break;
    }
    ht_insert_token(copy->tokenData, VALUE, dataCopy);
    return copy;
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
        case CONTINUE:
            return "Iterationsabbruch";
        case BREAK:
            return "Schleifenabbruch";
    	case INCLUDE:
    		return "Einfügung";
        case FUNC_WRAPPER:
            return "Funktionswrapper";
		case PROGRAM:
			return "Anweisungsfolge";
	}
}

void tokenToString(Token* token, char* str) {
    if (!token) {
        sprintf(str, "(null)");
        return;
    }
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

void destroyToken(Token* token) {
    // for safety, prevent destruction of any null token or a token
    // whose token data has already been destroyed
    if (!token || !token->tokenData) {
        return;
    }
    TokenData* value = ht_find_token(token->tokenData, VALUE);
    int count = value ? (int)value->floatVal : 0;
    for (int i = 0; i <= VALUE; i++) {
        TokenDataType datatype = (TokenDataType)i;
        void* stored = ht_find_token(token->tokenData, datatype);
        if (stored) {
            switch (datatype) {
                case LINE_NUM:
                case COL_NUM:
                    free((TokenData*)stored);
                    break;
                case VALUE:
                    switch (token->type) {
                        case LOOP:
                        case RETURN:
                            destroyToken((Token*)stored);
                            break;
                        case PUNCTUATION:
                        case OPERATOR:
                        case STRING:
                        case IDENTIFIER:
                        case KEYWORD:
                        case INCLUDE:
                        {
                            TokenData* data = (TokenData*)stored;
                            free(data->charVal);
                            free(data);
                            break;
                        }
                        default:
                            free((TokenData*)stored);
                            break;
                    }
                    break;
                case OP:
                    switch (token->type) {
                        case BINARY:
                            destroyToken((Token*)stored);
                            break;
                        default:
                            free((TokenData*)stored);
                            break;
                    }
                    break;
                case ARGUMENTS:
                {
                    switch (token->type) {
                        case CALL:
                        {
                            Token** args = (Token**)stored;
                            for (int i = 0; i < count; i++) {
                                destroyToken(args[i]);
                            }
                            free(args);
                            break;
                        }
                        case FUNC_WRAPPER:
                        {
                            char** args = (char**)stored;
                            for (int i = 0; i < count; i++) {
                                free(args[i]);
                            }
                            free(args);
                            break;
                        }
                        default:
                            destroyToken((Token*)stored);
                            break;
                    }
                    break;
                }
                case FUNCTION_BODY:
                    switch (token->type) {
                        case PROGRAM:
                        {
                            Token** statements = (Token**)stored;
                            for (int i = 0; i < count; i++) {
                                destroyToken(statements[i]);
                            }
                            free(statements);
                            break;
                        }
                        default:
                            destroyToken((Token*)stored);
                            break;
                    }
                    break;
                default:
                    destroyToken((Token*)stored);
                    break;
            }
        }
    }
    ht_destroy(token->tokenData);
    free(token->tokenData);
    token->tokenData = NULL;
    free(token);
}

int isLiteralToken(Token* token) {
    return token && (token->type == NUMBER || token->type == STRING);
}

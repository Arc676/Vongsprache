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

#include "builtins.h"

Token* vongsprache_print(int argc, Token** args, Scope* scope) {
	for (int i = 0; i < argc; i++) {
		Token* val = eval(args[i], scope);
		TokenData* valData = ht_find_token(val->tokenData, VALUE);
		switch (val->type) {
			case STRING:
				printf("%s", valData->charVal);
				break;
			case NUMBER:
				printf("%f", valData->floatVal);
				break;
			default:
			{
				char msg[100];
				tokenToString(val, msg);
				printf("%s", msg);
				break;
			}
		}
	}
	printf("\n");
	return NULL;
}

Token* vongsprache_input(int argc, Token** args, Scope* scope) {
	vongsprache_print(argc, args, scope);
	char* input = malloc(255);
	fgets(input, 255, stdin);
	input[strlen(input) - 1] = 0;
	TokenData* data = createTokenData(STRING, 0, input);
	Token* token = createToken(STRING);
	ht_insert_token(token->tokenData, VALUE, data);
	return token;
}

Token* vongsprache_toFloat(int argc, Token** args, Scope* scope) {
	char msg[100];
	if (argc != 1) {
		sprintf(msg, "1 Argument für Funktion %s erwartet aber %d gefunden",
				fID, argc);
		err(msg, BAD_ARG_COUNT);
		break;
	}
	Token* given = eval(args[0], scope);

	sprintf(msg, "Erwartete %sArgument, %s gefunden",
			(toString ? "numerisches " : "Zeichenfolge-"),
			tokenTypeToString(args[0]->type));

	TokenData* current = ht_find_token(given->tokenData, VALUE);
	TokenData* newData;
	Token* ret = createToken(toString ? STRING : NUMBER);
	if (given->type != STRING) {
		err(msg, BAD_ARG_TYPE);
		break;
	}
	float fVal = strtof(current->charVal, (char**)NULL);
	newData = createTokenData(NUMBER, fVal, NULL);
	ht_insert_token(ret->tokenData, VALUE, newData);
	return ret;
}

Token* vongsprache_toString(int argc, Token** args, Scope* scope) {
	char msg[100];
	if (argc != 1) {
		sprintf(msg, "1 Argument für Funktion %s erwartet aber %d gefunden",
				fID, argc);
		err(msg, BAD_ARG_COUNT);
		break;
	}
	Token* given = eval(args[0], scope);

	sprintf(msg, "Erwartete %sArgument, %s gefunden",
			(toString ? "numerisches " : "Zeichenfolge-"),
			tokenTypeToString(args[0]->type));

	TokenData* current = ht_find_token(given->tokenData, VALUE);
	TokenData* newData;
	Token* ret = createToken(toString ? STRING : NUMBER);
	if (given->type != NUMBER) {
		err(msg, BAD_ARG_TYPE);
		break;
	}
	char* strVal = (char*)malloc(20);
	sprintf(strVal, "%f", current->floatVal);
	newData = createTokenData(STRING, 0, strVal);
	ht_insert_token(ret->tokenData, VALUE, newData);
	return ret;
}

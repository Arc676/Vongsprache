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

const char* builtinFunctions[BUILTIN_COUNT] = {
	"drucke",
	"gib",
	"zuZahl",
	"zuZeichenfolge",
	"piMalDaumen",
	"samen"
};

BUILTIN *builtins[BUILTIN_COUNT] = {
	vongsprache_print,
	vongsprache_input,
	vongsprache_toFloat,
	vongsprache_toString,
	vongsprache_rand,
	vongsprache_srand
};

int RNGInitialized = 0;

Token* vongsprache_print(int argc, Token** args) {
	for (int i = 0; i < argc; i++) {
		Token* val = args[i];
		TokenData* valData = ht_find_token(val->tokenData, VALUE);
		switch (val->type) {
			case STRING:
				printf("%s", valData->charVal);
				break;
			case NUMBER:
			{
				float integral;
				if (modff(valData->floatVal, &integral) == 0) {
					printf("%d", (int)valData->floatVal);
				} else {
					printf("%f", valData->floatVal);
				}
				break;
			}
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

Token* vongsprache_input(int argc, Token** args) {
	vongsprache_print(argc, args);
	char* input = (char*)malloc(255);
	fgets(input, 255, stdin);
	input[strlen(input) - 1] = 0;
	TokenData* data = createTokenData(STRING, 0, input);
	Token* token = createToken(STRING);
	ht_insert_token(token->tokenData, VALUE, data);
	return token;
}

void typeConversionUtil(TokenType srcType, TokenType dstType, TokenData** current,
						Token** ret, Token* given, char* fID, int argc) {
	char msg[100];
	if (argc != 1) {
		sprintf(msg, "1 Argument für Funktion %s erwartet aber %d gefunden",
				fID, argc);
		err(msg, BAD_ARG_COUNT);
	}

	argTypeChk(srcType, given->type);

	*current = ht_find_token(given->tokenData, VALUE);
	*ret = createToken(dstType);
}

Token* vongsprache_toFloat(int argc, Token** args) {
	TokenData* current;
	Token* ret;
	typeConversionUtil(STRING, NUMBER, &current, &ret,
						args[0], "zuZahl", argc);

	float fVal = strtof(current->charVal, (char**)NULL);
	TokenData* newData = createTokenData(NUMBER, fVal, NULL);
	ht_insert_token(ret->tokenData, VALUE, newData);
	return ret;
}

Token* vongsprache_toString(int argc, Token** args) {
	TokenData* current;
	Token* ret;
	typeConversionUtil(NUMBER, STRING, &current, &ret,
						args[0], "zuZeichenfolge", argc);

	char* strVal = (char*)malloc(20);
	sprintf(strVal, "%f", current->floatVal);
	TokenData* newData = createTokenData(STRING, 0, strVal);
	ht_insert_token(ret->tokenData, VALUE, newData);
	return ret;
}

Token* vongsprache_srand(int argc, Token** args) {
	int seed;
	char msg[100];
	if (argc == 0) {
		seed = time(NULL);
	} else if (argc == 1) {
		Token* given = args[0];
		argTypeChk(NUMBER, given->type);
		TokenData* data = ht_find_token(given->tokenData, VALUE);
	} else {
		sprintf(msg, "0 oder 1 Argument für Funktion samen erwartet aber %d gefunden",
				argc);
		err(msg, BAD_ARG_COUNT);
		return NULL;
	}
	srand(seed);
	return NULL;
}

Token* vongsprache_rand(int argc, Token** args) {
	char msg[100];
	if (argc != 1) {
		sprintf(msg, "1 Argument für Funktion piMalDaumen erwartet aber %d gefunden",
				argc);
		err(msg, BAD_ARG_COUNT);
		return NULL;
	}
	Token* bound = args[0];
	argTypeChk(NUMBER, bound->type);
	TokenData* data = ht_find_token(bound->tokenData, VALUE);
	Token* ret = createToken(NUMBER);
	int rngVal = rand() % (int)data->floatVal;
	TokenData* retVal = createTokenData(NUMBER, rngVal, NULL);
	ht_insert_token(ret->tokenData, VALUE, retVal);
	return ret;
}

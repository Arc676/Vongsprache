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

#include <stdio.h>

#include "parser.h"
#include "scope.h"

Token* eval(Token* exp, Scope* scope) {
	switch (exp->type) {
		case NUMBER:
		case STRING:
			return exp;
		case IDENTIFIER:
		{
			TokenData* data = ht_find_token(exp->tokenData, VALUE);
			return getVariable(scope, data->charVal);
		}
		case ASSIGN:
		{
			TokenData* left = ht_find_token(exp->tokenData, LEFT_VAR);
			Token* leftVal = left->tokenVal;
			if (leftVal->type == IDENTIFIER) {
				left = ht_find_token(leftVal->tokenData, VALUE);
				TokenData* right = ht_find_token(exp->tokenData, RIGHT_VAR);
				return setVariable(scope, left->charVal,
									eval(right->tokenVal, scope));
			} else {
				char msg[100];
				tokenToString(leftVal, msg);
				sprintf(msg, "Dem Zeichen %s kann nicht zugeschrieben werden", msg);
				err(msg, ASSIGN_FAILED);
			}
		}
		default:
		{
			char msg[100];
			sprintf(msg, "Zeichen mit unerwartetem Typ %s gefunden",
					tokenTypeToString(exp-type));
			err(msg, UNKNOWN_TOKEN_TYPE);
		}
	}
	return NULL;
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Gebrauch: vongsprache Datei\n");
		return 1;
	}
	FILE* file = fopen(argv[1], "r");
	if (!file) {
		fprintf(stderr, "Datei konnte nicht geöffnet werden\n");
		return 1;
	}
	Token* topLevel = parseTopLevel(file);
	fclose(file);
	return 0;
}

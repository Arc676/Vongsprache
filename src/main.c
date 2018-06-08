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

#include "main.h"

void undeclaredIDErr(char* ID) {
	char msg[100];
	sprintf(msg, "Undefinierter Identifikator: %s", ID);
	err(msg, UNDECLARED_IDENTIFIER);
}

void vongsprache_print(int argc, Token** args, Scope* scope) {
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
}

Token* eval(Token* exp, Scope* scope) {
	switch (exp->type) {
		case NUMBER:
		case STRING:
			return exp;
		case IDENTIFIER:
		{
			TokenData* data = ht_find_token(exp->tokenData, VALUE);
			Token* val = getVariable(scope, data->charVal);
			if (val) {
				return val;
			}
			undeclaredIDErr(data->charVal);
			break;
		}
		case INIT:
		case ASSIGN:
		{
			Token* leftVal = ht_find_token(exp->tokenData, LEFT_VAR);
			switch (leftVal->type) {
				case IDENTIFIER:
				case CALL:
				{
					TokenData* left = ht_find_token(leftVal->tokenData, VALUE);
					Token* right = ht_find_token(exp->tokenData, RIGHT_VAR);
					if (leftVal->type == IDENTIFIER) {
						if (exp->type == ASSIGN) {
							Token* exists = getVariable(scope, left->charVal);
							if (!exists) {
								char msg[100];
								char token[100];
								tokenToString(leftVal, token);
								sprintf(msg, "Undeklariertes Token %s kann nicht zugewiesen werden",
										token);
								err(msg, ASSIGN_FAILED);
								return NULL;
							}
						}
						return setVariable(scope, left->charVal, eval(right, scope));
					} else {
						return setVariable(scope, left->charVal, right);
					}
				}
				default:
				{
					char msg[100];
					char token[100];
					tokenToString(leftVal, token);
					sprintf(msg, "Token %s kann nicht zugewiesen werden", token);
					err(msg, ASSIGN_FAILED);
					break;
				}
			}
		}
		case IF:
		{
			Token* cond = ht_find_token(exp->tokenData, CONDITION);
			Token* res = eval(cond, scope);
			int evalThen = res != NULL;
			if (evalThen && res->type == NUMBER) {
				TokenData* data = ht_find_token(res->tokenData, VALUE);
				evalThen = data->floatVal != 0;
			}
			if (evalThen) {
				Token* thenblk = ht_find_token(exp->tokenData, THEN_BLOCK);
				return eval(thenblk, scope);
			} else {
				Token* elseblk = ht_find_token(exp->tokenData, ELSE_BLOCK);
				if (elseblk) {
					return eval(elseblk, scope);
				}
				return NULL;
			}
		}
		case PROGRAM:
		{
			Scope* childScope = createScope(scope);
			Token* val = NULL;

			TokenData* data = ht_find_token(exp->tokenData, VALUE);
			int count = (int)data->floatVal;

			Token** statements = ht_find_token(exp->tokenData, FUNCTION_BODY);

			for (int i = 0; i < count; i++) {
				val = eval(statements[i], childScope);
			}
			return val;
		}
		case CALL:
		{
			Token* funcId = ht_find_token(exp->tokenData, FUNCTION_CALL);
			TokenData* funcIDData = ht_find_token(funcId->tokenData, VALUE);
			char* fID = funcIDData->charVal;

			Token** args = ht_find_token(exp->tokenData, ARGUMENTS);
			TokenData* data = ht_find_token(exp->tokenData, VALUE);
			int argc = (int)data->floatVal;

			if (!strcmp(fID, "drucke") || !strcmp(fID, "gib")) {
				vongsprache_print(argc, args, scope);
				if (!strcmp(fID, "gib")) {
					char* input = malloc(255);
					fgets(input, 255, stdin);
					input[strlen(input) - 1] = 0;
					TokenData* data = createTokenData(STRING, 0, input);
					Token* token = createToken(STRING);
					ht_insert_token(token->tokenData, VALUE, data);
					return token;
				}
			} else {
				Token* func = getVariable(scope, fID);
				if (func) {
					TokenData* fArgs = ht_find_token(func->tokenData, VALUE);
					int req = (int)fArgs->floatVal;
					// check correct number of arguments given
					if (argc != req) {
						char msg[100];
						sprintf(msg, "%d Argument%s für Funktion %s erwartet aber %d gefunden",
								req, (req == 1 ? "" : "e"), fID, argc);
						err(msg, BAD_ARG_COUNT);
						break;
					}
					char** params = ht_find_token(func->tokenData, ARGUMENTS);
					Scope* fScope = createScope(scope);
					for (int i = 0; i < argc; i++) {
						setVariable(fScope, params[i], eval(args[i], scope));
					}
					Token* fBody = ht_find_token(func->tokenData, FUNCTION_BODY);
					return eval(fBody, fScope);
				}
				undeclaredIDErr(fID);
				break;
			}
			break;
		}
		case LOOP:
		{
			Token* cond = ht_find_token(exp->tokenData, CONDITION);
			Token* prog = ht_find_token(exp->tokenData, FUNCTION_BODY);
			while (eval(cond, scope)) {
				eval(prog, scope);
			}
			break;
		}
		default:
		{
			char msg[100];
			sprintf(msg, "Token mit unerwartetem Typ %s gefunden",
					tokenTypeToString(exp->type));
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
	Token* ast = parseTopLevel(file);
	// create global scope when running main function
	eval(ast, NULL);
	fclose(file);
	return 0;
}

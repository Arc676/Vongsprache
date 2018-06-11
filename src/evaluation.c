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

#include "evaluation.h"

Token* applyOp(Token* op, Token* left, Token* right) {
	TokenData* lData = ht_find_token(left->tokenData, VALUE);
	TokenData* rData = ht_find_token(right->tokenData, VALUE);

	// are the operands numbers?
	int n1 = left->type == NUMBER;
	int n2 = right->type == NUMBER;

	void* v1 = n1 ? (void*)&(lData->floatVal) : (void*)lData->charVal;
	void* v2 = n2 ? (void*)&(rData->floatVal) : (void*)rData->charVal;

	TokenData* data = ht_find_token(op->tokenData, OP);
	int opType = (int)data->floatVal;

	// prepare error message
	char msg[100];
	sprintf(msg, "Falsche Datentypen %s und %s zum Operator '%s' gegeben",
			tokenTypeToString(left->type), tokenTypeToString(right->type),
			operators[opType]);

	// determine the requirements of the operator
	// default to requiring numbers
	int canOperate = n1 && n2;
	switch (opType) {
		// division requires nonzero
		case GETEILT:
		case REST:
			if (canOperate && *(float*)v2 == 0) {
				// overwrite error message
				sprintf(msg, "Kann nicht durch null teilen");
				canOperate = 0;
			}
			break;
		// these operators require operands of the same type
		case GLEICH:
			canOperate = n1 == n2;
		default:
			break;
	}

	if (canOperate) {
		float result;
		switch (opType) {
			case PLUS:
				result = *(float*)v1 + *(float*)v2;
				break;
			case MINUS:
				result = *(float*)v1 - *(float*)v2;
				break;
			case MAL:
				result = *(float*)v1 * *(float*)v2;
				break;
			case GETEILT:
				result = *(float*)v1 / *(float*)v2;
				break;
			case REST:
				result = fmod(*(float*)v1, *(float*)v2);
				break;
			case HOCH:
				result = powf(*(float*)v1, *(float*)v2);
				break;
			case GROESSER_ODER_GLEICH:
				result = *(float*)v1 >= *(float*)v2;
				break;
			case GROESSER_ALS:
				result = *(float*)v1 > *(float*)v2;
				break;
			case KLEINER_ODER_GLEICH:
				result = *(float*)v1 <= *(float*)v2;
				break;
			case KLEINER_ALS:
				result = *(float*)v1 < *(float*)v2;
				break;
			case GLEICH:
				if (n1) {
					result = *(float*)v1 == *(float*)v2;
				} else {
					result = !strcmp((char*)v1, (char*)v2);
				}
				break;
			case ODER:
				result = evalBool(left) || evalBool(right);
				break;
			case UND:
				result = evalBool(left) && evalBool(right);
				break;
			default:
				break;
		}
		Token* res = createToken(NUMBER);
		TokenData* data = createTokenData(NUMBER, result, NULL);
		ht_insert_token(res->tokenData, VALUE, data);
		return res;
	}
	// display error if cannot operate
	err(msg, OPERATOR_ERROR);
	return NULL;
}

int evalBool(Token* val) {
	int ret = val != NULL;
	if (ret && val->type == NUMBER) {
		TokenData* data = ht_find_token(val->tokenData, VALUE);
		ret = data->floatVal != 0;
	}
	return ret;
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
		case BINARY:
		{
			Token* op = ht_find_token(exp->tokenData, OP);
			Token* left = ht_find_token(exp->tokenData, LEFT_VAR);
			Token* right = ht_find_token(exp->tokenData, RIGHT_VAR);
			return applyOp(
				op,
				eval(left, scope),
				eval(right, scope)
			);
		}
		case IF:
		{
			Token* cond = ht_find_token(exp->tokenData, CONDITION);
			Token* res = eval(cond, scope);
			if (evalBool(res)) {
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
			// if in global scope, don't create a child scope
			Scope* childScope = scope;
			if (!isGlobalScope(scope)) {
				childScope = createScope(scope);
			}
			Token* val = NULL;

			TokenData* data = ht_find_token(exp->tokenData, VALUE);
			int count = (int)data->floatVal;

			Token** statements = ht_find_token(exp->tokenData, FUNCTION_BODY);

			for (int i = 0; i < count; i++) {
				val = eval(statements[i], childScope);
				// stop evaluating block if a return statement has been given
				if (getVariable(childScope, "hab")) {
					break;
				}
			}
			// destroy child scope unless it's the global scope
			if (childScope != scope) {
				destroyScope(childScope);
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
			} else if (!strcmp(fID, "zuZahl") || !strcmp(fID, "zuZeichenfolge")) {
				int toString = !strcmp(fID, "zuZeichenfolge");

				// prepare error message
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
				if (toString) {
					if (given->type != NUMBER) {
						err(msg, BAD_ARG_TYPE);
						break;
					}
					char* strVal = (char*)malloc(20);
					sprintf(strVal, "%f", current->floatVal);
					newData = createTokenData(STRING, 0, strVal);
				} else {
					if (given->type != STRING) {
						err(msg, BAD_ARG_TYPE);
						break;
					}
					float fVal = (float)strtol(current->charVal, (char**)NULL, 0);
					newData = createTokenData(NUMBER, fVal, NULL);
				}
				ht_insert_token(ret->tokenData, VALUE, newData);
				return ret;
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

					// create child scope for function evaluation
					Scope* fScope = createFuncScope(scope);

					// assign given arguments to parameter names, shadowing
					// any variables in parent scopes with the same name
					for (int i = 0; i < argc; i++) {
						defineVariable(fScope, params[i], eval(args[i], scope));
					}
					Token* fBody = ht_find_token(func->tokenData, FUNCTION_BODY);
					Token* ret = eval(fBody, fScope);
					destroyScope(fScope);
					return ret;
				}
				undeclaredIDErr(fID);
			}
			break;
		}
		case LOOP:
		{
			Token* cond = ht_find_token(exp->tokenData, CONDITION);
			Token* prog = ht_find_token(exp->tokenData, FUNCTION_BODY);

			// create child scope for loop
			Scope* lScope = createScope(scope);

			// determine if loop has a loop counter
			Token* counter = ht_find_token(exp->tokenData, VALUE);
			TokenData* counterVal;
			if (counter) {
				TokenData* cID = ht_find_token(counter->tokenData, VALUE);
				Token* start = ht_find_token(exp->tokenData, ARGUMENTS);

				// evaluate start value for counter
				Token* startVal = eval(start, scope);
				TokenData* cVal = ht_find_token(startVal->tokenData, VALUE);

				// copy start value to new token in case it's a literal
				Token* startCopy = createToken(NUMBER);
				counterVal = createTokenData(NUMBER, cVal->floatVal, NULL);
				ht_insert_token(startCopy->tokenData, VALUE, counterVal);

				// define loop counter in child scope
				defineVariable(lScope, cID->charVal, startCopy);
			}

			while (evalBool(eval(cond, lScope))) {
				eval(prog, lScope);
				if (counter) {
					counterVal->floatVal += 1;
				}
			}

			destroyScope(lScope);
			break;
		}
		case RETURN:
		{
			// can only return if inside a function
			if (!getVariable(scope, "Funktionigkeit")) {
				unexpected(exp);
			}
			Token* val = ht_find_token(exp->tokenData, VALUE);
			Token* ret = eval(val, scope);

			// find the stack frame in which the function started
			Scope* fScope = scope;
			while (!isFuncScope(fScope)) {
				fScope = fScope->parentScope;
			}
			setVariable(fScope, "hab", createToken(IDENTIFIER));
			return ret;
		}
		default:
			unexpected(exp);
			break;
	}
	return NULL;
}

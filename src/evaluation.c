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

extern int currentCol;
extern int currentLine;

extern char* libpath;

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
			case UNGLEICH:
				if (n1) {
					result = *(float*)v1 != *(float*)v2;
				} else {
					result = strcmp((char*)v1, (char*)v2) != 0;
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

Token* eval(Token* exp, Scope* scope) {
	TokenData* position;
	position = ht_find_token(exp->tokenData, LINE_NUM);
	currentLine = (int)position->floatVal;
	position = ht_find_token(exp->tokenData, COL_NUM);
	currentCol = (int)position->floatVal;
	Token* returnValue = NULL;
	switch (exp->type) {
		case NUMBER:
		case STRING:
			returnValue = exp;
			break;
		case IDENTIFIER:
		{
			TokenData* data = ht_find_token(exp->tokenData, VALUE);
			Token* val = getVariable(scope, data->charVal);
			if (val) {
				returnValue = val;
			} else {
				undeclaredIDErr(data->charVal);
			}
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

					// copy identifier
					char* id = copyString(left->charVal);

					// identifier lvalues indicate variable assignment
					if (leftVal->type == IDENTIFIER) {
						// assignment requires that the variable already exists
						if (exp->type == ASSIGN) {
							Token* exists = getVariable(scope, id);
							if (exists) {
								returnValue = setVariable(scope, id, eval(right, scope));
							} else {
								char msg[100];
								char token[100];
								tokenToString(leftVal, token);
								sprintf(msg, "Undeklariertes Token %s kann nicht zugewiesen werden",
										token);
								err(msg, ASSIGN_FAILED);
							}
						} else {
							// initialization calls shadow any parent scope variables
							returnValue = defineVariable(scope, id, eval(right, scope));
						}
					} else {
						// if assigning a function instead, don't evaluate right side
						returnValue = defineVariable(scope, id, right);
					}
					break;
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
			break;
		}
		case BINARY:
		{
			Token* op = ht_find_token(exp->tokenData, OP);
			Token* left = ht_find_token(exp->tokenData, LEFT_VAR);
			Token* right = ht_find_token(exp->tokenData, RIGHT_VAR);
			returnValue = applyOp(
				op,
				eval(left, scope),
				eval(right, scope)
			);
			break;
		}
		case IF:
		{
			Token* cond = ht_find_token(exp->tokenData, CONDITION);
			Token* res = eval(cond, scope);
			int evalThen = evalBool(res);
			if (!isLiteralToken(cond)) {
				destroyToken(res);
			}
			if (evalThen) {
				Token* thenblk = ht_find_token(exp->tokenData, THEN_BLOCK);
				returnValue = eval(thenblk, scope);
			} else {
				Token* elseblk = ht_find_token(exp->tokenData, ELSE_BLOCK);
				if (elseblk) {
					returnValue = eval(elseblk, scope);
				}
			}
			break;
		}
		case PROGRAM:
		{
			// if in global scope, don't create a child scope
			Scope* childScope = scope;
			if (!isGlobalScope(scope)) {
				childScope = createScope(scope);
			}

			TokenData* data = ht_find_token(exp->tokenData, VALUE);
			int count = (int)data->floatVal;

			Token** statements = ht_find_token(exp->tokenData, FUNCTION_BODY);

			for (int i = 0; i < count; i++) {
				returnValue = eval(statements[i], childScope);
				// stop evaluating block if any control flow statement has been given
				if (getVariable(childScope, "hab") ||
					getVariable(childScope, "aufgeben") ||
					getVariable(childScope, "durchmarsch")) {
					break;
				}
			}
			// destroy child scope unless it's the global scope
			if (childScope != scope) {
				// if present, copy the return value so it doesn't get
				// destroyed with the scope
				if (returnValue) {
					returnValue = copyToken(returnValue);
				}
				destroyScope(childScope);
			}
			break;
		}
		case CALL:
		{
			Token* funcId = ht_find_token(exp->tokenData, FUNCTION_CALL);
			TokenData* funcIDData = ht_find_token(funcId->tokenData, VALUE);
			char* fID = funcIDData->charVal;

			// obtain arguments given to function
			Token** givenArgs = ht_find_token(exp->tokenData, ARGUMENTS);
			TokenData* data = ht_find_token(exp->tokenData, VALUE);
			int argc = (int)data->floatVal;

			// store evaluated function arguments separate from originals
			Token** args = malloc(argc * sizeof(Token*));

			// prepare for return
			int hasEvaluated = 0;

			// evaluate function arguments before passing to function
			for (int i = 0; i < argc; i++) {
				args[i] = copyToken(eval(givenArgs[i], scope));
			}

			for (int i = 0; i < BUILTIN_COUNT; i++) {
				if (!strcmp(fID, builtinFunctions[i])) {
					returnValue = builtins[i](argc, args);
					hasEvaluated = 1;

					// tear down copy of arguments
					for (int i = 0; i < argc; i++) {
						if (!isLiteralToken(args[i])) {
							destroyToken(args[i]);
						}
					}
				}
			}

			if (!hasEvaluated) {
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
					Scope* fScope = createFrameScope(scope, CALL);

					// assign given arguments to parameter names, shadowing
					// any variables in parent scopes with the same name
					for (int i = 0; i < argc; i++) {
						defineVariable(fScope, copyString(params[i]), args[i]);
					}
					Token* fBody = ht_find_token(func->tokenData, FUNCTION_BODY);
					returnValue = eval(fBody, fScope);

					// tear down child scope and arguments contained within it
					destroyScope(fScope);
					hasEvaluated = 1;
				}
			}

			free(args);

			if (!hasEvaluated) {
				undeclaredIDErr(fID);
			}
			break;
		}
		case LOOP:
		{
			Token* cond = ht_find_token(exp->tokenData, CONDITION);
			Token* prog = ht_find_token(exp->tokenData, FUNCTION_BODY);

			// create child scope for loop
			Scope* lScope = createFrameScope(scope, LOOP);

			// determine if loop has a loop counter
			Token* counter = ht_find_token(exp->tokenData, VALUE);
			TokenData* counterVal;
			if (counter) {
				// evaluate start value for counter
				Token* start = ht_find_token(exp->tokenData, ARGUMENTS);
				Token* startVal = eval(start, scope);

				// copy start value to new token in case it's a literal
				Token* startCopy = copyToken(startVal);
				counterVal = ht_find_token(startCopy->tokenData, VALUE);

				// if it's NOT a literal, destroy original copy of start value
				if (!isLiteralToken(start)) {
					destroyToken(startVal);
				}

				// define loop counter in child scope
				TokenData* cID = ht_find_token(counter->tokenData, VALUE);
				defineVariable(lScope, copyString(cID->charVal), startCopy);
			}

			while (1) {
				Token* res = eval(cond, lScope);
				int evalBlock = evalBool(res);
				if (!isLiteralToken(cond)) {
					destroyToken(res);
				}
				if (!evalBlock) {
					break;
				}
				returnValue = eval(prog, lScope);
				// stop loop if a break or return statement has been given
				if (getVariable(lScope, "aufgeben") ||
					getVariable(lScope, "hab")) {
					break;
				}
				// finishing stages of loop
				if (counter) {
					counterVal->floatVal += 1;
				}
				// if a CONTINUE statement has been given, erase
				// if before the next iteration
				deleteVariable(lScope, "durchmarsch");
			}

			if (returnValue) {
				returnValue = copyToken(returnValue);
			}
			destroyScope(lScope);
			break;
		}
		case RETURN:
		{
			// can only return if inside a function
			Scope* fFrame = lookupScope(scope, "Funktionigkeit");
			if (!fFrame) {
				unexpected(exp);
			}

			// find the stack frame in which the function started
			Scope* fScope = scope;
			while (fScope->parentScope != fFrame) {
				fScope = fScope->parentScope;
			}

			Token* val = ht_find_token(exp->tokenData, VALUE);
			returnValue = eval(val, scope);
			setVariable(fScope, copyString("hab"), createToken(IDENTIFIER));
			break;
		}
		case BREAK:
		case CONTINUE:
		{
			// can only break/continue if inside a loop
			Scope* lFrame = lookupScope(scope, "solange");
			if (!lFrame) {
				unexpected(exp);
			}

			if (exp->type == BREAK) {
				setVariable(lFrame, copyString("aufgeben"), createToken(IDENTIFIER));
			} else {
				setVariable(lFrame, copyString("durchmarsch"), createToken(IDENTIFIER));
			}
			break;
		}
		case INCLUDE:
		{
			TokenData* data = ht_find_token(exp->tokenData, VALUE);
			char filename[100];
			sprintf(filename, "%s/%s.vong", libpath, data->charVal);
			FILE* included = fopen(filename, "r");
			if (!included) {
				char msg[300];
				sprintf(msg, "Einzufügende Datei %s konnte nicht geöffnet werden",
					filename);
				err(msg, INCLUDE_FAILED);
				break;
			}
			Token* ast = parseTopLevel(included);
			fclose(included);
			returnValue = eval(ast, scope);
			break;
		}
		default:
			unexpected(exp);
			break;
	}
	return returnValue;
}

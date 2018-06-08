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

#include "parser.h"

Token* parseTopLevel(FILE* fp) {
	Token* program = createToken(PROGRAM);
	size_t size = INITIAL_STATEMENT_COUNT * sizeof(Token*);
	Token** progs = (Token**)malloc(size);
	memset(progs, 0, size);

	int pos = 0;
	while (!eof(fp)) {
		if (pos >= size * sizeof(Token*)) {
			size += INITIAL_STATEMENT_COUNT * sizeof(Token*);
			progs = (Token**)realloc(progs, size);
		}
		Token* token = parseExpression(fp);
		if (token) {
			progs[pos++] = token;
		}
	}
	TokenData* data = createTokenData(NUMBER, pos, NULL);
	ht_insert_token(program->tokenData, VALUE, data);
	ht_insert_token(program->tokenData, FUNCTION_BODY, progs);
	return program;
}

Token* parseIf(FILE* fp) {
	Token* cond = parseExpression(fp);
	skipValue(fp, KEYWORD, 2, "vong", "Wahrigkeit");
	Token* then = parseProg(fp);
	Token* ret = createToken(IF);
	ht_insert_token(ret->tokenData, CONDITION, cond);
	ht_insert_token(ret->tokenData, THEN_BLOCK, then);
	skipValue(fp, KEYWORD, 1, "her");
	if (parser_isValue(fp, KEYWORD, "am")) {
		skipValue(fp, KEYWORD, 2, "am", "Sonstigkeit");
		Token* els = parseProg(fp);
		ht_insert_token(ret->tokenData, ELSE_BLOCK, els);
		skipValue(fp, KEYWORD, 1, "her");
	}
	return ret;
}

Token* parseProg(FILE* fp) {
	size_t size = INITIAL_FUNCTION_STATEMENT_COUNT * sizeof(Token*);
	Token** statements = (Token**)malloc(size);
	memset(statements, 0, size);

	int pos = 0;
	int blockClosed = 0;
	while (!eof(fp)) {
		if (parser_isValue(fp, KEYWORD, "her")) {
			lexer_next(fp);
			blockClosed = 1;
			break;
		}
		if (pos >= size / sizeof(Token*)) {
			size += INITIAL_FUNCTION_STATEMENT_COUNT * sizeof(Token*);
			Token** new = (Token**)realloc(statements, size);
			if (new) {
				statements = new;
			} else {
				err("Arbeitsspeicherplatz ungenügend", MEMORY_ERROR);
				return NULL;
			}
		}
		statements[pos++] = parseExpression(fp);
	}
	if (!blockClosed) {
		err("Geöffneter Anweisungsblock nicht geschlossen", EXPECTED_TOKEN);
		return NULL;
	}
	Token* prog = createToken(PROGRAM);
	ht_insert_token(prog->tokenData, FUNCTION_BODY, statements);
	TokenData* data = createTokenData(NUMBER, pos, NULL);
	ht_insert_token(prog->tokenData, VALUE, data);
	return prog;
}

Token* parseIdentifier(FILE* fp) {
	Token* var = lexer_next(fp);
	if (var->type != IDENTIFIER) {
		err("Erwartete Variablenamen", PARSE_ERROR);
		return NULL;
	}
	return var;
}

Token* parseCall(FILE* fp) {
	Token* token = createToken(CALL);
	Token* funcIdentifier = lexer_next(fp);
	if (funcIdentifier->type == IDENTIFIER) {
		ht_insert_token(token->tokenData, FUNCTION_CALL, funcIdentifier);
		TokenData* data = createTokenData(NUMBER, 0, NULL);
		if (parser_isValue(fp, KEYWORD, "mit")) {
			lexer_next(fp);
			int argc;
			Token** args = parseDelimited(fp, "(", ")", ",", &argc, parseExpression);
			data->floatVal = (float)argc;
			ht_insert_token(token->tokenData, ARGUMENTS, args);
		}
		ht_insert_token(token->tokenData, VALUE, data);
		return token;
	}
	err("Erwartete Funktionidentifikator", EXPECTED_TOKEN);
	return NULL;
}

Token* parseExpression(FILE* fp) {
	if (parser_isValue(fp, KEYWORD, "i")) {
		skipValue(fp, KEYWORD, 2, "i", "bims");
		Token* identifier = lexer_next(fp);
		if (identifier->type != IDENTIFIER) {
			err("Erwartete Identifikator", EXPECTED_TOKEN);
			return NULL;
		}
		skipValue(fp, KEYWORD, 1, "vong");
		Token* token = createToken(ASSIGN);
		ht_insert_token(token->tokenData, LEFT_VAR, identifier);
		if (parser_isValue(fp, KEYWORD, "Funktionigkeit")) {
			lexer_next(fp);
			identifier->type = CALL;
			Token* function = createToken(PROGRAM);
			TokenData* fArgs = createTokenData(NUMBER, 0, NULL);
			if (parser_isValue(fp, KEYWORD, "mit")) {
				lexer_next(fp);
				int argc;
				Token** args = parseDelimited(fp, "(", ")", ",", &argc, parseIdentifier);
				fArgs->floatVal = (float)argc;
				char** argNames = (char**)malloc(argc * sizeof(char*));
				for (int i = 0; i < argc; i++) {
					argNames[i] = malloc(100);
					TokenData* data = ht_find_token(args[i]->tokenData, VALUE);
					char* argName = data->charVal;
					memcpy(argNames[i], argName, strlen(argName));
					free(args[i]);
				}
				free(args);
				ht_insert_token(function->tokenData, ARGUMENTS, argNames);
			}
			ht_insert_token(function->tokenData, VALUE, fArgs);

			Token* fBody = parseProg(fp);
			ht_insert_token(function->tokenData, FUNCTION_BODY, fBody);

			ht_insert_token(token->tokenData, RIGHT_VAR, function);
		} else {
			Token* right = parseExpression(fp);
			ht_insert_token(token->tokenData, RIGHT_VAR, right);

			skipValue(fp, KEYWORD, 1, "her");
		}
		return token;
	}
	if (parser_isValue(fp, KEYWORD, "bims")) {
		lexer_next(fp);
		return parseIf(fp);
	}
	if (parser_isValue(fp, KEYWORD, "bidde")) {
		lexer_next(fp);
		return parseCall(fp);
	}
	Token* token = lexer_next(fp);
	if (token == NULL) {
		return NULL;
	}
	switch (token->type) {
		case IDENTIFIER:
		case STRING:
		case NUMBER:
			return token;
		default:
			unexpected(token);
			return NULL;
	}
}

Token** parseDelimited(FILE* fp, char* start, char* end, char* sep, int* count,
	Token* (*parse)(FILE*)) {
	int first = 1;
	size_t size = INITIAL_DELIM_COUNT * sizeof(Token*);
	Token** tokens = (Token**)malloc(size);

	int pos = 0;
	skipValue(fp, PUNCTUATION, 1, start);
	while (!eof(fp)) {
		if (parser_isValue(fp, PUNCTUATION, end)) {
			break;
		}
		if (pos >= size / sizeof(Token*)) {
			size += INITIAL_DELIM_COUNT * sizeof(Token*);
			Token** new = (Token**)realloc(tokens, size);
			if (new) {
				tokens = new;
			} else {
				err("Arbeitsspeicherplatz ungenügend", MEMORY_ERROR);
				return NULL;
			}
		}
		first ? first = 0 : skipValue(fp, PUNCTUATION, 1, sep);
		if (parser_isValue(fp, PUNCTUATION, end)) {
			break;
		}
		tokens[pos++] = parse(fp);
	}
	skipValue(fp, PUNCTUATION, 1, end);
	*count = pos;
	return tokens;
}

int parser_isValue(FILE* fp, TokenType type, char* value) {
	Token* token = lexer_peek(fp);
	if (token && token->type == type) {
		TokenData* data = ht_find_token(token->tokenData, VALUE);
		return !strcmp(data->charVal, value);
	}
	return 0;
}

void skipValue(FILE* fp, TokenType type, int count, ...) {
	va_list arglist;
	va_start(arglist, count);
	for (int i = 0; i < count; i++) {
		char* value = va_arg(arglist, char*);
		if (parser_isValue(fp, type, value)) {
			lexer_next(fp);
		} else {
			va_end(arglist);
			char token[100];
			char msg[200];
			tokenToString(lexer_peek(fp), token);
			sprintf(msg, "Erwartete %s-Token: \"%s\", %s gefunden",
				tokenTypeToString(type), value, token);
			err(msg, EXPECTED_TOKEN);
		}
	}
	va_end(arglist);
}

void unexpected(Token* token) {
	char msg[200];
	char tk[100];
	tokenToString(token, tk);
	sprintf(msg, "Unerwartetes %s", tk);
	err(msg, UNEXPECTED_TOKEN);
}

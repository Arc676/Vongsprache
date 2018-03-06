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
		Token* token = parseExpression(fp);
		progs[pos++] = token;
	}
	ht_insert(program->tokenData, PROG, progs);
	return program;
}

Token* parseIf(FILE* fp) {
	skipValue(fp, KEYWORD, 1, "bims");
	Token* cond = parseExpression(fp);
	skipValue(fp, KEYWORD, 2, "vong", "Wahrigkeit");
	Token* then = parseProg(fp);
	Token* ret = createToken(IF);
	ht_insert(ret->tokenData, CONDITION, cond);
	ht_insert(ret->tokenData, THEN_BLOCK, then);
	if (parser_isValue(fp, KEYWORD, "am")) {
		skipValue(fp, KEYWORD, 1, "Sonstigkeit");
		Token* els = parseProg(fp);
		ht_insert(ret->tokenData, ELSE_BLOCK, els);
	}
	return ret;
}

char* parseVariableName(FILE* fp) {
	Token* var = lexer_next(fp);
	if (var->type != IDENTIFIER) {
		err("Expected variable name", PARSE_ERROR);
	}
	TokenData* data = ht_find(var->tokenData, VALUE);
	return data->charVal;
}

Token** parseDelimited(FILE* fp, char* start, char* end, char* sep,
	Token* (*parse)(FILE*)) {
	int first = 0;
	size_t size = INITIAL_DELIM_COUNT * sizeof(Token*);
	Token** tokens = (Token**)malloc(size);
	memset(tokens, 0, size);

	skipValue(fp, PUNCTUATION, 1, start);
	while (!eof(fp)) {
		if (parser_isValue(fp, PUNCTUATION, end)) {
			break;
		}
		if (first) {
			first = 0;
		} else {
			skipValue(fp, PUNCTUATION, 1, sep);
		}
		if (parser_isValue(fp, PUNCTUATION, end)) {
			break;
		}
		Token* token = parse(fp);
	}
	skipValue(fp, PUNCTUATION, 1, end);
	return tokens;
}

int parser_isValue(FILE* fp, TokenType type, char* value) {
	Token* token = lexer_peek(fp);
	if (token && token->type == type) {
		TokenData* data = ht_find(token->tokenData, VALUE);
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
			Token* token = lexer_peek(fp);
			char msg[100];
			sprintf(msg, "Expected %s token: \"%s\", found %s", tokenTypeToString(type),
				value, tokenTypeToString(token->type));
			err(msg, EXPECTED_TOKEN);
		}
	}
	va_end(arglist);
}

void unexpected(Token* token) {
	char msg[100];
	sprintf(msg, "Unexpected %s token: ", tokenTypeToString(token->type));
	TokenData* data = ht_find(token->tokenData, VALUE);
	if (token->type == NUMBER) {
		sprintf(msg, "%s%d", msg, data->floatVal);
	} else {
		sprintf(msg, "%s%s", msg, data->charVal);
	}
	err(msg, UNEXPECTED_TOKEN);
}

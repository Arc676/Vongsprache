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

#include "scope.h"

Scope* createScope(Scope* parent) {
	Scope* scope = (Scope*)malloc(sizeof(Scope));
	scope->parentScope = parent;
	scope->variables = (hashtable_t*)malloc(sizeof(hashtable_t));
	ht_create(scope->variables, NULL, NULL);
	scope->identifierCount = 0;
	scope->storageSize = INITIAL_IDENTIFIER_COUNT * sizeof(char*);
	scope->storedIdentifiers = (char**)malloc(scope->storageSize);
	return scope;
}

void destroyScope(Scope* scope) {
	for (int i = 0; i < scope->identifierCount; i++) {
		char* identifier = scope->storedIdentifiers[i];
		if (identifier) {
			Token* token = ht_find(scope->variables, identifier);
			destroyToken(token);
			free(identifier);
		}
	}
	free(scope->storedIdentifiers);
	ht_destroy(scope->variables);
	free(scope->variables);
	free(scope);
}

Scope* createFrameScope(Scope* parent, TokenType type) {
	Scope* fScope = createScope(parent);
	Token* frame = createToken(IDENTIFIER);
	char* id;
	switch (type) {
		case CALL:
			id = copyString("Funktionigkeit");
			break;
		case LOOP:
			id = copyString("solange");
			break;
		case PROGRAM:
			id = copyString("i");
			break;
		default:
			err("Ungültiger Umfangstyp", BAD_ARG_TYPE);
			break;
	}
	defineVariable(fScope, id, frame);
	return fScope;
}

Scope* lookupScope(Scope* scope, char* identifier) {
	Scope* found = scope;
	while (found) {
		if (ht_find(found->variables, identifier)) {
			return found;
		}
		found = found->parentScope;
	}
	return NULL;
}

Token* getVariable(Scope* scope, char* identifier) {
	Scope* declaration = lookupScope(scope, identifier);
	if (declaration) {
		return ht_find(declaration->variables, identifier);
	}
	return NULL;
}

Token* setVariable(Scope* scope, char* identifier, Token* value) {
	Scope* defined = lookupScope(scope, identifier);
	if (!defined) {
		defined = scope;
	}
	return defineVariable(defined, identifier, value);
}

Token* defineVariable(Scope* scope, char* identifier, Token* value) {
	if (scope->identifierCount > scope->storageSize / sizeof(char*)) {
		scope->storageSize += INITIAL_IDENTIFIER_COUNT * sizeof(char*);
		scope->storedIdentifiers = (char**)resize(
			scope->storedIdentifiers,
			scope->storageSize
		);
	}
	scope->storedIdentifiers[scope->identifierCount++] = identifier;
	Token* existed = ht_find(scope->variables, identifier);
	if (existed) {
		destroyToken(existed);
	}
	ht_insert(scope->variables, identifier, value);
	return value;
}

int deleteVariable(Scope* scope, char* identifier) {
	if (lookupScope(scope, identifier) == scope) {
		Token* val = ht_find(scope->variables, identifier);
		destroyToken(val);
		ht_remove(scope->variables, identifier);
		return 1;
	}
	return 0;
}

int isGlobalScope(Scope* scope) {
	return lookupScope(scope, "i") == scope;
}

int isFuncScope(Scope* scope) {
	return lookupScope(scope, "Funktionigkeit") == scope->parentScope;
}

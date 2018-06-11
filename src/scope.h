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

#ifndef SCOPE_H
#define SCOPE_H

#include <stdlib.h>

#include "hashtable.h"
#include "token.h"

typedef struct Scope {
	struct Scope* parentScope;
	hashtable_t* variables;
} Scope;

/**
 * Create a new scope object with the given parent scope
 * @param parent Desired parent scope
 * @return The newly created scope object
 */
Scope* createScope(Scope* parent);

/**
 * Free all memory occupied by a given scope
 * @param scope Scope to destroy
 */
void destroyScope(Scope* scope);

/**
 * Utility function to create a new function scope
 * @param parent Parent scope
 * @return Newly created and appropriately initialized scope object
 */
Scope* createFuncScope(Scope* parent);

/**
 * Utility function to create a new global scope
 * @return Newly created and appropriately initialized scope object
 */
Scope* createGlobalScope();

/**
 * Find the scope in which a variable is declared
 * @param scope Scope from which to start searching
 * @param identifier Variable identifier
 * @return Scope in which variable is declared, NULL if none
 */
Scope* lookupScope(Scope* scope, char* identifier);

/**
 * Get the token object representing a given variable
 * @param scope Scope in which to search
 * @param identifier Variable identifier
 * @return Variable object
 */
Token* getVariable(Scope* scope, char* identifier);

/**
 * Sets the value of a given variable
 * @param scope Scope in which variable should be defined
 * @param identifier Variable identifier
 * @param value The new value for the variable
 * @return The new value of the variable, NULL if setting failed
 */
Token* setVariable(Scope* scope, char* identifier, Token* value);

/**
 * Define a new variable in a scope
 * @param scope Scope in which to define the variable
 * @param identifier Variable identifier
 * @param value Initial value to give to the variable
 * @return Value of the new variable, NULL if the variable already existed
 */
Token* defineVariable(Scope* scope, char* identifier, Token* value);

/**
 * Determine if the given scope is the global scope
 * @param scope Scope to check
 * @return Whether the given scope is the global scope
 */
int isGlobalScope(Scope* scope);

/**
 * Determine if the given scope marks the beginning of a frame in which
 * a function was called
 * @param scope Scope to check
 * @return Whether the given scope is a function scope
 */
int isFuncScope(Scope* scope);

#endif

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
#include <math.h>

#include "parser.h"
#include "scope.h"

/**
 * Raise an error caused by the use of an undeclared identifier
 * @param ID Relevant identifier
 */
void undeclaredIDErr(char* ID);

/**
 * Print function for Vongsprache; prints the contents of the passed arguments
 * to the console with a trailing newline
 * @param argc Number of arguments passed to print function
 * @param args Arguments passed to the print function (as tokens)
 * @param scope Scope in which the function was called
 */
void vongsprache_print(int argc, Token** args, Scope* scope);

/**
 * Applies a binary operator
 * @param op Operator
 * @param left Left value
 * @param right Right value
 * @return Result of operation
 */
Token* applyOp(Token* op, Token* left, Token* right);

/**
 * Determines if a token should evaluate to a truthy value
 * @param val Token to check
 * @return Boolean value of the evaluated token
 */
int evalBool(Token* val);

/**
 * Evaluates a token
 * @param exp Token to evaluate
 * @param scope Scope in which to evaluate the token
 * @return A token representing the return value of the evaluation
 */
Token* eval(Token* exp, Scope* scope);

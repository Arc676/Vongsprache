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

#ifndef BUILTINS_H
#define BUILTINS_H

#include "scope.h"

/**
 * Builtin print function; prints the contents of the passed arguments
 * to the console with a trailing newline
 * @param argc Number of arguments passed to print function
 * @param args Arguments passed to the print function (as tokens)
 * @param scope Scope in which the function was called
 * @return NULL
 */
Token* vongsprache_print(int argc, Token** args, Scope* scope);

/**
 * Builtin console input function; passes its arguments to
 * vongsprache_print and then reads user input and returns it
 * as a string token
 * @param argc Number of arguments passed
 * @param args Arguments passed
 * @param scope Scope in which the function was called
 * @return User input as a string token
 */
Token* vongsprache_input(int argc, Token** args, Scope* scope);

/**
 * Builtin string-to-float conversion function; checks argument for
 * a string value and creates a new token with a corresponding float value
 * @param argc Number of arguments passed
 * @param args Arguments passed
 * @param scope Scope in which the function was called
 * @return New token with float value of given argument
 */
Token* vongsprache_toFloat(int argc, Token** args, Scope* scope);

/**
 * Builtin float-to-string conversion function; checks argument for
 * a float value and creates a new token with a string containing that value
 * @param argc Number of arguments passed
 * @param args Arguments passed
 * @param scope Scope in which the function was called
 * @return New token with string form of given float argument
 */
Token* vongsprache_toString(int argc, Token** args, Scope* scope);

#endif

//Vongsprache interpreter
//Copyright (C) 2018-22  Arc676/Alessandro Vinciguerra <alesvinciguerra@gmail.com>

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

#include <time.h>
#include <math.h>

#include "parser.h"
#include "scope.h"
#include "error.h"

#define BUILTIN_COUNT 8

extern const char* builtinFunctions[BUILTIN_COUNT];

typedef Token* BUILTIN(int, Token**);
extern BUILTIN *builtins[BUILTIN_COUNT];

/**
 * Builtin print function; prints the contents of the passed arguments
 * to the console with a trailing newline
 * @param argc Number of arguments passed to print function
 * @param args Arguments passed
 * @return NULL
 */
Token* vongsprache_print(int argc, Token** args);

/**
 * Builtin console input function; passes its arguments to
 * vongsprache_print and then reads user input and returns it
 * as a string token
 * @param argc Number of arguments passed
 * @param args Arguments passed
 * @return User input as a string token
 */
Token* vongsprache_input(int argc, Token** args);

/**
 * Utility function containing common base code for type conversion functions
 * @param srcType Expected argument type
 * @param dstType Token type of return value
 * @param current Double pointer in which to store the argument's current value
 * @param ret Double pointer in which to store the newly created token to return
 * @param given Evaluated argument
 * @param fID Identifier of called function
 * @param argc Number of arguments passed
 */
void typeConversionUtil(TokenType srcType, TokenType dstType, TokenData** current,
						Token** ret, Token* given, char* fID, int argc);

/**
 * Builtin string-to-float conversion function; checks argument for
 * a string value and creates a new token with a corresponding float value
 * @param argc Number of arguments passed
 * @param args Arguments passed
 * @return New token with float value of given argument
 */
Token* vongsprache_toFloat(int argc, Token** args);

/**
 * Builtin float-to-string conversion function; checks argument for
 * a float value and creates a new token with a string containing that value
 * @param argc Number of arguments passed
 * @param args Arguments passed
 * @return New token with string form of given float argument
 */
Token* vongsprache_toString(int argc, Token** args);

/**
 * Builtin help function; launches an interactive help shell if no arguments
 * are given or attempts to find a help file whose name matches the given
 * single argument
 * @param argc Number of arguments passed
 * @param args Arguments passed
 * @return A numerical token indicating whether the operation succeeded
 */
Token* vongsprache_help(int argc, Token** args);

/**
 * Builtin program termination function; optionally takes an exit code
 * as argument
 * @param argc Number of arguments passed
 * @param args Arguments passed
 * @return NULL
 */
Token* vongsprache_exit(int argc, Token** args);

/**
 * Builtin RNG seed setting function; sets the seed for random number
 * generation to the given seed or the current UNIX time if none is given
 * @param argc Number of arguments passed
 * @param args Arguments passed
 * @return NULL
 */
Token* vongsprache_srand(int argc, Token** args);

/**
 * Builtin random number generation function; returns a random integer
 * between zero and the given numerical argument minus one
 * @param argc Number of arguments passed
 * @param args Arguments passed
 * @return New token with a random number
 */
Token* vongsprache_rand(int argc, Token** args);

#endif

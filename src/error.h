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

#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>

#include "token.h"

// use error code range 64-113 to avoid conflict with standard exit codes
#define PARSE_ERROR 64
#define EXPECTED_TOKEN 65
#define UNEXPECTED_TOKEN 66
#define BAD_ARG_TYPE 67
#define ASSIGN_FAILED 68
#define MEMORY_ERROR 69
#define UNDECLARED_IDENTIFIER 70
#define BAD_ARG_COUNT 71
#define OPERATOR_ERROR 72
#define INCLUDE_FAILED 73

/**
 * Raise an error
 * @param message Error message
 * @param code The exit code
 */
void err(char* message, int code);

/**
 * Throw an error because the current token is an unexpected token
 * @param token The unexpected token
 */
void unexpected(Token* token);

/**
 * Raise an error caused by the use of an undeclared identifier
 * @param ID Relevant identifier
 */
void undeclaredIDErr(char* ID);

#endif

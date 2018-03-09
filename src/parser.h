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

#ifndef PARSER_H
#define PARSER_H

#include <stdarg.h>

#include "lexer.h"

#define INITIAL_STATEMENT_COUNT 100
#define INITIAL_DELIM_COUNT 10
#define INITIAL_FUNCTION_STATEMENT_COUNT 10

/**
 * Top-level parse function to parse the whole script
 * @param fp FILE* from which to read
 * @return program token containing the script
 */
Token* parseTopLevel(FILE* fp);

/**
 * Parse a function from the token stream
 * @param fp FILE* from which to read
 * @return function token containing the defined function
 */
Token* parseFunction(FILE* fp);

/**
 * Parse an if-block from the token stream
 * @param fp FILE* from which to read
 * @return conditional token defining the block
 */
Token* parseIf(FILE* fp);

/**
 * Parse a function call from the token stream
 * @param fp FILE* from which to read
 * @return call token indicating the called function and passed arguments
 */
Token* parseCall(FILE* fp);

/**
 * Parse a sequence of statements
 * @param fp FILE* from which to read
 * @return program token containing statements
 */
Token* parseProg(FILE* fp);

/**
 * Parse the token stream for a variable name
 * @param fp FILE* from which to read
 * @return identifier of the variable
 */
char* parseVariableName(FILE* fp);

/**
 * Parse the next expression contained in the token stream
 * @param fp FILE* from which to read
 * @return token representing the next expression in the stream
 */
Token* parseExpression(FILE* fp);

/**
 * Parse the stream for a potential binary expression
 * @param fp FILE* from which to read
 * @param token given token that might start an expression
 * @param prec precedence of the given token
 * @return the given token or another token representing the expression
 */
Token* potentialBinary(FILE* fp, Token* token, int prec);

/**
 * Parse the stream for a list of tokens between a specified start and end
 * characters delimited by the specified separator
 * @param fp FILE* from which to read
 * @param start starting character for the list (as string)
 * @param end ending character for the list (as string)
 * @param sep delimiter character for the list (as string)
 * @param parse parse function to parse the delimited tokens
 * @return an array containing the parsed tokens
 */
Token** parseDelimited(FILE* fp, char* start, char* end, char* sep,
	Token* (*parse)(FILE*));

/**
 * Check whether the current token is of a given type and contains a given
 * string in the TokenData charVal of data stored with the VALUE key
 * @param fp FILE* from which to read
 * @param type desired token type
 * @param value desired string value
 * @return whether the current token matches
 */
int parser_isValue(FILE* fp, TokenType type, char* value);

/**
 * Confirm that the next tokens match the corresponding is* function and
 * remove them from the stream. Only values with matching token types can be
 * searched for and removed with a single function call.
 * @param fp FILE* from which to read
 * @param type desired token type
 * @param count number of tokens to search for and skip
 * @param ... variadic list of desired values
 */
void skipValue(FILE* fp, TokenType type, int count, ...);

/**
 * Throw an error if the current token is an unexpected token
 * @param token the unexpected token
 */
void unexpected(Token* token);

#endif

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

#include "error.h"

extern int currentCol;
extern int currentLine;

extern int isInteractive;

void err(char* message, int code) {
    fprintf(stderr, "%s: in der Nähe von Zeile %d, Spalte %d\n", message,
            currentLine, currentCol);
    if (!isInteractive) {
        exit(code);
    }
}

void unexpected(Token* token) {
	char msg[200];
	char tk[100];
	tokenToString(token, tk);
	sprintf(msg, "Unerwartetes %s", tk);
	err(msg, UNEXPECTED_TOKEN);
}

void undeclaredIDErr(char* ID) {
	char msg[100];
	sprintf(msg, "Undefinierter Identifikator: %s", ID);
	err(msg, UNDECLARED_IDENTIFIER);
}

void argTypeChk(TokenType expected, TokenType found) {
    if (expected != found) {
        char msg[100];
        sprintf(msg, "Erwartete %s-Argument, %s gefunden",
                tokenTypeToString(expected),
                tokenTypeToString(found));
        err(msg, BAD_ARG_TYPE);
    }
}

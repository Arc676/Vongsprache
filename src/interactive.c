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

#include "interactive.h"

int isInteractive = 0;
Scope* global;
jmp_buf intEnv;

void interactiveMode() {
	isInteractive = 1;
	global = createFrameScope(NULL, PROGRAM);
	char input[100];
	while (1) {
		Token* ast = NULL;
		Token* ret = NULL;
		int state = FAILED;
		if (!setjmp(intEnv)) {
			ast = parseTopLevel(stdin);
			next(stdin);
			state = PARSED;

			ret = eval(ast, global);
			state = EVALED;

			char token[100];
			tokenToString(ret, token);
			printf("\n=> Rückgabewert: %s\n", token);
		}
		switch (state) {
			case EVALED:
				destroyToken(ret);
			case PARSED:
				destroyToken(ast);
				break;
			default:
			case FAILED:
				next(stdin);
				break;
		}
	}
}

void exitInteractive(int code) {
	if (global) {
		destroyScope(global);
	}
	isInteractive = 0;
	exit(code);
}

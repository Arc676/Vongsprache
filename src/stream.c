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

#include "stream.h"

int currentLine = 1;
int currentCol = 0;

int currentChar = 0;
int lastChar = 0;

extern int isInteractive;
extern int blockDepth;

void restart() {
	currentLine = 1;
	currentCol = 0;
}

int peek(FILE* fp) {
	int c = fgetc(fp);
	currentChar = c;
	return c == EOF ? EOF : ungetc(c, fp);
}

char next(FILE* fp) {
	if (currentChar) {
		lastChar = currentChar;
	}
	char c = fgetc(fp);
	if (c == '\n') {
		currentLine++;
		currentCol = 0;
	} else {
		currentCol++;
	}
	currentChar = c;
	return c;
}

int eof(FILE* fp) {
	int c = peek(fp);
	if (isInteractive) {
		if (c == '\n') {
			if (blockDepth == 0) {
				return 1;
			}
			return lastChar == '\n';
		}
	}
	return c == EOF;
}

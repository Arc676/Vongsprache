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

extern int isInteractive;

void printHelp() {
	printf("Interaktivmodus läuft. Verfügbare Befehle:\n\
beenden - Interaktivmodus beenden\n\
bidde was [Befehl] - Hilfe [über 'Befehl'] bekommen\n\
parsen - ein Programm bis EOF parsen\n\
testparsen - ein Programm bis EOF parsen, ohne einen Kindprozess zu schaffen (experimentell)\n\
");
}

void interactiveMode() {
	isInteractive = 1;
	Scope* global = createFrameScope(NULL, PROGRAM);
	char input[100];
	while (1) {
		printf("> ");
		fgets(input, 100, stdin);
		input[strlen(input) - 1] = 0;
		if (!strcmp(input, "beenden")) {
			break;
		} else if (!strncmp(input, "bidde was", 9)) {
			printHelp();
		} else if (!strcmp(input, "parsen")) {
			// create a new pipe to read the program
			int fd[2];
			pipe(fd);

			int isReading = fork();

			if (isReading) {
				// parent process
				// close write end
				close(fd[1]);

				// open the pipe for reading
				FILE* rd = fdopen(fd[0], "r");
				Token* ast = parseTopLevel(rd);
				Token* ret = eval(ast, global);
				ret = copyToken(ret);
				destroyToken(ast);
				char token[100];
				tokenToString(ret, token);
				printf("Rückgabewert: %s\n", token);
				close(fd[0]);
			} else {
				// child process
				// close read end
				close(fd[0]);

				// open the pipe for writing
				FILE* wr = fdopen(fd[1], "w");
				while (1) {
					fgets(input, 100, stdin);
					if (input[0] == '\n') {
						break;
					}
					write(fd[1], input, strlen(input));
				}
				close(fd[1]);
				exit(0);
			}
		} else if (!strcmp(input, "testparsen")) {
			Token* ast = parseTopLevel(stdin);
			next(stdin);
			Token* ret = eval(ast, global);
			ret = copyToken(ret);
			destroyToken(ast);
			char token[100];
			tokenToString(ret, token);
			printf("Rückgabewert: %s\n", token);
		} else {
			printf("Unbekannter Befehl: %s\n", input);
		}
	}
	destroyScope(global);
	isInteractive = 0;
}

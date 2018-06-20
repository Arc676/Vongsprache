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
char* libpath;

int main(int argc, char* argv[]) {
	FILE* file;
	if (argc == 2) {
		libpath = getenv("VONGLIB");
		if (!libpath || strlen(libpath) == 0) {
			libpath = ".";
		}
		if (!strcmp(argv[1], "-i")) {
			printf("Interaktivmodus beginnt\n");
			interactiveMode();
			return 0;
		} else {
			file = fopen(argv[1], "r");
			if (!file) {
				fprintf(stderr, "Datei konnte nicht geöffnet werden\n");
				return 1;
			}
			Token* ast = parseTopLevel(file);
			fclose(file);
			Scope* global = createGlobalScope();
			eval(ast, global);
			destroyScope(global);
			destroyToken(ast);
			return 0;
		}
	} else {
		fprintf(stderr, "Gebrauch: vongsprache Datei\n");
		return 1;
	}
}

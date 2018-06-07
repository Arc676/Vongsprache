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

#include "tokendata.h"

char* tokenDataTypeToString(TokenDataType type) {
    switch (type) {
        case VALUE:
            return "Wert";
        case ARGUMENTS:
            return "Argumente";
        case FUNCTION_BODY:
            return "Funktionkörper";
        case FUNCTION_CALL:
            return "Funktionanruf";
        case CONDITION:
            return "Bedingung";
        case THEN_BLOCK:
            return "DannBlk";
        case ELSE_BLOCK:
            return "SonstBlk";
        case OP:
            return "Operator";
        case LEFT_VAR:
            return "LinkeVariable";
        case RIGHT_VAR:
            return "RechteVariable";
    }
}

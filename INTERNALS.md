# Innere Arbeitsweisen der Vongsprache

In diesem Dokument wird die Darstellung der Tokens und des abstrakten Syntaxbaum der Vongsprache erklärt. Die Regeln der Sprache können in `SYNTAX.md` gefunden werden.

## Struktur eines Tokens

Wie in `token.h` gesehen werden kann, hat jedes Token einen Typ und ein assoziatives Datenfeld in Form einer Hashtabelle.
```
typedef struct {
    TokenType type;
    hashtable_t* tokenData;
} Token;
```

Im Datenfeld werden Informationen über das Token und seinen Zweck gespeichert, beispielsweise andere Tokens und Token-Dateien, dessen Struktur hier unten gezeigt ist.

```
typedef union {
    float floatVal;
    char* charVal;
    Token* tokenVal;
} TokenData;
```

Token-Dateien können jederzeit nur eine Zahl, eine Zeichenfolge oder ein anderes Token enthalten. Ein Token kann aber mehrere Token-Datei-Strukturen enthalten, sollte sowas nötig sein.

## Tokentypen

In der Darstellung der abstrakten Syntaxbaum wird jedem Token einen Typ zugewiesen. Dies erklärt die Rolle des Tokens und wie es interpretiert werden sollte.

- PUNCTUATION - ein Satzzeichen, das etwas in der Sprache bedeutet
- NUMBER - eine Zahl
- STRING - eine Zeichenfolge
- KEYWORD - ein Schlüsselwort
- IDENTIFIER - ein Identifikator einer Variable oder einer Funktion
- OPERATOR - ein Operator
- CALL - ein Funktionanruf
- BINARY - ein binarischer Ausdruck
- IF - beginnt eine Folge einer bedingten Anweisungsblock
- ASSIGN - wiest einen Wert einer Variable zu
- PROGRAM - eine Folge von Anweisungen

## Inhalt der Hashtabelle

Die Schlüssel der Hashtabelle sind einer der folgenden Werte und bezeichnen, was das dem Schlüssel entsprechende Element für das Token bedeutet.

- VALUE - der Wert des Tokens; meistens mit Literal-Tokens benutzt, kann aber andere Bedeutungen haben
- ARGUMENTS - Argumente, die einer Funktion gegeben werden
- FUNCTION_BODY - Anweisungen, die eine Funktion bilden
- FUNCTION_CALL - enthält den Identifikator der Funktion, die angerufen wird
- CONDITION - Bedingung einer bedingten Anweisungsblock
- THEN_BLOCK - Ja-Fall einer bedingten Anweisungsblock
- ELSE_BLOCK - Nein-Fall einer bedingten Anweisungsblock
- OP - Operator eines Ausdrucks
- LEFT_VAR - die linke Variable eines Zuweisungausdrucks
- RIGHT_VAR - der rechte Wert eines Zuweisungausdrucks

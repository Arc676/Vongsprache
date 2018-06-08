# Innere Arbeitsweisen der Vongsprache

In diesem Dokument wird die Darstellung der Tokens und des abstrakten Syntaxbaums der Vongsprache erklärt. Die Regeln der Sprache können in `SYNTAX.md` gefunden werden.

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
} TokenData;
```

Token-Dateien können jederzeit nur eine Zahl oder eine Zeichenfolge enthalten. Das Datenfeld eines Tokens kann aber sowohl mehrere Token-Datei-Strukturen als auch andere Tokens enthalten, sollte sowas nötig sein.

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

## Inhalt der Tokens

In den folgenden Tabellen sind die Inhalte der verschiedenen Tokens gegeben. Ein einem Schlüssel folgendes Sternchen bezeichnet einen Wert, dessen Erscheinung nicht garantiert ist.

Der Schlüssel ist einer der nach oben gegebenen Hashtabellenschlüssel/Token-Datei-Typ.

Der Wertdatentyp bezeichnet, was für eine Struktur genutzt wird, um den Inhalt zu speichern.

- Zeichenfolge - der `charVal`-Wert eines `TokenData*`-Objekts wird genutzt
- Zahl - der `floatVal`-Wert eines `TokenData*`-Objekts wird genutzt
- Token - ein `Token*`-Objekt wird gespeichert
- Tokenfeld - ein Feld von Tokens (`Token**`) wird gespeichert

Tokens, die bei dem Anruf und der Darstellung von Funktionen genutzt werden, sind ganz nach unten beschrieben.

### Satzzeichentokens

Tokentyp: `PUNCTUATION`

| Schlüssel (Token-Datei-Typ) | Wertdatentyp | Inhalt |
| --- | --- | --- |
| `VALUE` | Zeichenfolge | Gefundenes Satzzeichen |

### Operatortokens

Tokentyp: `OPERATOR`

| Schlüssel (Token-Datei-Typ) | Wertdatentyp | Inhalt |
| --- | --- | --- |
| `VALUE` | Zeichenfolge | Gefundener Operator |

### Stringtokens

Tokentyp: `STRING`

| Schlüssel (Token-Datei-Typ) | Wertdatentyp | Inhalt |
| --- | --- | --- |
| `VALUE` | Zeichenfolge | Zeichenfolge-Literal |

### Numerische Tokens

Tokentyp: `NUMBER`

| Schlüssel (Token-Datei-Typ) | Wertdatentyp | Inhalt |
| --- | --- | --- |
| `VALUE` | Zahl | Numerisches Literal |

### Identifikatortokens/Schlüsselworttokens

Tokentyp: `IDENTIFIER`/`KEYWORD`

| Schlüssel (Token-Datei-Typ) | Wertdatentyp | Inhalt |
| --- | --- | --- |
| `VALUE` | Zeichenfolge | Gefundener Identifikator oder gefundenes Schlüsselwort |

### Bedingte Anweisungsblocktokens

Tokentyp: `IF`

| Schlüssel (Token-Datei-Typ) | Wertdatentyp | Inhalt |
| --- | --- | --- |
| `CONDITION` | Token | Zu ausdrückende Bedingung der Anweisungsblock |
| `THEN_BLOCK` | Token | Dem Ja-Fall entsprechende Funktiontoken |
| `ELSE_BLOCK`* | Token | Dem Nein-Fall entsprechende Funktiontoken |

Der `ELSE_BLOCK`-Wert ist für bedingte Anweisungsblöcken ohne Nein-Fall nicht definiert.

### Variablen- und Funktionenzuweisungtokens

Tokentyp: `ASSIGN`

| Schlüssel (Token-Datei-Typ) | Wertdatentyp | Inhalt |
| --- | --- | --- |
| `LEFT_VAR` | Token | Den Identifikator der Variable enthaltendes Token, zu der ein Wert zugewiesen wird |
| `RIGHT_VAR` | Token | Zu zuweisender Wert in Form eines Tokens |

Da Funktionen auch als Variablen gespeichert werden, kann der `RIGHT_VAR`-Wert sowohl ein einfacher Wert als auch ein Funktiontoken sein.

## Funktionen

Funktionen werden in der Vongsprache als Variablen gespeichert. Um Funktionen zu speichern und anzurufen werden drei unterschiedliche Tokens benutzt:
- Das Funktionenzuweisungtoken enthält den Identifikator der neuen Funktion und ein Funktionswrappertoken, das als Variable mit dem gegebenen Identifikator im aktuellen Umfang gespeichert wird
- Das Funktionswrappertoken enthält Informationen über die benötigten Argumente und ein Funktiontoken
- Das Funktiontoken ist die letzte Darstellung einer Funktion und enthält nur die enthaltenen Anweisungen

### Funktionanruftokens

Tokentyp: `CALL`

| Schlüssel (Token-Datei-Typ) | Wertdatentyp | Inhalt |
| --- | --- | --- |
| `FUNCTION_CALL` | Token | Den Identifikator der angerufenen Funktion enthaltendes Token |
| `ARGUMENTS`* | Tokenfeld | Zur Funktion zu gebende Argumente in Form Tokens |
| `VALUE` | Zahl | Anzahl von gegebenen Argumenten |

Für Funktionanrufe ohne Argumente:
- Der `ARGUMENTS`-Wert ist nicht definiert
- Der `VALUE`-Wert ist gleich null

### Funktionswrappertoken

Tokentyp: `PROGRAM`

| Schlüssel (Token-Datei-Typ) | Wertdatentyp | Inhalt |
| --- | --- | --- |
| `VALUE` | Zahl | Anzahl von benötigten Argumenten |
| `ARGUMENTS`* | Zeichenfolgenfeld | Identifikatore der zur Funktion entsprechenden Argumente |
| `FUNCTION_BODY` | Token | Funktiontoken |

Für Funktionanrufe ohne Argumente:
- Der `ARGUMENTS`-Wert ist nicht definiert
- Der `VALUE`-Wert ist gleich null

Die zur Funktion gegebenen Argumente werden mit den im `ARGUMENTS`-Wert gefundenen Identifikatoren in einem neuen Umfang gespeichert.

### Funktiontokens

Tokentyp: `PROGRAM`

| Schlüssel (Token-Datei-Typ) | Wertdatentyp | Inhalt |
| --- | --- | --- |
| `VALUE` | Zahl | Anzahl von enthaltenen Anweisungen |
| `FUNCTION_BODY` | Tokenfeld | Enthaltene Anweisungen in Form Tokens |

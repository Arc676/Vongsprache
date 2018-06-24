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
- LOOP - eine Schleife
- ASSIGN - weist einen Wert einer Variable zu
- INIT - initialisiert eine Variable
- RETURN - bestimmt den Rückgabewert einer Funktion
- INCLUDE - fügt eine andere Datei ein
- FUNC_WRAPPER - ein Funktionswrappertoken
- PROGRAM - eine Folge von Anweisungen

## Inhalt der Hashtabelle

Die Schlüssel der Hashtabelle sind einer der folgenden Werte und bezeichnen, was das dem Schlüssel entsprechende Element für das Token bedeutet.

- ARGUMENTS - Argumente, die einer Funktion gegeben werden
- FUNCTION_BODY - Anweisungen, die eine Funktion bilden
- FUNCTION_CALL - enthält den Identifikator der Funktion, die angerufen wird
- CONDITION - Bedingung einer bedingten Anweisungsblock
- THEN_BLOCK - Ja-Fall einer bedingten Anweisungsblock
- ELSE_BLOCK - Nein-Fall einer bedingten Anweisungsblock
- OP - Operator eines Ausdrucks
- LEFT_VAR - die linke Variable eines Zuweisungausdrucks
- RIGHT_VAR - der rechte Wert eines Zuweisungausdrucks
- LINE_NUM - die Zeile, an der das Token geschafft wurde
- COL_NUM - die Spalte, an der das Token geschafft wurde
- VALUE - der Wert des Tokens; meistens mit Literal-Tokens benutzt, kann aber andere Bedeutungen haben

## Inhalt der Tokens

In den folgenden Tabellen sind die Inhalte der verschiedenen Tokens gegeben. Ein einem Schlüssel folgendes Sternchen bezeichnet einen Wert, dessen Erscheinung nicht garantiert ist.

Der Schlüssel ist einer der nach oben gegebenen Hashtabellenschlüssel/Token-Datei-Typ.

Der Wertdatentyp bezeichnet, was für eine Struktur genutzt wird, um den Inhalt zu speichern.

- Zeichenfolge - der `charVal`-Wert eines `TokenData*`-Objekts wird genutzt
- Zeichenfolgenfeld - ein Feld von Zeichenfolgen (`char**`) wird gespeichert
- Zahl - der `floatVal`-Wert eines `TokenData*`-Objekts wird genutzt
- Token - ein `Token*`-Objekt wird gespeichert
- Tokenfeld - ein Feld von Tokens (`Token**`) wird gespeichert

Tokens, die bei dem Anruf und der Darstellung von Funktionen genutzt werden, sind ganz nach unten beschrieben.

Jedes Token enthält die folgenden Dateien. Diese helfen dabei, den richtigen Ursprungsort eines Fehlers festzustellen.

| Schlüssel (Token-Datei-Typ) | Wertdatentyp | Inhalt |
| --- | --- | --- |
| `LINE_NUM` | Zahl | Zeile, an der das Token geschafft wurde |
| `COL_NUM` | Zahl | Spalte, an der das Token geschafft wurde |

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
| `OP` | Zahl | Index gefundenes Operators |

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

Bei der Zuweisung einer Funktion wird der Schlüsselwert des Identifikators der Funktion durch `CALL` ersetzt.

### Einfügungstokens

Tokentyp: `INCLUDE`

| Schlüssel (Token-Datei-Typ) | Wertdatentyp | Inhalt |
| --- | --- | --- |
| `VALUE` | Zeichenfolge | Name der einzufügenden Datei, ohne `.vong` |

Bei der Einfügung einer Datei wird den Inhalt davon als Wurzel eines neuen Syntaxbaums berechnet, aber im aktuellen Umfang.

Die Standardeinstellung ist, die einzufügende Datei im aktuellen Ordner zu suchen. Sollte aber die Umgebungsvariable `VONGLIB` definiert sein, wird deren Wert als Suchpfad genutzt.

```
$ cd /pfad/vongsprache/vonglib
$ export VONGLIB=`pwd`
$ cd /pfad/.../
$ vongsprache skript.vong
```

Sollte eine Datei im Skript eingefügt werden, wenn beispielsweise `benutze larry` im Skript steht, wird der Interpreter nach `/pfad/vongsprache/vonglib/larry.vong` suchen.

### Bedingte Anweisungsblocktokens

Tokentyp: `IF`

| Schlüssel (Token-Datei-Typ) | Wertdatentyp | Inhalt |
| --- | --- | --- |
| `CONDITION` | Token | Zu ausdrückende Bedingung der Anweisungsblock |
| `THEN_BLOCK` | Token | Dem Ja-Fall entsprechende Funktiontoken |
| `ELSE_BLOCK`* | Token | Dem Nein-Fall entsprechende Funktiontoken |

Der `ELSE_BLOCK`-Wert ist für bedingte Anweisungsblöcken ohne Nein-Fall nicht definiert.

### Variablen- und Funktionenzuweisungstokens

Tokentyp: `ASSIGN`/`INIT` (Variablenzuweisungstokens)
Tokentyp: `INIT` (Funktionenzuweisungstokens)

| Schlüssel (Token-Datei-Typ) | Wertdatentyp | Inhalt |
| --- | --- | --- |
| `LEFT_VAR` | Token | Den Identifikator der Variable enthaltendes Token, zu der ein Wert zugewiesen wird |
| `RIGHT_VAR` | Token | Zu zuweisender Wert in Form eines Tokens |

Da Funktionen auch als Variablen gespeichert werden, kann der `RIGHT_VAR`-Wert sowohl ein einfacher Wert als auch ein Funktiontoken sein.

### Binärausdruckstoken

Tokentyp: `BINARY`

| Schlüssel (Token-Datei-Typ) | Wertdatentyp | Inhalt |
| --- | --- | --- |
| `LEFT_VAR` | Token | Den linken Ausdruck enthaltendes Token |
| `RIGHT_VAR` | Token | Den rechten Ausdruck enthaltendes Token |
| `OP` | Token | Operator des Ausdrucks in Form eines Tokens |

### Schleifentokens

Tokentyp: `LOOP`

| Schlüssel (Token-Datei-Typ) | Wertdatentyp | Inhalt |
| --- | --- | --- |
| `VALUE`* | Token | Identifikator des Zählers der Schleife |
| `ARGUMENTS`* | Token | Den ersten Wert des Schleifezählers darstellendes Ausdruckstoken |
| `CONDITION` | Token | Bedingung der Schleife |
| `FUNCTION_BODY` | Token | Die Anweisungen der Schleife enthaltendes Funktiontoken |

Alle Schleifen, das heißt sowohl Zählschleifen als auch kopfgesteuerte Schleifen, werden als kopfgesteuerte Schleifen gespeichert und berechnet. Die Bedingung einer Zählschleife ist ein Vergleich zwischen dem Zähler und dem End-Wert. Die Schleife läuft, solange der Zähler kleiner oder gleich dem End-Wert ist.

Die `VALUE`- und `ARGUMENTS`-Werte sind nur für Zählschleifen anwesend.

## Funktionen

Funktionen werden in der Vongsprache als Variablen gespeichert. Um Funktionen zu speichern und anzurufen werden drei unterschiedliche Tokens benutzt:
- Das Funktionenzuweisungstoken enthält den Identifikator der neuen Funktion und ein Funktionswrappertoken, das als Variable mit dem gegebenen Identifikator im aktuellen Umfang gespeichert wird
- Das Funktionswrappertoken enthält Informationen über die benötigten Argumente und ein Funktiontoken
- Das Funktiontoken ist die letzte Darstellung einer Funktion und enthält nur die Anweisungen der Funktion

Bei der Berechnung einer Funktion gibt es auch ein viertes Token, das den Rückgabewert der Funktion enthält. Für Funktionen ohne Rückgabewert ist dies nicht anwesend.

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

Tokentyp: `FUNC_WRAPPER`

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

### Rückgabewertstokens

Tokentyp: `RETURN`

| Schlüssel (Token-Datei-Typ) | Wertdatentyp | Inhalt |
| --- | --- | --- |
| `VALUE` | Token | Den Rückgabewert bildender Ausdruck |

## Umfang

In der Vongsprache werden Variablen (und Funktionen) in einer Hashtabelle mit ihren Identifikatoren gespeichert. Diese sind in Unterumfängen zugänglich.

Der globale Umfang hat keinen Überumfang. Der Wert des Überumfangs des globalen Umfangs ist gleich `NULL`.

Bei der Berechnung eines `PROGRAM`-Tokens wird ein neuer Unterumfang geschafft. Das gewährleistet, dass in einem Block deklarierte Variablen nach dem Ende dieses Blocks gelöscht werden.

Beim Anruf einer Funktion werden **zwei** neue Unterumfänge geschafft. Im ersten werden die zur Funktion gegebenen Argumente gespeichert und der zweite wird beim Berechnung der Anweisungsblock der Funktion geschafft.

Am Anfang einer Schleife werden auch **zwei** neue Unterumfänge geschafft.

### Besondere Variablen

In bestimmten Umfängen werden besondere Variablen mit Schlüsselwörtern als Identifikatoren gespeichert. Die Benutzung von Schlüsselwörtern als Identifikatoren verhindert, dass von dem Benutzer definierte Variablen damit verwechselt werden könnten.

Diese besonderen Variablen bezeichnen den Anfang einer Struktur beispielsweise eine Funktion oder eine Schleife. Dies hilft bei der Bestimmung des Kontrollflusses.

Besondere Variablen entstehen alle aus leeren Identifikatortokens.

#### Funktionen und Schleifen

Bevor der Anweisungsblock einer Funktion oder Schleife berechnet wird, wird eine besondere Variable mit dem Identifikator „Funktionigkeit“ für Funktionen oder „solange“ für Schleifen im zuerst geschafften Unterumfang gespeichert.

#### Rückgabewert

Bei der Bestimmung des Rückgabewertes einer Funktion durch die Benutzung des Schlüsselwortes `hab` wird eine besondere Variable mit demselben Schlüsselwort als Identifikator gespeichert.

Nachdem ein Rückgabewertstoken berechnet wird, werden alle weitere Befehle der Funktion übersprungen. Das heißt, alle folgenden Befehle werden nicht berechnet, bis der letzte Umfang mit einer mit dem Identifikator „Funktionigkeit“ gespeicherten Variable gefunden ist.

#### Globaler Umfang

Nur im globalen Umfang darf eine Variable mit dem Identifikator „i“ gespeichert werden. Bei der Berechnung eines `PROGRAM`-Tokens wird kein neuer Umfang erschafft, wenn sich das Token im globalen Umfang befindet.

Diese Variable wird vor der Berechnung der Wurzel des abstrakten Syntaxbaums bei der Erschaffung des globalen Umfangs gespeichert.

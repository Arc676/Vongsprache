# Syntax der Vongsprache

In diesem Dokument wird die Syntax der Vongsprache erklärt. In den Beispielen sind Wörter zwischen `[eckigen Klammern]` fakultativ. Beispielsweise können Funktionen auch keine Parameter haben. Wörter zwischen `<spitzen Klammern>` bezeichnen, was eigentlich geschrieben werden sollte. Beispielsweise bedeutet `<Identifikator>`, dass dort ein Identifikator erwartet wird.

Bei der Benutzung der Vongsprache ist die Groß- und Kleinschreibung zu beachten. Einrückung der Anweisungen in Blöcken ist für Lesbarkeit empfohlen aber nicht notwendig.

## Eingebildete Funktionen

### Kommentare

In der Vongsprache gibt es nur Zeilenkommentare. Diese fangen mit `#` an.
```
# Das ist ein Kommentar
# Und das hier auch
```

### Ein-/Ausgabe

Die Vongsprache bietet eine Eingabe- und eine Ausgabefunktion an.

```
# Hallo, Welt! drucken
bidde drucke mit ("Hallo, Welt!")

# Nach Eingabe fragen, ohne zu speichern
bidde gib mit ("Diese Eingabe wird nicht gespeichert.")

# Nach dem Namen des Benutzers fragen und dann ihn begrüßen
i bims Name vong bidde gib mit ("Wie heißt du?") her
bidde drucke mit ("Hallo, ", Name)
```

### Typumwandlung

Der Datentyp einer Variable kann mit den folgenden eingebildeten Funktionen umgewandelt werden. Der umgewandelte Wert wird als Rückgabewert gegeben, muss also auch zugewiesen werden, um gespeichert zu werden.

```
i bims Zeichenfolge vong "12" her
i bims Zahl vong 14 her

# Zeichenfolge in Zahl umwandeln
bidde zuZahl mit (Zeichenfolge)

# Zahl in Zeichenfolge umwandeln
i bims UmgewandelteZeichenfolge vong bidde zuZeichenfolge mit (Zahl) her
```

### Dateieinfügung

Das Schlüsselwort `benutze` erlaubt die Einfügung einer anderen Datei. Das macht den Inhalt davon zugänglich beispielsweise in der eingefügten Datei deklarierte Funktionen.

```
# Datei1.vong
i bims foo vong Funktionigkeit
	bidde drucke mit ("i bims in einer anderen Datei")
her

# Datei2.vong
benutze Datei1

bidde foo
```

## Deklarationen

### Identifikatoren

Schlüsselwörter, die hier unten aufgezählt werden, dürfen nicht als Identifikatoren benutzt werden.
Das erste Zeichen eines Identifikatoren muss ein Unterstrich oder ein Buchstabe sein.

In der Vongsprache sind Operatoren auch Schlüsselwörter. Diese sind im Abschnitt "Operatoren" aufgezählt.

Nicht-Operator Schlüsselwörter:
```
i
bims
vong
her
mit
bis
Funktionigkeit
am
Wahrigkeit
Sonstigkeit
solange
hab
bidde
benutze
```

### Variablen

Die Vongsprache benutzt, was entweder [Duck Typing](https://de.wikipedia.org/wiki/Duck-Typing) oder Dynamic Typing genannt wird. Außerdem besitzt sie technisch gesehen eine starke Typisierung, genau so wie Python.
Variablen dürfen allerdings in andere Typen umgewandelt werden.

Diese werden mit `bimsen` deklariert.

```
i bims <Identifikator> vong <Wert> her
```

Hier sind einige Beispiele.

```
i bims Alter vong 26 her
i bims RationaleZahl vong 2.5 her
i bims Begrüßung vong "Hallo, Welt!" her
```

### Funktionen

Auch Funktionen werden mit demselben Verb deklariert.

```
i bims <Identifikator> vong Funktionigkeit [mit (Parameter1, Parameter2, ...)]
    <Anweisungen>
her
```

#### Rückkehranweisungen

Das Schlüsselwort `hab` bezeichnet den Rückgabewert der Funktion.

```
i bims Doppel vong Funktionigkeit mit (x)
    hab x mal 2
her
```

## Anweisungen

Funktionen müssen höflich aufgerufen werden.

```
bidde <Funktionidentifikator> [mit (Parameter1, Parameter2, ...)]
```

## Operatoren

Alle folgenden Wörter sind auch Schlüsselwörter und können deshalb nicht als Identifikatoren genutzt werden.

| Operator | Priorität | Funktion |
| --- | --- | --- |
| `bimst` | 1 | Zuweisung |
| `oder` | 2 | Logischer Oder-Operator |
| `und` | 3 | Logischer Und-Operator |
| `größer` | 7 | Vergleichsoperator (`x > y`) |
| `größergleich` | 7 | Vergleichsoperator (`x >= y`) |
| `kleiner` | 7 | Vergleichsoperator (`x < y`) |
| `kleinergleich` | 7 | Vergleichsoperator (`x <= y`) |
| `gleich` | 7 | Gleichheitsoperator |
| `plus` | 10 | Additionoperator |
| `minus` | 10 | Subtraktionoperator |
| `nicht` | 20 | Logische Negation |
| `mal` | 20 | Multiplikationoperator |
| `gteild` | 20 | Divisionoperator |
| `hoch` | 20 | Potenzoperator |
| `rest` | 20 | Moduloperator |

## Kontrollstrukturen

Die Vongsprache bietet folgende Kontrollstrukturen an. In der Vongsprache gibt es keine booleschen Ausdrücke.
Deshalb brauchen bedingte Anweisungen und Schleifen boolesche Variablen.

### Bedingte Anweisungen

```
bims <Ausdruck> vong Wahrigkeit
    <Anweisungen>
her am Sonstigkeit
    <Anweisungen>
her
```

### Schleifen

Die Vongsprache hat Zählschleifen und kopfgesteuerte Schleifen. Die folgenden Beispiele zeigen, wie man diese nutzt.

#### Zählschleifen

```
mit <Zähler> vong <Start> bis <Ende>
    <Anweisungen>
her
```

Die Werte des Zählers sind inklusive der Start- und End-Werte.

#### Kopfgesteuerte Schleifen

```
solange <Ausdruck> vong Wahrigkeit
    <Anweisungen>
her bims
```

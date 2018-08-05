# Syntax der Vongsprache

In diesem Dokument wird die Syntax der Vongsprache erklärt. In den Beispielen sind Wörter zwischen `[eckigen Klammern]` fakultativ. Beispielsweise können Funktionen auch keine Parameter haben. Wörter zwischen `<spitzen Klammern>` bezeichnen, was eigentlich geschrieben werden sollte. Beispielsweise bedeutet `<Identifikator>`, dass dort ein Identifikator erwartet wird.

Bei der Benutzung der Vongsprache ist die Groß- und Kleinschreibung zu beachten. Einrückung der Anweisungen in Blöcken ist für Lesbarkeit empfohlen aber nicht notwendig.

## Grundlagen

### Kommentare

In der Vongsprache gibt es nur Zeilenkommentare. Diese fangen mit `#` an.
```
# Das ist ein Kommentar
# Und das hier auch
```

### Eingebildete Funktionen

Diese Tabelle zeigt die eingebildeten Funktionen der Vongsprache und die Argumente, die ihnen gegeben werden müssen. Beim Anruf einer Funktion werden diese zuerst gesucht; deshalb ist es unmöglich, eigene Funktionen mit diesen Namen zu schaffen.

| Funktion | Argumente | Rückgabewerttyp | Beschreibung |
| --- | --- | --- | --- |
| `drucke` | Irgendeine Anzahl irgendwelcher Argumente | `NULL` | Ausgabefunktion, druckt die gegebenen Argumente |
| `gib` | Irgendeine Anzahl irgendwelcher Argumente | Zeichenfolge | Eingabefunktion, druckt die gegebenen Argumente und ergibt den von dem Benutzer eingegebenen Wert |
| `zuZeichenfolge` | Zahl | Zeichenfolge | Gegebenen Argument in Zeichenfolge umwandeln |
| `zuZahl` | Zeichenfolge | Zahl | Gegebenen Argument in Zahl umwandeln; ergibt null falls keine Zahl gefunden wird |
| `piMalDaumen` | Zahl | Zahl | Ergibt eine zufällige Zahl zwischen null und den gegebenen Wert minus 1 |
| `samen` | Zahl oder nichts | `NULL` | Setzt den gegebenen Wert als Saat für die Zufallszahlgeneration; benutzt die aktuelle UNIX-Zeit falls kein Wert gegeben wird |
| `raus` | Zahl oder nichts | `NULL` | Beendet sofort das Programm mit der gegebenen Zahl als Exit-Code; nutzt 0 falls nichts gegeben wird |

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
aufgeben
durchmarsch
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

Darauf muss geachtet werden, dass Zeilenumbrüche zwischen Wörtern in der Vongsprache ignoriert werden. Sollte die erste Anweisung in einer Funktion eine Zählschleife sein, welche mit dem Schlüsselwort `mit` anfängt, muss die andernfalls unnötige Argumentenliste unbedingt angegeben werden, um zu verhindern, dass der Anfang der Zählschleife mit einer Argumentenliste verwechselt wird. Wenn die Funktion keine Argumente braucht, muss eine leere Argumentenliste angegeben werden. Dies wird im folgenden Beispiel gezeigt.

```
i bims eineFunktion vong Funktionigkeit mit ()
	mit j vong 0 bis 10
		bidde drucke mit (j)
	her
her
```

### Kontrollfluss

#### Rückkehranweisungen

Das Schlüsselwort `hab` bezeichnet den Rückgabewert der Funktion.

```
i bims Doppel vong Funktionigkeit mit (x)
    hab x mal 2
her
```

#### Schleifenabbruch und Iterationsabbruche

Die Schlüsselwörter `aufgeben` und `durchmarsch` brechen die aktuelle Schleife beziehungsweise nur die aktuelle Iteration der Schleife ab.

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

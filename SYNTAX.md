# Syntax der Vongsprache

In diesem Dokument wird die Syntax der Vongsprache erklärt. In den Beispielen sind Wörter zwischen `[eckigen Klammern]` fakultativ. Beispielsweise können Funktionen auch keine Parameter haben. Wörter zwischen `<spitzen Klammern>` bezeichnen, was eigentlich geschrieben werden sollte. Beispielsweise bedeutet `<Identifikator>`, dass dort ein Identifikator erwartet wird.

## Kommentare

In der Vongsprache gibt es nur Zeilenkommentare. Diese fangen mit `#` an.
```
# Das ist ein Kommentar
# Und das hier auch
```

## Format

Bei der Benutzung der Vongsprache ist die Groß- und Kleinschreibung zu beachten. Einrückung der Anweisungen in Blöcken ist für Lesbarkeit empfohlen aber nicht notwendig.

## Ein-/Ausgabe

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

## Deklarationen

### Identifikatoren

Schlüsselwörter, die hier unten aufgezählt werden, dürfen nicht als Identifikatoren benutzt werden.
Das erste Zeichen eines Identifikatoren muss ein Unterstrich oder ein Buchstabe sein.

Schlüsselwörter:
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
bidde
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

## Anweisungen

Funktionen müssen höflich aufgerufen werden.

```
bidde <Funktionidentifikator> [mit (Parameter1, Parameter2, ...)]
```

## Operatoren

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

#### Kopfgesteuerte Schleifen

```
solange <Ausdruck> vong Wahrigkeit
  <Anweisungen>
her bims
```

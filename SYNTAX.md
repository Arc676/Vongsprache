# Syntax der Vongsprache

In diesem Dokument wird die Syntax der Vongsprache erklärt. In den Beispielen sind Wörter zwischen `[eckigen Klammern]` fakultativ. Beispielsweise können Funktionen auch keine Parameter haben. Wörter zwischen `<spitzen Klammern>` bezeichnen, was eigentlich geschrieben werden sollte. Beispielsweise bedeutet `<Identifikator>`, dass dort ein Identifikator erwartet wird.

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
bims <Variable> vong Wahrigkeit
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
solange <Variable> vong Wahrigkeit
  <Anweisungen>
her bims
```

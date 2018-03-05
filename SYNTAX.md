# Syntax der Vongsprache

## Deklarationen

### Identifikatoren

Schlüsselwörter, die hier unten aufgezählt werden, dürfen nicht als Identifikatoren benutzt werden. Das erste Zeichen eines Identifikatoren muss ein Unterstrich oder ein Buchstabe sein.

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
```

### Variablen

Die Vongsprache benutzt was jetzt auch [Duck Typing](https://de.wikipedia.org/wiki/Duck-Typing) oder Dynamic Typing genannt wird. Wie Python, die Vongsprache besitzt technisch gesehen eine starke Typisierung, aber Variablen dürfen in andere Typen umgewandelt werden.

Variablen werden mit `bimsen` deklariert.

```
i bims [Identifikator] vong [Wert] her
```

Hier sind einige Beispiele.

```
i bims Alter vong 26 her
i bims RationaleZahl vong 2.5 her
i bims Begrüßung vong "Hallo, Welt!" her
```

### Funktionen

Auch Funktionen werden mit dem selben Verb deklariert.

```
i bims [Identifikator] vong Funktionigkeit mit [(Parameter1, Parameter2, ...)]
  [Anweisungen]
her
```

## Anweisungen

## Operatoren

## Kontrollstrukturen

Die Vongsprache bietet Ihnen die folgende Kontrollstrukturen an. In der Vongsprache gibt es keine boolesche Ausdrücke; deshalb brauchen bedingte Anweisungen und Schleifen boolesche Variablen.

### Bedingte Anweisungen

```
bims [Variable] vong Wahrigkeit
  [Anweisungen]
am Sonstigkeit
  [Anweisungen]
her
```

### Schleifen

Die Vongsprache hat Zählschleifen und Kopfgesteuerte Schleifen. Die folgende Beispiele zeigen, wie man diese Schleifen nutzt.

#### Zählschleifen

```
mit [Zähler] vong [Start] bis [Ende]
  [Anweisungen]
her
```

#### Kopfgesteuerte Schleifen

```
solange [Variable] vong Wahrigkeit
  [Anweisungen]
her bims
```

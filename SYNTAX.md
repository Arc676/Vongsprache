# Syntax der Vongsprache

## Deklarationen

### Identifikatoren

Schlüsselwörter dürfen nicht als Identifikatoren benutzt werden.

### Variablen

Die Vongsprache benutzt was jetzt auch [Duck Typing](https://de.wikipedia.org/wiki/Duck-Typing) genannt wird. Wie Python, die Vongsprache besitzt technisch gesehen eine starke Typisierung, aber Variablen dürfen in andere Typen umgewandelt werden.

Variablen werden mit `bimsen` deklariert.

```
i bims [Identifikator] vong [Wert] her
```

Hier sind einige Beispiele.

```
i bims Alter vong 26 her
i bims RationaleZahl vong 2.5 her
i bims Bool vong Ja her
i bims Begrüßung vong "Hallo, Welt!" her
```

### Funktionen

Auch Funktionen werden mit dem selben Verb deklariert.

```
i bims [Identifikator] vong Funktionigkeit
  [Anweisungen]
her
```

## Anweisungen

## Operatoren

## Kontrollstruktur

Die Vongsprache bietet Ihnen die folgende Kontrollstrukturen an.

### Bedingte Anweisungen

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

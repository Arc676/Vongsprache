# EBNF für Vongsprache

| Name | Definition |
| --- | --- |
| Operator | `"bimst" \| "oder" \| "und" \| "größer" \| "größergleich" \| "kleiner" \| "kleinergleich" \| "gleich" \| "ungleich" \| "plus" \| "minus" \| "mal" \| "gteild" \| "hoch" \| "rest"` |
| Ziffer | `? 0-9 ?` |
| Zahl | `Ziffer { Ziffer }` |
| Buchstabe | `? A-Z ? \| ? a-z ?` |
| Zeichen | `? Alle von C lesbare Zeichen ?` |
| Zeichenkette | `'"' { Zeichen } '"'` (siehe Ausnahmen unten) |
| Identifikator | `("_" \| Buchstabe) { "_" \| Buchstabe \| Ziffer }` außer Schlüsselwörtern |
| Funktion | `"Funktionigkeit" [ "mit (" Identifikator { "," Identifikator } ")" ] Programm "her"` |
| Fallunterscheidung | `"bims" Ausdruck "vong Wahrigkeit" Programm "her" [ "am Sonstigkeit" Programm "her" ]` |
| Funktionsaufruf | `"bidde" Identifikator [ "mit (" Ausdruck { "," Ausdruck } ")" ]` |
| Schleife | `Zählschleife \| Kopfgesteuerte_Schleife` |
| Zählschleife | `"mit" Identifikator "vong" Ausdruck "bis" Ausdruck Programm "her"` |
| Kopfgesteuerte_Schleife | `"solange" Ausdruck "vong Wahrigkeit" Programm "her bims"` |
| Kontrollflussbefehl | `"hab" Ausdruck \| "aufgeben" \| "durchmarsch"` |
| Bibliothekinklusion | `"benutze" Identifikator` |
| Zuweisung | `"i bims" Identifikator "vong" Wert "her"` |
| Kern | `Zuweisung \| Fallunterscheidung \| Funktionsaufruf \| Schleife \| Kontrollflussbefehl \| Bibliothekinklusion \| Identifikator \| Zahl \| Zeichenkette \| "(" Ausdruck ")"` |
| Ausdruck | `Kern { operator Kern }` |
| Wert | `Funktion \| Ausdruck` |
| Programm | `Ausdruck { Ausdruck }` |

## Zeichenketten

Eine Zeichenkette muss durch Anführungszeichen begrenzt sein. Sie darf beliebig viele willkürlichen Zeichen enthalten, außer anderen Anführungszeichen, es sei denn, diese werden escaped `\"`.

## Identifikatoren

Schlüsselwörter sind keine gültigen Identifikatoren. Folgende Zeichenketten sind Schlüsselwörter:
```
i bims vong bis mit her Funktionigkeit am Wahrigkeit Sonstigkeit solange hab aufgeben durchmarsch benutze bidde
```

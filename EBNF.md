# EBNF für Vongsprache

| Name | Definition |
| --- | --- |
| Operator | `"bimst" \| "oder" \| "und" \| "größer" \| "größergleich" \| "kleiner" \| "kleinergleich" \| "gleich" \| "ungleich" \| "plus" \| "minus" \| "mal" \| "gteild" \| "hoch" \| "rest"` |
| Ziffer | `? 0-9 ?` |
| Zahl | `Ziffer { Ziffer }` |
| Buchstabe | `? A-Z ? \| ? a-z ?` |
| Zeichen | `? Alle von C lesbare Zeichen ?` |
| Zeichenkette | `'"' { Zeichen } '"'` |
| Identifikator | `("_" \| Buchstabe) { "_" \| Buchstabe \| Ziffer }` |
| Wert | `Funktion \| Ausdruck` |
| Programm | `Ausdruck { Ausdruck }` |
| Funktion | `"Funktionigkeit" [ "mit (" Identifikator { "," Identifikator } ")" ] Programm "her"` |
| Fallunterscheidung | `"bims" Ausdruck "vong Wahrigkeit" Programm "her" [ "am Sonstigkeit" Programm "her" ]` |
| Funktionsaufruf | `"bidde" Identifikator [ "mit (" Ausdruck { "," Ausdruck } ")" ]` |
| Schleife | `Zählschleife \| Kopfgesteuerte_Schleife` |
| Zählschleife | `"mit" Identifikator "vong" Ausdruck "bis" Ausdruck Programm "her"` |
| Kopfgesteuerte_Schleife | `"solange" Ausdruck "vong Wahrigkeit" Programm "her bims"` |
| Kontrollflussbefehl | `"hab" Ausdruck \| "aufgeben" \| "durchmarsch"` |
| Bibliothekinklusion | `"benutze" Identifikator` |
| Ausdruck | `Kern { operator Kern }` |
| Kern | `Zuweisung \| Fallunterscheidung \| Funktionsaufruf \| Schleife \| Kontrollflussbefehl \| Bibliothekinklusion \| Identifikator \| Zahl \| Zeichenkette \| "(" Ausdruck ")"` |
| Zuweisung | `"i bims" Identifikator "vong" Wert "her"` |

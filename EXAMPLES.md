# Auf Vongsprache geschriebene Beispielsprogramme

## Fakultät

```
i bims fakultät vong Funktionigkeit mit (n)
	bims n größer 1 vong Wahrigkeit
		hab n mal bidde fakultät mit (n minus 1)
	her
	hab n
her
bidde drucke mit (
	bidde fakultät mit (
		bidde zuZahl mit (
			bidde gib mit ("Geben Sie eine Zahl ein")
		)
	)
)
```

## Sterne

```
i bims Anzahl vong bidde gib mit ("Wie viele Sterne möchten Sie?") her
Anzahl bimst bidde zuZahl mit (Anzahl)
mit j vong 0 bis Anzahl
	bidde drucke mit ("*")
her
```

## [Collatz-Problem](https://de.wikipedia.org/wiki/Collatz-Problem)

```
i bims Zahl vong bidde zuZahl mit (
	bidde gib mit ("Geben Sie eine Zahl ein")
) her
solange Zahl größer 1 vong Wahrigkeit
	bims Zahl rest 2 gleich 0 vong Wahrigkeit
		Zahl bimst Zahl gteild 2
	her am Sonstigkeit
		Zahl bimst Zahl mal 3 plus 1
	her
	bidde drucke mit (Zahl)
her bims
```

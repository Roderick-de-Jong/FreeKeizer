  Uitvoeren
  
Vanuit de FreeKeizer directory uitvoeren: bin/FreeKeizer

  Configuratiebestand CompetitieParameters.txt

1e regel: naam van de competitie (vrij formaat, maximaal 255 tekens)
2e regel: startdatum van de competitie (vrij formaat, maximaal 255 tekens)
3e regel: eigenwaarde van de nummer 1 van de competitie, moet een geheel, positief getal zijn
4e regel: aantal ronden dat gespeeld is

  Configuratiebestand Spelers.txt

De eerste regel bevat het aantal spelers in de competitie.
Elke volgende regel bestaat uit een uniek getal gevolgd door een spatie en de naam van de speler.
De naam van de speler mag spaties en speciale tekens bevatten, en mag maximaal 255 tekens lang zijn.

  Invoer bestand UitslagX.txt

De eerste regel bevat het aantal uitslagen.
Elke volgende regel is van het formaat A B (C), waar A het type uitslag is, B het id van de witspeler,
en C het id van de zwartspeler (in sommige gevallen wordt dit laatste weggelaten). De mogelijke waarden
voor A (het uitslagtype) zijn:
0 zwart wint
1 wit wint
2 remise
3 vrije ronde (C kan worden weggelaten)


Working copie
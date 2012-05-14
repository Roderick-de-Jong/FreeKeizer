#ifndef KEIZERIO_H
#define KEIZERIO_H

#include <memory>

#include "Competitie.h"
#include "CompetitieParameters.h"
#include "Ranglijst.h"
#include "Ronde.h"
#include "Speler.h"
#include "Spelerslijst.h"

// Forward declarations
class Competitie;



/**
 * Verzorgt de volgende taken:
 * - Input (en in de toekomst output) m.b.t. de Keizerdatabase
 * - HTML output van o.a. ranglijsten
 */
class KeizerIO
{
	public:
	KeizerIO();
	virtual ~KeizerIO();
	
	/**
	 * Leest het bestand met de algemene gevens en instellingen van de competitie en vult daarmee een CompetitieParameters object.
	 */
	virtual std::auto_ptr<CompetitieParameters> leesCompetitieParameters();
	
	/**
	 * Leest het bestand met de spelerslijst, en voegt de spelers die daarin worden gevonden toe aan het gegeven Spelerslijst object.
	 */
	virtual void leesSpelers(Spelerslijst* spelerslijst);
	
	/**
	 * Leest het bestand met de uitslag van de gevraagde ronde, en retourneert de inhoud ervan in de vorm van een Ronde object.
	 * @param r Het nummer van de ronde, bereik [1;n] waarbij n het aantal rondes in de competitie is.
	 */
	virtual Ronde* leesRonde(unsigned int r);
	
	/**
	 * Produceert een ranglijst in de vorm van een HTML document op basis van de competitiestand na de gegeven ronde.
	 * @param competitie De competitiedata op basis waarvan de stand gemaakt moet worden.
	 * @param ronde De stand na deze ronde wordt gebruikt voor het document. Ronde 0 betekent de beginstand, ronde 1 de stand na de eerste ronde partijen, etc.
	 */
	virtual void writeRanglijstDocument(Competitie* competitie, unsigned int ronde);
	
	/**
	 * Produceert een HTML document met daarin een kruistabel voor de competitie, daarin opnemend de partijen tot en met de gegeven ronde.
	 * @param competitie De competitiedata op basis waarvan de kruistabel gemaakt moet worden.
	 * @param ronde De competitiestatus na deze ronde wordt gebruikt voor het document. Ronde 0 betekent de beginstand, ronde 1 de stand na de eerste ronde partijen, etc.
	 */
	virtual void writeKruistabel(Competitie* competitie, unsigned int ronde);
};

#endif // KEIZERIO_H


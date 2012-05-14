#ifndef COMPETITIE_H
#define COMPETITIE_H

#include <memory>
#include <stdexcept>
#include <vector>
#include <iostream>

#include "CompetitieParameters.h"
#include "KeizerIO.h"
#include "Ranglijst.h"
#include "Ronde.h"
#include "Spelerslijst.h"

// Forward declarations
class KeizerIO;



// ALGORITME:
//
// leesCompetitieDatabase():
// 		CompetitieParameters object construeren en vullen met data uit bestand
// 		Spelerslijst object construeren en vullen met Speler objecten n.a.v. data uit bestand
// 		Ronde object construeren voor elke gespeelde ronde en vullen met Partij objecten n.a.v. data uit bestand
// 		berekenStand() aanroepen
//
// berekenStand():
//    for(int r = 1; r <= huidigeRonde; r++)
//		 		berekenStandNaRonde(r);
//
// berekenStandNaRonde(int ronde)
//    neem _ranglijstNaRonde[ronde - 1] als waarderingsranglijst
//		gebruik _rondes[1...ronde] om de nieuwe ranglijst te bepalen

// TODO: maakIndeling() en relevante datastructuren


/**
 * Kern van het Keizerprogramma; beheert de datastructuren, bevat het Keizeralgoritme, en biedt een API aan.
 */
class Competitie
{
	public:
	Competitie();
	virtual ~Competitie();
	// TODO: copy constructor, operator=
	
	
	///// MUTATORS /////
	
	/**
	 * Leest één of meerdere bestanden met competitiegegevens.
	 */
	virtual void leesCompetitieDatabase();
	
	// TODO: mutators voor simpele operaties, zodat een GUI gebouwd kan worden voor FreeKeizer
	// TODO: batch mode, zodat niet na elke simpele operatie de stand herberekend wordt
	
	
	
	///// GETTERS /////
	
	/**
	 * Retourneert een kopie van de datastructuur met competitieparameters.
	 * De competitieparameters zijn zaken zoals de naam van de competitie en de instelling voor de hoogste eigenwaarde e.d.
	 */
	virtual std::auto_ptr<CompetitieParameters> getCompetitieParameters();
	
	/**
	 * Geeft het totaal aantal gespeelde rondes in deze competitie.
	 */
	virtual unsigned int getNrRondes();
	
	/**
	 * Retourneert een kopie van de lijst met spelers in de competitie.
	 */
	virtual std::auto_ptr<Spelerslijst> getSpelerslijst();
	
	/**
	 * Retourneert een kopie van de ranglijst na de gevraagde ronde.
	 * @param ronde Geeft aan welke ranglijst gewenst is; bereik is [0;n] waarbij n het aantal gespeelde rondes is in deze competitie.
	 */
	virtual std::auto_ptr<Ranglijst> getRanglijstNaRonde(unsigned int ronde);
	
	/**
	 * Retourneert een kopie van de partij-uitslagen van de gevraagde ronde.
	 * @param ronde Geeft aan voor welke ronde de uitslag gevraagd wordt; bereik is [0;n] waarbij n het aantal gespeelde rondes is in deze competitie.
	 * TODO: opvragen van uitslag ronde 0 illegaal maken? Da's een NULL pointer namelijk.
	 */
	virtual std::auto_ptr<Ronde> getUitslagVanRonde(unsigned int ronde);

	
	
	///// OUTPUT METHODS /////
	
	/**
	 * Schrijft een ranglijst naar een bestand.
	 * @param ronde Geeft aan welke ranglijst gebruikt moet worden; gebruikt wordt de ranglijst van direct na de gegeven ronde. Geef ronde 0 op voor de beginranglijst.
	 * @see KeizerIO::writeRanglijstDocument()
	 */
	virtual void writeRanglijstDocument(unsigned int ronde);
	
	/**
	 * Geeft het Competitie object de mogelijkheid statusberichten en dergelijke aan de gebruiker te tonen of naar een logbestand te schrijven.
	 * Het gegeven ostream object wordt niet gedeleted door de ~Competitie destructor.
	 */
	virtual void setLogStream(std::ostream* stream);

	/**
	 * Geeft de lijst van spelers tegen wie de gegeven speler heeft gespeeld deze competitie, gesorteerd op ronde.
	 * @return Een kopie van de lijst met tegenstanders voor de gegeven speler. De geretourneerde datastructuur heeft grootte (nrRondes + 1) en heeft
	 *         de rondenummers als indices, dus bijvoorbeeld getTegenstanderLijst(x)[3] geeft de tegenstander van speler x in ronde 3. Belangrijk:
	 *         getTegenstanderLijst(x)[r] is -1 voor r = 0 en tevens voor elke ronde r waarin speler x geen tegenstander had (bijv. vrije ronde of afwezig).
	 */
	virtual std::vector<int> getTegenstanderLijst(unsigned int spelerId);
	
	
	
	///// PROTECTED METHODS /////
	
	protected:
	
	/**
	 * Berekent de stand in de competitie na de huidige ronde (n).
	 * Voert een gedeeltelijke herberekening (1 iteratie) uit m.b.v. de stand na ronde (n - 1).
	 */
	virtual void _berekenStand();
	
	/**
	 * Berekent de stand in de competitie na de gegeven ronde.
	 * Voert een gedeeltelijke herberekening (1 iteratie) uit m.b.v. de stand voor die ronde.
	 */
	virtual void _berekenStandNaRonde(unsigned int ronde);
	
	/**
	 * Hulpfunctie voor berekenStandNaRonde(); verwerkt de uitslag van 1 partij in de gegeven ranglijst.
	 * @param partij De partij waarvan de uitslag verwerkt moet worden.
	 * @param waarderingsRanglijst De bron van te gebruiken eigenwaarden.
	 * @param nieuweRanglijst De ranglijst waarin de partij moet worden opgenomen; dit is zowel een invoer- als uitvoerparameter.
	 */
	virtual void _verwerkPartij(Partij* partij, Ranglijst* waarderingsRanglijst, Ranglijst* nieuweRanglijst);
	
	/**
	 * Hulpfunctie voor berekenStandNaRonde(); zoekt uit wie er afwezig was in een gegeven ronde en geeft die personen eventueel wat punten.
	 * @param ronde De uitslagen van de gespeelde partijen deze ronde.
	 * @param waarderingsRanglijst De bron van te gebruiken eigenwaarden.
	 * @param nieuweRanglijst De te wijzigen ranglijst waarin eventueel punten moeten worden toebedeeld voor afwezigheid.
	 */
	virtual void _verwerkAfwezigeSpelers(Ronde* ronde, Ranglijst* waarderingsRanglijst, Ranglijst* nieuweRanglijst);
	
	/**
	 * Vult de datastructuur _tegenstandersPerSpeler m.b.v. de uitslagen in _rondes.
	 */
	virtual void _vulTegenstanderLijsten();
	
	
	
	///// PROTECTED DATASTRUCTURES /////
	
	/**
	 * Output stream to write status messages to.
	 */
	std::ostream* _logStream;
	
	/**
	 * Hulpobject voor het delegeren van IO taken.
	 */
	KeizerIO* _keizerIO;
	
	std::auto_ptr<CompetitieParameters> _competitieParameters;

	/**
	 * De spelers die deelnemen aan de competitie.
	 */
	Spelerslijst _spelerslijst;

	/**
	 * Geeft aan hoeveel rondes er gespeeld zijn.
	 */	
	unsigned int _nrRondes;
	
	/**
	 * _rondes[n] bevat de uitslag van ronde n voor n > 0, en _rondes[0] is NULL.
	 */
	std::vector<Ronde*> _rondes;
	
	/**
	 * _ranglijstNaRonde[0] is de beginranglijst, en _ranglijstNaRonde[n] is de ranglijst na ronde n.
	 */
	std::vector<Ranglijst*> _ranglijstNaRonde;
	
	/**
	 * Hulpdatastructuur om makkelijk op te kunnen zoeken tegen wie een gegeven speler allemaal gespeeld heeft.
	 * Element _tegenstandersPerSpeler[x] is een vector van speler-IDs geïndexeerd op ronde-nummer.
	 * Element _tegenstandersPerSpeler[x][r] geeft dus het speler-ID van de tegenstander van speler x in ronde r.
	 */
	std::vector<std::vector<int> > _tegenstandersPerSpeler;
};

#endif // COMPETITIE_H


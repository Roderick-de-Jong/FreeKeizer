#ifndef COMPETITION_H
#define COMPETITION_H

/*************************************************************************
 * Copyright 2012 Roderick de Jong                                       *
 *                                                                       *
 * This file is part of the FreeKeizer project.                          *
 * FreeKeizer is free software: you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation, either version 3 of the License, or     *
 *  (at your option) any later version.                                  *
 *                                                                       *
 * FreeKeizer is distributed in the hope that it will be useful,         *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 * GNU General Public License for more details.                          *
 *                                                                       *
 * You should have received a copy of the GNU General Public License     *
 * along with FreeKeizer.  If not, see <http://www.gnu.org/licenses/>.   *
 *                                                                       *
 *************************************************************************/

#include <memory>
#include <stdexcept>
#include <vector>
#include <iostream>

#include "CompetitieParameters.h"
#include "IOServices.h"
#include "Ranking.h"
#include "Round.h"
#include "PlayerList.h"

namespace FreeKeizer
{

// Forward declarations
class IOServices;



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
class Competition
{
	public:
	Competition();
	virtual ~Competition();
	// TODO: copy constructor, operator=
	
	
	///// MUTATORS /////
	
	/**
	 * Leest één of meerdere bestanden met competitiegegevens.
	 */
	virtual void readCompetitionDatabase();
	
	/**
	 * Voegt een lege Ronde aan de competitie toe.
	 * Deze ronde kan vervolgens handmatig of automatisch worden ingedeeld,
	 * of met een combinatie van beide.
	 * TODO: zorgen dat dit alleen aangeroepen kan worden als de vorige ronde voltooid is
	 */
	virtual void newRound();
	
	/**
	 * Neemt de meest recente ronde en alle spelers die nog niet ingedeeld zijn
	 * in die ronde, en probeert de ronde aan te vullen met partijen tussen die
	 * spelers.
	 */
	virtual void generatePairing();
	
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
	virtual unsigned int getNrRounds();
	
	/**
	 * Retourneert een kopie van de lijst met spelers in de competitie.
	 */
	virtual std::auto_ptr<PlayerList> getPlayerList();
	
	/**
	 * Retourneert een kopie van de ranglijst na de gevraagde ronde.
	 * @param ronde Geeft aan welke ranglijst gewenst is; bereik is [0;n] waarbij n het aantal gespeelde rondes is in deze competitie.
	 */
	virtual std::auto_ptr<Ranking> getRankingAfterRound(unsigned int round);
	
	/**
	 * Retourneert een kopie van de partijen (incl. uitslagen) van de gevraagde ronde.
	 * @param ronde Geeft aan voor welke ronde de uitslag gevraagd wordt; bereik is [0;n] waarbij n het aantal gespeelde rondes is in deze competitie.
	 * TODO: opvragen van uitslag ronde 0 illegaal maken? Da's een NULL pointer namelijk.
	 */
	virtual std::auto_ptr<Round> getRound(unsigned int round);

	
	
	///// OUTPUT METHODS /////
	
	/**
	 * Schrijft een ranglijst naar een bestand.
	 * @param ronde Geeft aan welke ranglijst gebruikt moet worden; gebruikt wordt de ranglijst van direct na de gegeven ronde. Geef ronde 0 op voor de beginranglijst.
	 * @see KeizerIO::writeRanglijstDocument()
	 */
	virtual void writeRankingDocument(unsigned int round);
	
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
	virtual std::vector<int> getOpponentList(unsigned int playerId);
	
	
	
	///// PROTECTED METHODS /////
	
	protected:
	
	/**
	 * Berekent de stand in de competitie na de huidige ronde (n).
	 * Voert een gedeeltelijke herberekening (1 iteratie) uit m.b.v. de stand na ronde (n - 1).
	 */
	virtual void _calculateRankings();
	
	/**
	 * Berekent de stand in de competitie na de gegeven ronde.
	 * Voert een gedeeltelijke herberekening (1 iteratie) uit m.b.v. de stand voor die ronde.
	 */
	virtual void _calculateRankingAfterRound(unsigned int round);
	
	/**
	 * Hulpfunctie voor berekenStandNaRonde(); verwerkt de uitslag van 1 partij in de gegeven ranglijst.
	 * @param partij De partij waarvan de uitslag verwerkt moet worden.
	 * @param waarderingsRanglijst De bron van te gebruiken eigenwaarden.
	 * @param nieuweRanglijst De ranglijst waarin de partij moet worden opgenomen; dit is zowel een invoer- als uitvoerparameter.
	 */
	virtual void _processGame(Game* game, Ranking* valuationRanking, Ranking* updatedRanking);
	
	/**
	 * Hulpfunctie voor berekenStandNaRonde(); zoekt uit wie er afwezig was in een gegeven ronde en geeft die personen eventueel wat punten.
	 * @param ronde De uitslagen van de gespeelde partijen deze ronde.
	 * @param waarderingsRanglijst De bron van te gebruiken eigenwaarden.
	 * @param nieuweRanglijst De te wijzigen ranglijst waarin eventueel punten moeten worden toebedeeld voor afwezigheid.
	 */
	virtual void _processAbsentPlayers(Round* round, Ranking* valuationRanking, Ranking* updatedRanking);
	
	/**
	 * Vult de datastructuur _tegenstandersPerSpeler m.b.v. de uitslagen in _rondes.
	 */
	virtual void _fillOpponentLists();
	
	
	
	///// PROTECTED DATASTRUCTURES /////
	
	/**
	 * Output stream to write status messages to.
	 */
	std::ostream* _logStream;
	
	/**
	 * Hulpobject voor het delegeren van IO taken.
	 */
	IOServices* _ioServices;
	
	std::auto_ptr<CompetitieParameters> _competitieParameters;

	/**
	 * De spelers die deelnemen aan de competitie.
	 */
	PlayerList _playerList;

	/**
	 * Geeft aan hoeveel rondes er gespeeld zijn.
	 * TODO: weg-refactoren, en _rondes.size() gebruiken!!
	 */	
	unsigned int _nrRounds;
	
	/**
	 * _rondes[n] bevat de uitslag van ronde n voor n > 0, en _rondes[0] is NULL.
	 */
	std::vector<Round*> _rounds;
	
	/**
	 * _ranglijstNaRonde[0] is de beginranglijst, en _ranglijstNaRonde[n] is de ranglijst na ronde n.
	 */
	std::vector<Ranking*> _rankingAfterRound;
	
	/**
	 * Hulpdatastructuur om makkelijk op te kunnen zoeken tegen wie een gegeven speler allemaal gespeeld heeft.
	 * Element _tegenstandersPerSpeler[x] is een vector van speler-IDs geïndexeerd op ronde-nummer.
	 * Element _tegenstandersPerSpeler[x][r] geeft dus het speler-ID van de tegenstander van speler x in ronde r.
	 */
	std::vector<std::vector<int> > _opponentPerPlayer;
};

}

#endif // COMPETITION_H


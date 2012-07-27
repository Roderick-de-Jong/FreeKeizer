#ifndef KEIZERIO_H
#define KEIZERIO_H

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

#include "Competition.h"
#include "CompetitieParameters.h"
#include "Ranking.h"
#include "Round.h"
#include "Player.h"
#include "PlayerList.h"

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


#ifndef RONDE_H
#define RONDE_H

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

#include <iostream>
#include <string>
#include <vector>

#include "Partij.h"
#include "Spelerslijst.h"

/**
 * Representeert de nog te spelen of reeds gespeelde partijen in een ronde van de competitie.
 * Kan worden gebruikt voor zowel het registreren en opslaan van uitslagen als het maken
 * van een indeling voor een ronde.
 */
class Ronde
{
	public:
	Ronde();
	virtual ~Ronde();
	
	/**
	 * Geeft aan of alle partijen van deze ronde gespeeld zijn.
	 * @return false als er nog minstens 1 partij is met waarde NOG_TE_SPELEN
	 *         true als er 0 partijen zijn met waarde NOG_TE_SPELEN
	 */
	virtual bool isVoltooid();
	
	virtual void dump(Spelerslijst* spelerslijst, std::ostream* outputStream);

	std::string datum;
	std::vector<Partij*> partijen;
};

#endif // RONDE_H


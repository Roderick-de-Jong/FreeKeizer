#ifndef ROUND_H
#define ROUND_H

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

#include "Game.h"
#include "PlayerList.h"

/**
 * Representeert de nog te spelen of reeds gespeelde partijen in een ronde van de competitie.
 * Kan worden gebruikt voor zowel het registreren en opslaan van uitslagen als het maken
 * van een indeling voor een ronde.
 */
class Round
{
	public:
	Round();
	virtual ~Round();
	
	/**
	 * Geeft aan of alle partijen van deze ronde gespeeld zijn.
	 * @return false als er nog minstens 1 partij is met waarde NOG_TE_SPELEN
	 *         true als er 0 partijen zijn met waarde NOG_TE_SPELEN
	 */
	virtual bool isCompleted();
	
	virtual void dump(PlayerList* playerList, std::ostream* outputStream);

	std::string date;
	std::vector<Game*> games;
};

#endif // ROUND_H


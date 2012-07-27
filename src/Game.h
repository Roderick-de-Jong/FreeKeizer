#ifndef PARTIJ_H
#define PARTIJ_H

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

#include "ChessConstants.h"

/**
 * Representatie van (de uitslag van) één individuele schaakpartij.
 * Wordt zowel gebruikt bij het indelen van partijen als het verwerken van uitslagen.
 */
class Partij
{
	public:
	Partij();
	virtual ~Partij();
	
	/**
	* Het unieke ID van de witspeler.
	*/
	int idWit;
	
	/**
	* Het unieke ID van de zwartspeler, of -1 bij geen zwartspeler, bijv. bij een vrije ronde.
	*/
	int idZwart;
	
	/**
	* Code die aangeeft wat de uitslag van de partij was.
	*/
	enum PartijResultaat resultaat;
};

#endif // PARTIJ_H


#ifndef SPELERSLIJST_H
#define SPELERSLIJST_H

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
#include <vector>

#include "Speler.h"
#include "Unittest.h"



// TODO: IMPORTANT: add a (dummy) player 0 to the player database, now that player IDs start at 0!!!!!

class Spelerslijst
{
	friend class FreeKeizerUnittest;
	
	public:
	
	/**
	 * Default constructor.
	 */
	Spelerslijst();
	
	/**
	 * Copy constructor.
	 */
	Spelerslijst(Spelerslijst& original);
	
	/**
	 * Destructor.
	 */
	virtual ~Spelerslijst();
	
	/**
	 * Assignment operator.
	 */
	Spelerslijst& operator=(const Spelerslijst& original);
	
	virtual Speler* getSpelerById(unsigned int spelerId);
	
	/**
	 * Returns:
	 * De speler met de naam in kwestie, of NULL indien niet gevonden.
	 */
	virtual Speler* getSpelerByNaam(const std::string& spelerNaam);

	virtual unsigned int getNrSpelers();
	
	virtual void voegSpelerToe(Speler* speler);
	
	/**
	 * Geeft de spelerslijst weer op de gegeven outputstream.
	 */
	virtual void dump(std::ostream* outputStream);
	
	protected:
	
	virtual void _verwijderAlleSpelers();
	
	std::vector<Speler*> _spelers;
};

#endif // SPELERSLIJST_H


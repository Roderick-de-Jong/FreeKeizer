#ifndef PAIRINGGENERATOR_H
#define PAIRINGGENERATOR_H

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

#include "Competition.h"
#include "Round.h"


/**
 * Produceert mogelijke indelingen voor de volgende ronde van een competitie.
 * Dit is een abstract class.
 */
class PairingGenerator
{
	public:
	PairingGenerator(Competition* competition);
	virtual ~PairingGenerator() { }
	
	virtual Round* generate() = 0;
	
	protected:
	Competition* _competition;
};

#endif /* PAIRINGGENERATOR_H */

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

#include "GAPairingGenerator.h"

namespace FreeKeizer
{

GAPairingGenerator::GAPairingGenerator(Competition* competition)
	: PairingGenerator(competition)
{
}

GAPairingGenerator::~GAPairingGenerator()
{
}

Round* GAPairingGenerator::generate()
{
	std::auto_ptr<PlayerList> playerList(_competition->getPlayerList());
	
	// TODO: run genetic algorithm
	
	Round* round = new Round();
	// TODO: store GA's result in Ronde object
	
	return round;
}

}

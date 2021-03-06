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

#include "RanglijstItem.h"

RanglijstItem::RanglijstItem()
{
	plaats = 0;
	spelerId = -1;
	punten = 0;
	eigenwaarde = 0;
	nrPartijenGespeeld = 0;
	nrPartijenGewonnen = 0;
	nrPartijenRemise = 0;
	nrPartijenVerloren = 0;
	nrVrijeRondes = 0;
	nrAfwezig = 0;
	wedstrijdPunten = 0;
	nrPartijenWit = 0;
	nrPartijenZwart = 0;
}

RanglijstItem::~RanglijstItem()
{
}

RanglijstItem& RanglijstItem::operator=(const RanglijstItem& original)
{
	if(this != &original)
	{
		plaats = original.plaats;
		spelerId = original.spelerId;
		punten = original.punten;
		eigenwaarde = original.eigenwaarde;
		nrPartijenGespeeld = original.nrPartijenGespeeld;
		nrPartijenGewonnen = original.nrPartijenGewonnen;
		nrPartijenRemise = original.nrPartijenRemise;
		nrPartijenVerloren = original.nrPartijenVerloren;
		nrVrijeRondes = original.nrVrijeRondes;
		nrAfwezig = original.nrAfwezig;
		wedstrijdPunten = original.wedstrijdPunten;
		nrPartijenWit = original.nrPartijenWit;
		nrPartijenZwart = original.nrPartijenZwart;		
	}
	
	return *this;
}




#ifndef RANGLIJSTITEM_H
#define RANGLIJSTITEM_H

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

#include "Spelerslijst.h"

/**
 * Datastructuur voor één regel met spelersstatistieken op een ranglijst.
 *
 * Definitie: TotaalAantalCompetitieRondes = nrAanwezig + nrAfwezig
 * Definitie: nrAanwezig = nrGespeeld + nrVrijeRondes
 * Definitie: nrGespeeld = nrGewonnen + nrRemise + nrVerloren = nrWit + nrZwart
 */
class RanglijstItem
{
	public:

	/**
	* Default constructor.
	*/
	RanglijstItem();

	/**
	* Copy constructor.
	*/
	RanglijstItem(RanglijstItem& original) : spelerId(original.spelerId), punten(original.punten), eigenwaarde(original.eigenwaarde), nrPartijenGespeeld(original.nrPartijenGespeeld),
	                                       nrPartijenGewonnen(original.nrPartijenGewonnen), nrPartijenRemise(original.nrPartijenRemise), nrPartijenVerloren(original.nrPartijenVerloren),
                                           nrVrijeRondes(original.nrVrijeRondes), nrAfwezig(original.nrAfwezig), wedstrijdPunten(original.wedstrijdPunten),
                                           nrPartijenWit(original.nrPartijenWit), nrPartijenZwart(original.nrPartijenZwart)
	{
		
	}
  
  virtual ~RanglijstItem();
  
  /**
   * Assignment operator.
   */
  RanglijstItem& operator=(const RanglijstItem& original);
  
  /**
   * Geeft de plaats van de speler op de ranglijst aan. De hoogste (beste) plaats is 1.
   */
  int plaats;
  
  /**
   * Het unieke ID van de speler op deze plaats op de ranglijst.
   */
  unsigned int spelerId;
  
  /**
   * Het totaal aantal verzamelde keizerpunten van deze speler.
   */
  int punten;
  int eigenwaarde;
  int nrPartijenGespeeld;
  int nrPartijenGewonnen;
  int nrPartijenRemise;
  int nrPartijenVerloren;
  int nrVrijeRondes;
  int nrAfwezig;
  float wedstrijdPunten;
  int nrPartijenWit;
  int nrPartijenZwart;
};

#endif // RANGLIJSTITEM_H


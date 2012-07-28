#ifndef RANKINGITEM_H
#define RANKINGITEM_H

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

#include "PlayerList.h"

/**
 * Datastructuur voor één regel met spelersstatistieken op een ranglijst.
 *
 * Definitie: TotaalAantalCompetitieRondes = nrAanwezig + nrAfwezig
 * Definitie: nrAanwezig = nrGespeeld + nrVrijeRondes
 * Definitie: nrGespeeld = nrGewonnen + nrRemise + nrVerloren = nrWit + nrZwart
 */
class RankingItem
{
	public:

	/**
	* Default constructor.
	*/
	RankingItem();

	/**
	* Copy constructor.
	*/
	RankingItem(RankingItem& original) : playerId(original.playerId), points(original.points), eigenvalue(original.eigenvalue), nrGamesPlayed(original.nrGamesPlayed),
	                                       nrGamesWon(original.nrGamesWon), nrGamesDrawn(original.nrGamesDrawn), nrGamesLost(original.nrGamesLost),
                                           nrRoundsFree(original.nrRoundsFree), nrAbsent(original.nrAbsent), score(original.score),
                                           nrGamesWhite(original.nrGamesWhite), nrGamesBlack(original.nrGamesBlack)
	{
		
	}
  
  virtual ~RankingItem();
  
  /**
   * Assignment operator.
   */
  RankingItem& operator=(const RankingItem& original);
  
  /**
   * Geeft de plaats van de speler op de ranglijst aan. De hoogste (beste) plaats is 1.
   */
  int place;
  
  /**
   * Het unieke ID van de speler op deze plaats op de ranglijst.
   */
  unsigned int playerId;
  
  /**
   * Het totaal aantal verzamelde keizerpunten van deze speler.
   */
  int points;
  int eigenvalue;
  int nrGamesPlayed;
  int nrGamesWon;
  int nrGamesDrawn;
  int nrGamesLost;
  int nrRoundsFree;
  int nrAbsent;
  float score;
  int nrGamesWhite;
  int nrGamesBlack;
};

#endif // RANKINGITEM_H


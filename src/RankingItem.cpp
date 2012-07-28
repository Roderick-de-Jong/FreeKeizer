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

#include "RankingItem.h"

RankingItem::RankingItem()
{
	place = 0;
	playerId = -1;
	points = 0;
	eigenvalue = 0;
	nrGamesPlayed = 0;
	nrGamesWon = 0;
	nrGamesDrawn = 0;
	nrGamesLost = 0;
	nrRoundsFree = 0;
	nrAbsent = 0;
	score = 0;
	nrGamesWhite = 0;
	nrGamesBlack = 0;
}

RankingItem::~RankingItem()
{
}

RankingItem& RankingItem::operator=(const RankingItem& original)
{
	if(this != &original)
	{
		place = original.place;
		playerId = original.playerId;
		points = original.points;
		eigenvalue = original.eigenvalue;
		nrGamesPlayed = original.nrGamesPlayed;
		nrGamesWon = original.nrGamesWon;
		nrGamesDrawn = original.nrGamesDrawn;
		nrGamesLost = original.nrGamesLost;
		nrRoundsFree = original.nrRoundsFree;
		nrAbsent = original.nrAbsent;
		score = original.score;
		nrGamesWhite = original.nrGamesWhite;
		nrGamesBlack = original.nrGamesBlack;		
	}
	
	return *this;
}




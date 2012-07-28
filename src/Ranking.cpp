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

#include <algorithm>
#include <stdexcept>
#include <sstream>

#include "Ranking.h"
#include "RankingItem.h"

Ranking::Ranking()
{
}

Ranking::~Ranking()
{
	_deleteAllItems();
}



Ranking::Ranking(Ranking& original)
{
	_items.resize(original._items.size());
	for(unsigned int i = 0; i < original._items.size(); i++)
		_items[i] = new RankingItem(*original._items[i]);
}



Ranking& Ranking::operator=(const Ranking& original)
{
	if(this != &original)
	{
		_deleteAllItems();
		_items.resize(original._items.size());
		for(unsigned int i = 0; i < original._items.size(); i++)
			_items[i] = new RankingItem(*original._items[i]);
	}
	
	return *this;
}



unsigned int Ranking::getLength()
{
	return _items.size();
}



RankingItem* Ranking::getItemAtPlace(unsigned int place)
{
	if(place == 0)
		throw std::invalid_argument("Argument 'plaats' is 0, maar de ranglijst begint bij plaats 1.");
	if(place > _items.size())
		throw std::invalid_argument("Argument 'plaats' is groter dan het aantal spelers op de ranglijst.");

	return _items.at(place - 1); // -1 omdat de eerste plek op de ranglijst in element 0 wordt opgeslagen
}



RankingItem* Ranking::getRankingItemByPlayerId(unsigned int playerId)
{
  RankingItem* rankingItem = NULL;
  for(unsigned int r = 0; r < _items.size(); r++)
  {
    rankingItem = _items.at(r);
    if(rankingItem->playerId == playerId)
      return rankingItem;
  }

	std::ostringstream message;
	message << "Speler ID " << playerId << " niet gevonden in ranglijst.";
	throw std::invalid_argument(message.str());
}



void Ranking::addItem(RankingItem* item)
{
	_items.push_back(item);
}



void Ranking::sort()
{
	// Look up highest eigenvalue
	int highestEigenvalue = -1;
	for(unsigned int p = 0; p < _items.size(); p++)
		if(_items[p]->eigenvalue > highestEigenvalue)
			highestEigenvalue = _items[p]->eigenvalue;

	// Sort the list on keizerpoints
	std::sort(_items.begin(), _items.end(), compareRankingItems);
	
	// Update eigenvalues
	for(unsigned int s = 0; s < _items.size(); s++)
	{
		_items.at(s)->eigenvalue = highestEigenvalue - s;
	}
}



bool Ranking::compareRankingItems(RankingItem* a, RankingItem* b)
{
	return a->points > b->points;
}



void Ranking::dump(PlayerList* playerList, std::ostream* outputStream)
{
	RankingItem* rankingItem = NULL;
	Player* player = NULL;

	if(outputStream != NULL)
		(*outputStream) << "Ranking:" << std::endl;

	for(unsigned int r = 0; r < _items.size(); r++)
	{
		rankingItem = _items.at(r);
		player = playerList->getPlayerById(rankingItem->playerId);
		if(outputStream != NULL)
		{
			(*outputStream) << r + 1 << ". " << player->name << " " << rankingItem->points << " "
			                << rankingItem->eigenvalue << "   gsp:" << rankingItem->nrGamesPlayed << " gw:" << rankingItem->nrGamesWon
			                << " rem:" << rankingItem->nrGamesDrawn << " vrl:" << rankingItem->nrGamesLost
			                << " vrij:" << rankingItem->nrRoundsFree << " wpt:" << rankingItem->score
			                << "    (id: " << rankingItem->playerId << ")" << std::endl;
		}
	}
}



void Ranking::_deleteAllItems()
{
	for(unsigned int i = 0; i < _items.size(); i++)
	{
		if(_items[i] != NULL)
		{
			delete _items[i];
			_items[i] = NULL;
		}
	}
	_items.clear();
}



Ranking* Ranking::createInitialRanking(int highestEigenvalue, PlayerList* playerList)
{
  Ranking* initialRanking = new Ranking();
  RankingItem* rankingItem = NULL;
  
  for(unsigned int p = 0; p < playerList->getNrPlayers(); p++)
  {
  	Player* player = playerList->getPlayerById(p);
    rankingItem = new RankingItem();
    rankingItem->playerId = player->id;
    rankingItem->eigenvalue = highestEigenvalue - p;
    rankingItem->points = rankingItem->eigenvalue;
    initialRanking->addItem(rankingItem);
  }
  
  return initialRanking;
}

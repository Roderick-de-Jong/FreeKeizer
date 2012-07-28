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

#include <stdexcept>

#include "PlayerList.h"

PlayerList::PlayerList()
{
}

PlayerList::PlayerList(PlayerList& original)
{
	_players.resize(original._players.size());
	for(unsigned int s = 0; s < original._players.size(); s++)
		_players.at(s) = new Player(*original._players.at(s));
}

PlayerList::~PlayerList()
{
	_deleteAllPlayers();
}

PlayerList& PlayerList::operator=(const PlayerList& original)
{
	if(this != &original)
	{
		_deleteAllPlayers();
		_players.resize(original._players.size());
		for(unsigned int s = 0; s < original._players.size(); s++)
			_players.at(s) = new Player(*original._players.at(s));
	}
	
	return *this;
}

Player* PlayerList::getPlayerById(unsigned int playerId)
{
	return _players.at(playerId);
}

Player* PlayerList::getPlayerByName(const std::string& playerName)
{
	for(unsigned int s = 0; s < _players.size(); s++)
		if(_players[s]->name == playerName)
			return _players[s];
	return NULL;
}

unsigned int PlayerList::getNrPlayers()
{
	return _players.size();
}

void PlayerList::addPlayer(Player* player)
{
	_players.push_back(player);
	player->id = _players.size() - 1;
}

void PlayerList::dump(std::ostream* outputStream)
{
  Player* player = NULL;
  
  if(outputStream != NULL)
  {
	(*outputStream) << "Spelerslijst:" << std::endl;
	for(unsigned int s = 0; s < _players.size(); s++)
	{
		player = _players.at(s);
		(*outputStream) << player->id << " " << player->name << std::endl;
	}
  }
}

void PlayerList::_deleteAllPlayers()
{
	for(unsigned int s = 0; s < _players.size(); s++)
	{
		if(_players[s] != NULL)
		{
			delete _players[s];
			_players[s] = NULL;
		}
	}
	_players.clear();
}


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

#include "Round.h"
#include "PlayerList.h"

namespace FreeKeizer
{

Round::Round()
{
}

Round::~Round()
{
  for(unsigned int u = 0; u < games.size(); u++)
  {
    delete games[u];
    games[u] = NULL;
  }
  games.clear();
}

bool Round::isCompleted()
{
	for(std::vector<Game*>::iterator i = games.begin(); i != games.end(); i++)
		if((*i)->result == GameResult::YET_TO_BE_PLAYED)
			return false;
	return true;
}

void Round::dump(PlayerList* playerList, std::ostream* outputStream)
{
  Game* game = NULL;
  Player* playerWhite = NULL;
  Player* playerBlack = NULL;
  
  if(outputStream != NULL)
  {  
	for(unsigned int g = 0; g < games.size(); g++)
	{
		game = games[g];
		(*outputStream) << (int)(game->result) << " " << game->idWhite << " " << game->idBlack << " ---> ";
			playerWhite = playerList->getPlayerById(game->idWhite);
			playerBlack = NULL;
			if(game->idBlack >= 0)
				playerBlack = playerList->getPlayerById(game->idBlack);
		switch(game->result)
		{
		case GameResult::WHITE_WINS:
			(*outputStream) << playerWhite->name << " - " << playerBlack->name << " 1 - 0" << std::endl;
			break;
		case GameResult::BLACK_WINS:
			(*outputStream) << playerWhite->name << " - " << playerBlack->name << " 0 - 1" << std::endl;
			break;
		case GameResult::DRAW:
			(*outputStream) << playerWhite->name << " - " << playerBlack->name << " 1/2 - 1/2" << std::endl;
			break;
		case GameResult::FREE:
			(*outputStream) << playerWhite->name  << " vrij" << std::endl;
			break;
		default:
			(*outputStream) << "Ongedefiniëerd resultaat." << std::endl;
			break;
		}
	}
  }
}

}

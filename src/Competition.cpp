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

#include <cmath>
#include <memory>
#include <set>
#include <stdexcept>
#include <iostream>

#include "Competition.h"
#include "CompetitieParameters.h"
#include "Ranking.h"
#include "ChessConstants.h"
#include "GAPairingGenerator.h"

//#define DEBUG_DUMP_COMPETITIEPARAMETERS
//#define DEBUG_DUMP_SPELERSLIJST
//#define DEBUG_DUMP_STARTRANGLIJST
//#define DEBUG_DUMP_UITSLAGEN

Competition::Competition()
{
	_logStream = NULL;
	_nrRounds = 0;
	_ioServices = new IOServices();
	_rounds.push_back(NULL); // Round 0 doesn't actually exist.
}

Competition::~Competition()
{
	delete _ioServices;
	_ioServices = NULL; // TODO: make it an auto_ptr
}



void Competition::readCompetitionDatabase()
{
	if(_logStream != NULL)
	{
		(*_logStream) << "Bezig met lezen van competitieparameters...";
		_logStream->flush();
	}
	
	_competitieParameters = _ioServices->leesCompetitieParameters();
	_nrRounds = _competitieParameters->aantalRondes;
	
	if(_logStream != NULL)
	{
#ifdef DEBUG_DUMP_COMPETITIEPARAMETERS
		(*_logStream) << std::endl;
		_competitieParameters->dump(_logStream);
#endif
		(*_logStream) << std::endl << "Bezig met lezen van spelerslijst...";
		_logStream->flush();
	}
		
	_ioServices->readPlayers(&_playerList);
	
	if(_logStream != NULL)
	{
#ifdef DEBUG_DUMP_SPELERSLIJST
		(*_logStream) << std::endl;
		_playerList.dump(_logStream);
#endif		
		(*_logStream) << std::endl << "Bezig met opstellen van startranglijst..."; 
		_logStream->flush();
	}
	
	_rankingAfterRound.clear(); // TODO: vervangen door VOLLEDIGE RESET-FUNCTIE!!
	_rankingAfterRound.push_back(Ranking::createInitialRanking(_competitieParameters->hoogsteEigenwaarde, &_playerList));
	
	if(_logStream != NULL)
	{
#ifdef DEBUG_DUMP_STARTRANGLIJST
		(*_logStream) << std::endl;
		rankingAfterRound[0]->dump(&_playerList, _logStream);
#endif
		(*_logStream) << std::endl << "Bezig met lezen van ronde-uitslagen..." << std::endl;
	}
	
  for(int r = 1; r <= _competitieParameters->aantalRondes; r++)
  {
    if(_logStream != NULL)
    {
    	(*_logStream) << "Bezig met lezen van uitslag voor ronde: " << r;
    	_logStream->flush();
    }
    Round* round = _ioServices->readRound(r);
    _rounds.push_back(round);
    if(_logStream != NULL)
	{
    	(*_logStream) << std::endl;
#ifdef DEBUG_DUMP_UITSLAGEN
		round->dump(&_playerList, _logStream);
#endif
	}
  }
  
  if(_logStream != NULL)
		(*_logStream) << "Klaar met inlezen van competitiedatabase. Bezig met berekenen van competitiestand..." << std::endl;
  
  _calculateRankings();
  
  if(_logStream != NULL)
		(*_logStream) << "Klaar met berekenen." << std::endl;
}



void Competition::newRound()
{
	if(_logStream != NULL)
	{
		(*_logStream) << "Niewe ronde toevoegen...";
		_logStream->flush();
	}
	
	_rounds.push_back(new Round());
	
	if(_logStream != NULL)
		(*_logStream) << "OK" << std::endl;
}



void Competition::generatePairing()
{
	if(_logStream != NULL)
		(*_logStream) << "Bezig met maken van indeling..." << std::endl;
	
	std::auto_ptr<GAPairingGenerator> pairingGenerator(new GAPairingGenerator(this));
	
	// Take the partial pairing:
	Round* fixedGames = _rounds.back();
	// Obtain a full pairing:
	Round* round = pairingGenerator->generate();
	
	// Replace the old pairing with the new one
	_rounds.pop_back();
	delete fixedGames;
	fixedGames = NULL;
	_rounds.push_back(round);
	
	if(_logStream != NULL)
		(*_logStream) << "Klaar met maken van indeling." << std::endl;
}



std::auto_ptr<CompetitieParameters> Competition::getCompetitieParameters()
{
	return std::auto_ptr<CompetitieParameters>(new CompetitieParameters(*_competitieParameters));
}



unsigned int Competition::getNrRounds()
{
	return _nrRounds;
}



std::auto_ptr<PlayerList> Competition::getPlayerList()
{
	return std::auto_ptr<PlayerList>(new PlayerList(_playerList));
}



void Competition::setLogStream(std::ostream* stream)
{
	_logStream = stream;
}



std::auto_ptr<Ranking> Competition::getRankingAfterRound(unsigned int round)
{
	if(round > _nrRounds)
		throw std::invalid_argument("Kan geen ranglijst geven voor een nog niet gespeelde ronde.");
		
	return std::auto_ptr<Ranking>(new Ranking(*_rankingAfterRound.at(round)));
}



std::auto_ptr<Round> Competition::getRound(unsigned int round)
{
	if(round > _rounds.size())
		throw std::invalid_argument("Kan geen rondes uit de toekomst geven.");
	
	return std::auto_ptr<Round>(new Round(*_rounds.at(round)));
}



void Competition::writeRankingDocument(unsigned int round)
{
	if(_logStream != NULL)
		(*_logStream) << "Bezig met schrijven van ranglijstdocument..."; _logStream->flush();
	
	_ioServices->writeRankingDocument(this, round);
	
	if(_logStream != NULL)
		(*_logStream) << "OK" << std::endl;
}



void Competition::_calculateRankings()
{
	if(_logStream != NULL)
		(*_logStream) << "Bezig met doorberekenen van de competitie..." << std::endl;
	
	// Sanity checks
	if(_rounds.size() < _nrRounds)
		throw std::logic_error("Kan stand niet berekenen - er missen uitslagen in de competitie.");

	// Delete previous versions of the rankings
	for(unsigned int r = 1; r < _rankingAfterRound.size(); r++)  // Preserve index 0 (save initial rankings)
	{
		delete _rankingAfterRound[r];
		_rankingAfterRound[r] = NULL;
	}
	_rankingAfterRound.resize(_nrRounds + 1); // Initial ranking + 1 ranking after each round
	
	// Calculate new rankings
	// TODO: first check whether the most recent round has been completed
	for(unsigned int r = 1; r <= _nrRounds; r++)
		_calculateRankingAfterRound(r);
}



void Competition::_calculateRankingAfterRound(unsigned int round)
{
	Ranking* valuationRanking;
	
	if(_logStream != NULL)
		(*_logStream) << "Bezig met berekenen van stand na ronde " << round << "..." << std::endl;
	
	valuationRanking = _rankingAfterRound[round - 1];

	// Start with a copy of the initial rankings
	Ranking* updatedRanking = new Ranking(*_rankingAfterRound[0]);

	// Process all games up til the given round
	for(unsigned int r = 1; r <= round; r++)
	{
		Round* objRound = _rounds[r];
		for(unsigned int g = 0; g < objRound->games.size(); g++)
			_processGame(objRound->games.at(g), valuationRanking, updatedRanking);
		_processAbsentPlayers(objRound, valuationRanking, updatedRanking);
	}
	
	// Sort on keizerpoints and update all eigenvalues
	updatedRanking->sort();
	
	// Save the rankings
	_rankingAfterRound[round] = updatedRanking;
}



void Competition::_processGame(Game* game, Ranking* valuationRanking, Ranking* updatedRanking)
{
	Player* playerWhite = _playerList.getPlayerById(game->idWhite);
	Player* playerBlack = NULL;
	RankingItem* valuationRankingItemWhite = valuationRanking->getRankingItemByPlayerId(playerWhite->id);
	RankingItem* valuationRankingItemBlack = NULL;
	RankingItem* updatedRankingItemWhite = updatedRanking->getRankingItemByPlayerId(playerWhite->id);
	RankingItem* updatedRankingItemBlack = NULL;
	if(game->idBlack >= 0)
	{
		playerBlack = _playerList.getPlayerById(game->idBlack);
		valuationRankingItemBlack = valuationRanking->getRankingItemByPlayerId(playerBlack->id);
		updatedRankingItemBlack = updatedRanking->getRankingItemByPlayerId(playerBlack->id);
	}
	
	double pointIncreaseWhite = 0;
	double pointIncreaseBlack = 0;
	switch(game->result)
	{
	case GameResult::WHITE_WINS:
			pointIncreaseWhite = WIN_FACTOR  * valuationRankingItemBlack->eigenvalue;
			pointIncreaseBlack = LOSS_FACTOR * valuationRankingItemWhite->eigenvalue;
			updatedRankingItemWhite->nrGamesWon++;
			updatedRankingItemBlack->nrGamesLost++;
			updatedRankingItemWhite->score += 1;
			break;
		case GameResult::BLACK_WINS:
			pointIncreaseWhite = LOSS_FACTOR * valuationRankingItemBlack->eigenvalue;
			pointIncreaseBlack = WIN_FACTOR  * valuationRankingItemWhite->eigenvalue;
			updatedRankingItemWhite->nrGamesLost++;
			updatedRankingItemBlack->nrGamesWon++;
			updatedRankingItemBlack->score += 1;
			break;
		case GameResult::DRAW:
			pointIncreaseWhite = DRAW_FACTOR * valuationRankingItemBlack->eigenvalue;
			pointIncreaseBlack = DRAW_FACTOR * valuationRankingItemWhite->eigenvalue;
			updatedRankingItemWhite->nrGamesDrawn++;
			updatedRankingItemBlack->nrGamesDrawn++;
			updatedRankingItemWhite->score += 0.5;
			updatedRankingItemBlack->score += 0.5;
			break;
		case GameResult::FREE:
			pointIncreaseWhite = FREE_FACTOR * valuationRankingItemWhite->eigenvalue;
			updatedRankingItemWhite->nrRoundsFree++;
			break;
		default:
			throw std::logic_error("Ongedefiniëerd resultaat tegengekomen bij verwerken partij-uitslag.");
			break;
	}
	
	if(game->result != GameResult::FREE)
	{
		updatedRankingItemWhite->nrGamesPlayed++;
		updatedRankingItemBlack->nrGamesPlayed++;
		updatedRankingItemWhite->nrGamesWhite++;
		updatedRankingItemBlack->nrGamesBlack++;
	}
	
	updatedRankingItemWhite->points = lround(updatedRankingItemWhite->points + pointIncreaseWhite);
	if(playerBlack != NULL)
		updatedRankingItemBlack->points = lround(updatedRankingItemBlack->points + pointIncreaseBlack);
}



void Competition::_processAbsentPlayers(Round* round, Ranking* valuationRanking, Ranking* updatedRanking)
{
	// Determine the players that are present
	std::set<int> presentPlayers;
	for(unsigned int g = 0; g < round->games.size(); g++)
	{
		Game* game = round->games[g];
		presentPlayers.insert(game->idWhite);
		if(game->idBlack >= 0)
			presentPlayers.insert(game->idBlack);
	}

	// Traverse ranking in search of absent players
	for(unsigned int place = 1; place <= updatedRanking->getLength(); place++)
	{
		RankingItem* rankingItem = updatedRanking->getItemAtPlace(place);
		int playerId = rankingItem->playerId;
		if(presentPlayers.find(playerId) == presentPlayers.end()) // Player absent?
		{
			rankingItem->nrAbsent++;
			// Perhaps award some points for being absent, if the player hasn't been absent too often.
			if(rankingItem->nrAbsent <= MAX_ROUNDS_ABSENT && rankingItem->nrGamesPlayed > 0)
			{
				int eigenvalue = valuationRanking->getRankingItemByPlayerId(playerId)->eigenvalue;
				rankingItem->points = lround(rankingItem->points + ABSENT_FACTOR * eigenvalue);
			}
		}
	}
}



void Competition::_fillOpponentLists()
{
	std::vector<int> emptyVector;
	
	_opponentPerPlayer.clear();
	
	// Initialise all player lists with the value for 'no opponent'.
	for(unsigned int p = 0; p < _playerList.getNrPlayers(); p++)
	{
		_opponentPerPlayer.push_back(emptyVector);
		_opponentPerPlayer.at(p).resize(_nrRounds + 1, NO_PLAYER); // _nrRondes + 1 omdat ronde 0 niet bestaat
	}
	
	// Process the game results
	for(unsigned int r = 1; r <= _nrRounds; r++)
	{
		Round* round = _rounds.at(r);
		for(unsigned int g = 0; g < round->games.size(); g++)
		{
			Game* game = round->games.at(g);
			_opponentPerPlayer.at(game->idWhite).at(r) = game->idBlack;
			if(game->idBlack != NO_PLAYER)
				_opponentPerPlayer.at(game->idBlack).at(r) = game->idWhite;
		}
	}
}



std::vector<int> Competition::getOpponentList(unsigned int playerId)
{
	if(playerId >= _opponentPerPlayer.size())
		throw std::invalid_argument("Speler ID buiten bereik.");
	
	return _opponentPerPlayer.at(playerId);  // <3 Copy constructors
}



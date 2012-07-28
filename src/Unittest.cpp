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

#include <string>
#include <vector>
#include <assert.h>

#include "Unittest.h"
#include "Game.h"
#include "Ranking.h"
#include "Round.h"
#include "PlayerList.h"


using namespace std;



int FreeKeizerUnittest::testPlayerList()
{
	int fails = 0;
	const char* rawNamedata[] = {"Jan", "Marjan", "Pia", "Piet", "Klaas", "Jan-Peter", "Victor", "Rudolf", "Eric", "Kees", "Wouter", "Jolande", "Annemarie"};
	unsigned int nrNames = 13;
	vector<string> namedata;

	cout << endl << "----- Unittest for SPELERSLIJST module -----" << endl;
	
	// Prepare testing data
	namedata.reserve(nrNames);
	for(unsigned int n = 0; n < nrNames; n++)
		namedata.push_back(rawNamedata[n]);
	
	cout << "Testing ID assignment at player addition..."; cout.flush();
	PlayerList* playerList = NULL;
	try
	{
		playerList = new PlayerList();
		for(unsigned int i = 0; i < namedata.size(); i++)
		{
			Player* player = new Player();
			player->name = namedata.at(i);
			playerList->addPlayer(player);
			if(player->id != i)
			{
				cout << "FAILED" << endl
					<< "\tPlayer " << i  << " was assigned id " << player->id << " which is not equal." << endl;
				fails++;
			}
		}
	}
	catch(std::exception& e)
	{
		cout << "EXCEPTION: " << e.what() << endl;
		fails++;
	}
	cout << "DONE" << endl;
	
	cout << "Testing player count and id-based player retrieval..."; cout.flush();
	unsigned int nrPlayers = 0;
	try
	{
		nrPlayers = playerList->getNrPlayers();
		if(nrPlayers != namedata.size())
		{
			cout << "FAILED" << endl
				<< "\tPlayer count incorrect; got " << nrPlayers << " instead of the expected " << namedata.size() << "." << endl;
			fails++;
		}
		for(unsigned int i = 0; i < nrPlayers; i++)
		{
			Player* player = playerList->getPlayerById(i);
			if(player->id != i)
			{
				cout << "FAILED" << endl
					<< "\tPlayer retrieval failed. Found id " << player->id  << " instead of " << i << "." << endl;
				fails++;
			}
			if(player->name != namedata.at(i))
			{
				cout << "FAILED" << endl
					<< "\tPlayer retrieval failed. Found name " << player->name  << " instead of " << namedata.at(i) << "." << endl;
				fails++;
			}
		}
	}
	catch(std::exception& e)
	{
		cout << "EXCEPTION: " << e.what() << endl;
		fails++;
	}
	cout << "DONE" << endl;
	
	cout << "Testing name-based player retrieval..."; cout.flush();
	try
	{
		for(unsigned int i = 0; i < nrPlayers; i++)
		{
			string name = namedata.at(i);
			Player* player = playerList->getPlayerByName(name);
			if(player->name != name)
			{
				cout << "FAILED" << endl
					<< "\tPlayer retrieval failed. Found name " << player->name  << " instead of " << name << "." << endl;
				fails++;
			}
			if(player->id != i)
			{
				cout << "FAILED" << endl
					<< "\tPlayer retrieval failed. Found id " << player->id  << " instead of " << i << "." << endl;
				fails++;
			}
		}
	}
	catch(std::exception& e)
	{
		cout << "EXCEPTION: " << e.what() << endl;
		fails++;
	}
	cout << "DONE" << endl;
	
	cout << "Testing copy contructor..."; cout.flush();
	PlayerList* listTwo = NULL;
	try
	{
		listTwo = new PlayerList(*playerList);
		for(unsigned int i = 0; i < nrPlayers; i++)
		{
			Player* playerA = playerList->getPlayerById(i);
			Player* playerB = listTwo->getPlayerById(i);
			if(playerA == playerB)
			{
				cout << "FAILED" << endl
					<< "\tFailed to make deep copy; pointers are identical (" << playerA << " and " << playerB << ")." << endl;
				fails++;
			}
			if(playerA->id != playerB->id)
			{
				cout << "FAILED" << endl
					<< "\tCopy error: player ID mismatch (found " << playerB->id << ", original was " << playerA->id << ")." << endl;
				fails++;
			}
			if(playerA->name != playerB->name)
			{
				cout << "FAILED" << endl
					<< "\tCopy error: player name mismatch (found " << playerB->name << ", original was " << playerA->name << ")." << endl;
				fails++;
			}
		}
	}
	catch(std::exception& e)
	{
		cout << "EXCEPTION: " << e.what() << endl;
		fails++;
	}
	cout << "DONE" << endl;
	
	cout << "Testing mass removal..."; cout.flush();
	try
	{
		playerList->_deleteAllPlayers();
		if(playerList->getNrPlayers() != 0)
		{
			cout << "FAILED" << endl
					<< "\tList isn't zero-length yet." << endl;
			fails++;
		}
		if(playerList->_players.size() != 0)
		{
			cout << "FAILED" << endl
					<< "\tInternal datastructure isn't zero-length yet." << endl;
			fails++;
		}
	}
	catch(std::exception& e)
	{
		cout << "EXCEPTION: " << e.what() << endl;
		fails++;
	}
	cout << "DONE" << endl;
	
	cout << "Testing assignment operator..."; cout.flush();
	try
	{
		*playerList = *listTwo;
		for(unsigned int i = 0; i < nrPlayers; i++)
		{
			Player* playerA = playerList->getPlayerById(i);
			Player* playerB = listTwo->getPlayerById(i);
			if(playerA == playerB)
			{
				cout << "FAILED" << endl
					<< "\tFailed to make deep copy; pointers are identical (" << playerA << " and " << playerB << ")." << endl;
				fails++;
			}
			if(playerA->id != playerB->id)
			{
				cout << "FAILED" << endl
					<< "\tCopy error: player ID mismatch (found " << playerA->id << ", original was " << playerB->id << ")." << endl;
				fails++;
			}
			if(playerA->name != playerB->name)
			{
				cout << "FAILED" << endl
					<< "\tCopy error: player name mismatch (found " << playerA->name << ", original was " << playerB->name << ")." << endl;
				fails++;
			}
		}
	}
	catch(std::exception& e)
	{
		cout << "EXCEPTION: " << e.what() << endl;
		fails++;
	}
	cout << "DONE" << endl;
	
	// TODO: test out-of-order look-ups
	
	cout << "Testing deletion of list 1..."; cout.flush();
	try
	{
		delete playerList;
		playerList = NULL;
	}
	catch(std::exception& e)
	{
		cout << "EXCEPTION: " << e.what() << endl;
		fails++;
	}
	cout << "DONE" << endl;
	
	cout << "Testing deletion of list 2..."; cout.flush();
	try
	{
		delete listTwo;
		listTwo = NULL;
	}
	catch(std::exception& e)
	{
		cout << "EXCEPTION: " << e.what() << endl;
		fails++;
	}
	cout << "DONE" << endl;
	
	return fails;
}



int FreeKeizerUnittest::testRanking()
{
	int fails = 0;
	const char* rawNamedata[] = {"Jan", "Marjan", "Pia", "Piet", "Klaas", "Jan-Peter", "Victor", "Rudolf", "Eric", "Kees", "Wouter", "Jolande", "Annemarie"};
	unsigned int nrNames = 13;
	vector<string> namedata;
	PlayerList* playerList = NULL;
	
	cout << endl << "----- Unittest for RANGLIJST module -----" << endl;
	
	// Prepare testing data
	namedata.reserve(nrNames);
	playerList = new PlayerList();
	for(unsigned int n = 0; n < nrNames; n++)
	{
		namedata.push_back(rawNamedata[n]);
		Player* player = new Player(namedata.at(n));
		playerList->addPlayer(player);
	}
	assert(playerList->getNrPlayers() == nrNames);
	
	cout << "Testing maakStartRanglijst() and destructor..."; cout.flush();
	Ranking* ranking = NULL;
	try
	{
		for(unsigned int highestEigenvalue = nrNames; highestEigenvalue < nrNames + 100; highestEigenvalue++)
		{
			ranking = Ranking::createInitialRanking(highestEigenvalue, playerList);
			for(unsigned int place = 1; place <= nrNames; place++) // Plaatsen beginnen bij 1
			{
				RankingItem* rankingItem = ranking->getItemAtPlace(place);
				if(rankingItem->playerId != place - 1)
				{
					cout << "FAILED" << endl
					     << "\tPlayer ID mismatch (found " << rankingItem->playerId << ", expected " << place - 1 << ")." << endl;
					fails++;
				}
				if(static_cast<unsigned int>(rankingItem->eigenvalue) != highestEigenvalue + 1 - place)   // +1 omdat plaatsen beginnen bij 1
				{
					cout << "FAILED" << endl
					     << "\tIncorrect eigenvalue (found " << rankingItem->eigenvalue << ", expected " << highestEigenvalue + 1 - place
					     << " for place " << place << " and max eigenvalue " << highestEigenvalue << ")." << endl;
					fails++;
				}
				if(rankingItem->points != rankingItem->eigenvalue)
				{
					cout << "FAILED" << endl
					     << "\tPoints (" << rankingItem->points << ") do not equal eigenvalue (" << rankingItem->eigenvalue << ")." << endl;
					fails++;
				}
			}
			delete ranking;
		}
	}
	catch(std::exception& e)
	{
		cout << "EXCEPTION: " << e.what() << endl;
		fails++;
	}
	cout << "DONE" << endl;
	
	cout << "Setting up list for more tests..."; cout.flush();
	try
	{
		unsigned int highestEigenvalue = 2 * nrNames;
		ranking = Ranking::createInitialRanking(highestEigenvalue, playerList);
		delete ranking;
		ranking = Ranking::createInitialRanking(highestEigenvalue, playerList);
	}
	catch(std::exception& e)
	{
		cout << "EXCEPTION: " << e.what() << endl;
		fails++;
	}
	cout << "DONE" << endl;
	
	cout << "Testing copy constructor and assignment operator..."; cout.flush();
	try
	{
		Ranking* rankingCopies[2];
		rankingCopies[0] = new Ranking(*ranking);  // Copy contructor
		rankingCopies[1] = new Ranking();
		*rankingCopies[1] = *ranking;  // Assignment operator
		for(int r = 0; r < 2; r++)
		{
			if(rankingCopies[r]->getLength() != ranking->getLength())
			{
				cout << "FAILED" << endl
				     << "\tList length mismatch (found " << rankingCopies[r]->getLength() << ", expected " << ranking->getLength() << ")." << endl;
				fails++;
			}
			for(unsigned int place = 1; place <= ranking->getLength(); place++)
			{
				RankingItem* originalItem = ranking->getItemAtPlace(place);
				RankingItem* clonedItem = rankingCopies[r]->getItemAtPlace(place);
				if(originalItem == clonedItem)
				{
					cout << "FAILED" << endl
						 << "\tFailed to make deep copy; pointers are identical (" << originalItem << " and " << clonedItem << ")." << endl;
				}
				if(originalItem->playerId != clonedItem->playerId)
				{
					cout << "FAILED" << endl
					     << "\tPlayer ID mismatch (found " << clonedItem->playerId << ", expected " << originalItem->playerId << ")." << endl;
					fails++;
				}
				if(originalItem->points != clonedItem->points)
				{
					cout << "FAILED" << endl
					     << "\tPlayer points mismatch (found " << clonedItem->points << ", expected " << originalItem->points << ")." << endl;
					fails++;
				}
				if(originalItem->eigenvalue != clonedItem->eigenvalue)
				{
					cout << "FAILED" << endl
					     << "\tPlayer eigenvalue mismatch (found " << clonedItem->eigenvalue << ", expected " << originalItem->eigenvalue << ")." << endl;
					fails++;
				}
			}
			delete rankingCopies[r];
		}	
	}
	catch(std::exception& e)
	{
		cout << "EXCEPTION: " << e.what() << endl;
		fails++;
	}
	cout << "DONE" << endl;
	
	cout << "Testing sort..."; cout.flush();
	try
	{
		Ranking* listTwo = new Ranking(*ranking);
		int swapDistance[] = {5,  7,  7, 1, 0,
		                      4, 12, 11, 6, 3,
							  8,  0,  8, 3, 7,
							  5,  8}; // Items are moved this far.
		int nrSwapDistances = 17;
		// Do some non-random permutations on the copy
		for(unsigned int i = 0; i < listTwo->getLength(); i++)
		{
			RankingItem* itemAtI = listTwo->getItemAtPlace(i + 1); // Plaats param is base 1
			int j = (i + swapDistance[i % nrSwapDistances]) % (listTwo->getLength());
			RankingItem* itemAtJ = listTwo->getItemAtPlace(j + 1); // Idem
			//cout << "Swapping items at " << i + 1 << " and " << j + 1 << "." << endl;
			listTwo->_items[i] = itemAtJ;
			listTwo->_items[j] = itemAtI;
		}
		// Test if sorting results in the original list
		listTwo->sort();
		if(listTwo->getLength() != ranking->getLength())
		{
			cout << "FAILED" << endl
			     << "\tList length mismatch (found " << listTwo->getLength() << ", expected " << ranking->getLength() << ")." << endl;
			fails++;
		}
		for(unsigned int place = 1; place <= ranking->getLength(); place++)
		{
			RankingItem* originalItem = ranking->getItemAtPlace(place);
			RankingItem* clonedItem = listTwo->getItemAtPlace(place);
			if(originalItem->playerId != clonedItem->playerId)
			{
				cout << "FAILED" << endl
						<< "\tPlayer ID mismatch (found " << clonedItem->playerId << ", expected " << originalItem->playerId << ")." << endl;
				fails++;
			}
			if(originalItem->points != clonedItem->points)
			{
				cout << "FAILED" << endl
						<< "\tPlayer points mismatch (found " << clonedItem->points << ", expected " << originalItem->points << ")." << endl;
				fails++;
			}
			if(originalItem->eigenvalue != clonedItem->eigenvalue)
			{
				cout << "FAILED" << endl
						<< "\tPlayer eigenvalue mismatch (found " << clonedItem->eigenvalue << ", expected " << originalItem->eigenvalue << ")." << endl;
				fails++;
			}
		}
		delete listTwo;
	}
	catch(std::exception& e)
	{
		cout << "EXCEPTION: " << e.what() << endl;
		fails++;
	}
	cout << "DONE" << endl;
	
	// TODO Speler & RanglijstItem refcounts
	// TODO in all tests, check all fields of RanglijstItem, not just the most important ones
	
	return fails;
}



int main(void)
{
	FreeKeizerUnittest unittest;
	int fails = 0;
	
	cout << "FreeKeizer unit tests" << endl;
	fails += unittest.testPlayerList();
	fails += unittest.testRanking();
	
	cout << endl << "TOTAL FAILS: " << fails << endl;
}

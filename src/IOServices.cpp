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
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <memory>
#include <iomanip>
#include <assert.h>
#include <string>
#include <stdexcept>

#include "IOServices.h"
#include "CompetitieParameters.h"
#include "Game.h"
#include "Round.h"
#include "PlayerList.h"



IOServices::IOServices()
{
}



IOServices::~IOServices()
{
}



std::auto_ptr<CompetitieParameters> IOServices::leesCompetitieParameters()
{
  std::auto_ptr<CompetitieParameters> competitieParameters;
  std::ifstream invoerBestand;
  char tempNaam[256];
  char tempDatum[256];
  
  invoerBestand.exceptions(std::ifstream::failbit | std::ifstream::badbit);  // Treat everything except EOF as an exception.
  invoerBestand.open("input/CompetitieParameters.txt", std::ios::in);

  competitieParameters = std::auto_ptr<CompetitieParameters>(new CompetitieParameters());
  invoerBestand.getline(tempNaam, 256);
  competitieParameters->naam = std::string(tempNaam);
  invoerBestand.getline(tempDatum, 256);
  competitieParameters->startDatum = std::string(tempDatum);
  invoerBestand >> competitieParameters->hoogsteEigenwaarde;
  invoerBestand >> competitieParameters->aantalRondes;
  
  invoerBestand.close();
  
  return competitieParameters;
}



void IOServices::readPlayers(PlayerList* playerList)
{
  std::ifstream invoerBestand;
  int nrSpelers = 0;
  Player* player = NULL;
  char tempNaam[256];
  
  invoerBestand.exceptions(std::ifstream::failbit | std::ifstream::badbit);  // Treat everything except EOF as an exception.
  invoerBestand.open("input/Spelers.txt", std::ios::in);
  
  invoerBestand >> nrSpelers;
  assert(nrSpelers > 0);
  
  for(int s = 0; s < nrSpelers; s++)
  {
    player = new Player();
    invoerBestand >> player->id;   // TODO: remove this, since player IDs are automatically assigned in order of addition
    // Eet spaties alvorens getline te doen
    while(invoerBestand.peek() == ' ') { invoerBestand.get(); }
    invoerBestand.getline(tempNaam, 256);
    player->name = std::string(tempNaam);
    playerList->addPlayer(player);
  }
  
  invoerBestand.close();
}



Round* IOServices::readRound(unsigned int r)
{
	Round* round = new Round();
	std::ostringstream bestandsnaam;
	std::ifstream invoerBestand;
	char datumBuffer[256];
	int nrUitslagen = 0;
	Game* game = NULL;
	int intResult;
	
	invoerBestand.exceptions(std::ifstream::failbit | std::ifstream::badbit);  // Treat everything except EOF as an exception.
	bestandsnaam << "input/Uitslag" << r << ".txt";
	
	invoerBestand.open(bestandsnaam.str().c_str(), std::ios::in);
	invoerBestand.getline(datumBuffer, 256);
	round->date = datumBuffer;
	
	invoerBestand >> nrUitslagen;
	for(int u = 0; u < nrUitslagen; u++)
	{
		game = new Game();
		invoerBestand >> intResult;
		game->result = static_cast<enum GameResult>(intResult);
		invoerBestand >> game->idWhite;
		if(game->result == WHITE_WINS || game->result == BLACK_WINS || game->result == DRAW)
		{
			// Betekent dat er een zwartspeler geweest moet zijn.
			invoerBestand >> game->idBlack;
		}
		round->games.push_back(game);
	}
	
	invoerBestand.close();
	
	return round;
}



void IOServices::writeRankingDocument(Competition* competition, unsigned int round)
{
	std::ostringstream bestandsnaam;
	std::auto_ptr<CompetitieParameters> competitieParameters;
	std::string competitieTitel;
	std::ofstream document;
	std::auto_ptr<Ranking> ranking;
	std::auto_ptr<Round> uitslagAfgelopenRonde;
	std::string datumAfgelopenRonde;
	std::auto_ptr<PlayerList> playerList;

	// Sanity checks
	if(competition == NULL)
		throw std::invalid_argument("Competitie is NULL");
	if(round > competition->getNrRounds())
		throw std::invalid_argument("Gevraagde ronde is nog niet gespeeld.");
	
	// Set up error handling
	document.exceptions(std::ofstream::failbit | std::ofstream::badbit);  // Treat everything except EOF as an exception.
	
	// Gather some data
	competitieParameters = competition->getCompetitieParameters();
	if(competitieParameters->naam.empty())
		competitieTitel = "Naamloze Competitie";
	else
		competitieTitel = competitieParameters->naam;
		
	ranking = competition->getRankingAfterRound(round);
	
	if(round > 0)
	{
		uitslagAfgelopenRonde = competition->getRound(round);
		datumAfgelopenRonde = uitslagAfgelopenRonde->date;
	}
	else
	{
		datumAfgelopenRonde = competitieParameters->startDatum;
	}
	
	playerList = competition->getPlayerList();

	// Prepare output
	bestandsnaam << "output/Ranglijst na ronde " << std::setw(4) << std::setfill('0') << round << ".html";
	document.open(bestandsnaam.str().c_str(), std::ios::out);
	
	document << "<html>\n<head>\n<title>Ranglijst na ronde " << round << "</title>\n"
	         << "<style type=\"text/css\">\n"
	         << "table {border: dotted; border-width: 1}\n"
	         << "td {border: dotted; border-width: 1}\n"
	         << "</style>\n"
	         << "</head>\n"
	         << "<body>\n<h1>" << competitieTitel << "</h1>\n"
	         << "<h2>Ranglijst na ronde " << round << ", gespeeld op " << datumAfgelopenRonde << "</h2>\n"
	         << "<table>\n"
	         << "<tr><th>Plaats</th> <th>Naam speler</th> <th>Keizerpunten</th> <th>Eigenwaarde</th> "
	         << "<th>Aantal gespeelde partijen</th> <th>Aantal gewonnen</th> <th>Aantal remises</th> "
	         << "<th>Aantal verloren</th> <th>Aantal vrije rondes</th> <th>Wedstrijdpunten</th> "
	         << "<th>Wit</th> <th>Zwart</th></tr>\n"
	         ;
	
	for(unsigned int p = 1; p <= ranking->getLength(); p++)
	{
		RankingItem* rankingItem = ranking->getItemAtPlace(p);
		Player* player = playerList->getPlayerById(rankingItem->playerId);
		document << "<tr><td style=\"text-align:right\">" << p << "</td> <td>" << player->name << "</td> "
		         << "<td>" << rankingItem->points << "</td> <td>" << rankingItem->eigenvalue << "</td> "
		         << "<td>" << rankingItem->nrGamesPlayed << "</td> <td>" << rankingItem->nrGamesWon << "</td> "
		         << "<td>" << rankingItem->nrGamesDrawn << "</td> <td>" << rankingItem->nrGamesLost << "</td>"
		         << "<td>" << rankingItem->nrRoundsFree << "</td> <td>" << rankingItem->score << "</td>"
		         << "<td>" << rankingItem->nrGamesWhite << "</td> <td>" << rankingItem->nrGamesBlack << "</td>"
		         << "</tr>\n";
	}

	document << "</table>\n";
	
	// Is er al minstens 1 ronde gespeeld?
	if(round > 0)
	{
		document << "<h2>Uitslagen ronde " << round << "</h2>\n"
		         << "<table>\n"
		         << "<tr><th>Wit</th> <th>Zwart</th> <th>Uitslag</th></tr>\n";

		for(unsigned int p = 0; p < uitslagAfgelopenRonde->games.size(); p++)
		{
			std::string uitslagTekst;
			Game* partij = uitslagAfgelopenRonde->games.at(p);
			std::string naamWit = playerList->getPlayerById(partij->idWhite)->name;
			std::string naamZwart;
			if(partij->idBlack >= 0)
				naamZwart = playerList->getPlayerById(partij->idBlack)->name;
			
			switch(partij->result)
			{
				case BLACK_WINS:
					uitslagTekst = "0 - 1";
					break;
				case WHITE_WINS:
					uitslagTekst = "1 - 0";
					break;
				case DRAW:
					uitslagTekst = "1/2 - 1/2";
					break;
				case FREE:
					uitslagTekst = "vrij";
					break;
			}
			document << "<tr><td>" << naamWit << "</td> <td>" << naamZwart << "</td> <td>" << uitslagTekst << "</td>\n";
		}
		
		document << "</table>\n";
	}

	document << "</body>\n"
	         << "</html>";
	
	document.close();
}



void IOServices::writeCrossTable(Competition* competition, unsigned int round)
{
	std::ostringstream bestandsnaam;
	std::ofstream document;
	std::string competitieTitel;
	std::auto_ptr<PlayerList> spelerslijst = competition->getPlayerList();
	std::auto_ptr<CompetitieParameters> competitieParameters = competition->getCompetitieParameters();
	unsigned int aantalSpelers = spelerslijst->getNrPlayers();
	
	// Set up error handling.
	document.exceptions(std::ofstream::failbit | std::ofstream::badbit);  // Treat everything except EOF as an exception.
	
	if(competitieParameters->naam.empty())
		competitieTitel = "Naamloze Competitie";
	else
		competitieTitel = competitieParameters->naam;
	
	// Kruistabel voorbereiden
	float** kruistabel = new float*[aantalSpelers + 1];  // Kruistabel[s][t] is resultaat van speler s behaald tegen speler t.
	for(unsigned int s = 0; s < aantalSpelers + 1; s++)	 // laatste index is voor vrije rondes, vandaar arraylengte aantalSpelers+1
	{
		kruistabel[s] = new float[aantalSpelers + 1];
		for(unsigned int t = 0; t < aantalSpelers + 1; t++)
			kruistabel[s][t] = 0;
	}
	
	// Kruistabel vullen
	for(unsigned int r = 0; r < round; r++)
	{
		std::auto_ptr<Round> uitslagDezeRonde = competition->getRound(r);
		for(unsigned int p = 0; p < uitslagDezeRonde->games.size(); p++)
		{
			Game* partij = uitslagDezeRonde->games[p];
			if(partij->idWhite < 0 || static_cast<unsigned int>(partij->idWhite) >= aantalSpelers)
				throw std::logic_error("Ongeldig speler-ID voor witspeler gevonden bij vullen van kruistabel.");
			switch(partij->result)
			{
				case WHITE_WINS:
					if(partij->idBlack < 0 || static_cast<unsigned int>(partij->idBlack) >= aantalSpelers)
						throw std::logic_error("Ongeldig speler-ID voor zwartspeler gevonden bij vullen van kruistabel.");
					kruistabel[partij->idWhite][partij->idBlack] += 1;
					break;
				case BLACK_WINS:
					if(partij->idBlack < 0 || static_cast<unsigned int>(partij->idBlack) >= aantalSpelers)
						throw std::logic_error("Ongeldig speler-ID voor zwartspeler gevonden bij vullen van kruistabel.");
					kruistabel[partij->idBlack][partij->idWhite] += 1;
					break;
				case DRAW:
					if(partij->idBlack < 0 || static_cast<unsigned int>(partij->idBlack) >= aantalSpelers)
						throw std::logic_error("Ongeldig speler-ID voor zwartspeler gevonden bij vullen van kruistabel.");
					kruistabel[partij->idWhite][partij->idBlack] += 0.5;
					kruistabel[partij->idBlack][partij->idWhite] += 0.5;
					break;
				case FREE:
					kruistabel[partij->idWhite][aantalSpelers] += 1;
					break;
				default:
					std::cerr << "Onbekend resultaat in uitslagen gevonden bij vullen van kruistabel!" << std::endl;
					assert(partij->result >= 0 && partij->result <= 3);
					break;
			}
		}
	}
	
	// Kruistabel uitvoeren
	bestandsnaam << "output/Kruistabel na ronde " << std::setw(4) << std::setfill('0') << round << ".html";
	document.open(bestandsnaam.str().c_str(), std::ios::out);
	
	document << "<html>\n<head>\n<title>Kruistabel na ronde " << round << "</title>\n"
	         << "<style type=\"text/css\">\n"
	         << "table {border: dotted; border-width: 1}\n"
	         << "td {border: dotted; border-width: 1}\n"
	         << "div.naamTegenstander { -webkit-transform:rotate(90deg); -moz-transform:rotate(90deg);"
	         << "	-o-transform: rotate(90deg); display:block }\n"
	         << "td.blackout { background-color: black }\n"
	         << "td.score { text-align: center}\n"
	         << "</style>\n"
	         << "</head>\n"
	         << "<body>\n<h1>" << competitieTitel << "</h1>\n"
	         << "<h2>Kruistabel na ronde " << round << ", gespeeld op "
	         << competition->getRound(round)->date << "</h2>\n"
	         << "<table>\n"
	         << "<tr>\n"
	         << "  <th rowspan=\"2\" style=\"vertical-align:bottom\">Naam speler</td>\n"
	         << "  <th colspan=\"" << aantalSpelers + 1 << "\" style=\"text-align:center\">Naam tegenstander</th>\n"
	         << "</tr>\n";

	document << "<tr style=\"height:100px\">\n";
	for(unsigned int s = 0; s < aantalSpelers; s++)
	{
		document << "  <th><div class=\"naamTegenstander\">" << spelerslijst->getPlayerById(s)->name << "</div></th>\n";
	}
	document << "<th><div class=\"naamTegenstander\">Vrije ronde</div></th>\n"
           << "</tr>\n";
		
	for(unsigned int s = 0; s < aantalSpelers; s++)
	{
		document << "<tr><td>" << spelerslijst->getPlayerById(s)->name << "</td>";
		for(unsigned int t = 0; t < aantalSpelers + 1; t++)
		{
			if(s == t)
				document << "<td class=\"blackout\"></td>";
			else
				document << "<td class=\"score\">" << kruistabel[s][t] << "</td>";
		}
		document << "</tr>\n";
	}
	document << "<tr><td>Vrije ronde</td>";
	for(unsigned int t = 0; t < aantalSpelers; t++)
		document << "<td class=\"score\">" << kruistabel[aantalSpelers][t] << "</td>";
	document << "<td class=\"blackout\"></td></tr>\n";
	
	document << "</table>\n"
	         << "</body>\n"
	         << "</html>";

	// Clean-up
	for(unsigned int s = 0; s < aantalSpelers + 1; s++)
	{
		delete kruistabel[s];
		kruistabel[s] = NULL;
	}
	delete kruistabel;
	kruistabel = NULL;

	document.close();
}


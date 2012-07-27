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

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <assert.h>
#include <math.h>

#include "Competition.h"
#include "CompetitieParameters.h"
#include "Game.h"
#include "Ranking.h"
#include "Player.h"
#include "PlayerList.h"

using namespace std;



int main(/* int argc, char** argv */)
{
	cout << "FreeKeizer 0.5   Copyright 2012 Roderick de Jong" << endl
	     << "This program comes with ABSOLUTELY NO WARRANTY." << endl
	     << "This is free software, and you are welcome to redistribute it under certain conditions." << endl
		 << "For details, please view the license contained in 'gpl.txt'." << endl << endl;
	
	try
	{
		Competitie competitie;
		competitie.setLogStream(&cout);
		competitie.leesCompetitieDatabase();
		
		unsigned int aantalRondes = competitie.getNrRondes();
		
		//auto_ptr<Spelerslijst> spelerslijst = competitie.getSpelerslijst();
		//auto_ptr<Ranglijst> ranglijst = competitie.getRanglijstNaRonde(aantalRondes);
		//Spelerslijst* pSpelerslijst = spelerslijst.release();
		//ranglijst->dump(pSpelerslijst, &cout);
		//delete pSpelerslijst;
		
		competitie.writeRanglijstDocument(aantalRondes);
		
		competitie.nieuweRonde();
		aantalRondes++;
		competitie.maakIndeling();
		
		// Toon de nieuwe indeling
		cout << "--- Indeling ronde " << aantalRondes << " ---" << endl;
		auto_ptr<Ronde> indeling = competitie.getRonde(aantalRondes); // vraag de nieuwe ronde op
		auto_ptr<Spelerslijst> spelerslijst = competitie.getSpelerslijst();
		Spelerslijst* pSpelerslijst = spelerslijst.release();
		indeling->dump(pSpelerslijst, &cout);
		delete pSpelerslijst; pSpelerslijst = NULL;
		cout << "-------------------------" << endl;
		
		// Roadmap:
		// - Unittests voor class Ronde
		// - class IndelingMaker met GeneticAlgorithm
		// - competitiedatabase ontwerpen i.p.v. plain text files gebruiken
	}
	catch(std::exception& e)
	{
		cerr << "FreeKeizer encountered an unexpected exception. Please check the competition database for any problems, or report this as a bug." << endl
		     << "Exception details: " << e.what() << endl;
		return 1;
	}
	
	return 0;
}




// GA ideas:
// - Generate random population
// - Report after each iteration:
//   - Current genotype top 3 and their fitness values
//   - Current top 3 valid genotypes and their fitness values
//   - Top genotype so far (doesn't need to be alive anymore) and its fitness
//   - Top valid genotype so far (doesn't need to be alive anymore) and its fitness
//   - Current population's min/max/average fitness, std.dev., and their deltas.
// - Repeat N times, then ask the user whether to continue for another N iterations or not.

// Fitness function ideas:
// 1) Classify pairing history:
//    - 'good' = 1 (haven't played each other yet)
//    - 'decent' = 0.75 (haven't met during the past 5 rounds)
//    - 'bad' = 0.1 - 0.5 (have played each other x rounds ago where x in [1;5]. Resulting eval value: x / 10)
// 2) Classify pairing quality:
//    - use inverse-ranking-distance, as a percentage of the number of players in the competition
// 3) Calculated weighted average of pairing history and pairing quality.
// 4) Penalize invalid solutions:
//    - for pairings that duplicate players, reduce their fitness contribution by 2/3rd (to 1/3rd)
// 5) Sum up the values of all pairings to obtain the genotype's fitness value.



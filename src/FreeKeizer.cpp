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

#include "Competitie.h"
#include "CompetitieParameters.h"
#include "Partij.h"
#include "Ranglijst.h"
#include "Speler.h"
#include "Spelerslijst.h"

using namespace std;



int main(/* int argc, char** argv */)
{
	cout << "FreeKeizer 0.5" << endl;
	
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



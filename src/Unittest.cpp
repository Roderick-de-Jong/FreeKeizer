#include <string>
#include <vector>
#include <assert.h>

#include "Unittest.h"
#include "Partij.h"
#include "Ranglijst.h"
#include "Ronde.h"
#include "Spelerslijst.h"


using namespace std;



int FreeKeizerUnittest::testSpelerslijst()
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
	Spelerslijst* spelerslijst = NULL;
	try
	{
		spelerslijst = new Spelerslijst();
		for(unsigned int i = 0; i < namedata.size(); i++)
		{
			Speler* speler = new Speler();
			speler->naam = namedata.at(i);
			spelerslijst->voegSpelerToe(speler);
			if(speler->id != i)
			{
				cout << "FAILED" << endl
					<< "\tSpeler " << i  << " was assigned id " << speler->id << " which is not equal." << endl;
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
	unsigned int aantalSpelers = 0;
	try
	{
		aantalSpelers = spelerslijst->getNrSpelers();
		if(aantalSpelers != namedata.size())
		{
			cout << "FAILED" << endl
				<< "\tPlayer count incorrect; got " << aantalSpelers << " instead of the expected " << namedata.size() << "." << endl;
			fails++;
		}
		for(unsigned int i = 0; i < aantalSpelers; i++)
		{
			Speler* speler = spelerslijst->getSpelerById(i);
			if(speler->id != i)
			{
				cout << "FAILED" << endl
					<< "\tPlayer retrieval failed. Found id " << speler->id  << " instead of " << i << "." << endl;
				fails++;
			}
			if(speler->naam != namedata.at(i))
			{
				cout << "FAILED" << endl
					<< "\tPlayer retrieval failed. Found name " << speler->naam  << " instead of " << namedata.at(i) << "." << endl;
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
		for(unsigned int i = 0; i < aantalSpelers; i++)
		{
			string naam = namedata.at(i);
			Speler* speler = spelerslijst->getSpelerByNaam(naam);
			if(speler->naam != naam)
			{
				cout << "FAILED" << endl
					<< "\tPlayer retrieval failed. Found name " << speler->naam  << " instead of " << naam << "." << endl;
				fails++;
			}
			if(speler->id != i)
			{
				cout << "FAILED" << endl
					<< "\tPlayer retrieval failed. Found id " << speler->id  << " instead of " << i << "." << endl;
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
	Spelerslijst* lijstTwee = NULL;
	try
	{
		lijstTwee = new Spelerslijst(*spelerslijst);
		for(unsigned int i = 0; i < aantalSpelers; i++)
		{
			Speler* spelerA = spelerslijst->getSpelerById(i);
			Speler* spelerB = lijstTwee->getSpelerById(i);
			if(spelerA == spelerB)
			{
				cout << "FAILED" << endl
					<< "\tFailed to make deep copy; pointers are identical (" << spelerA << " and " << spelerB << ")." << endl;
				fails++;
			}
			if(spelerA->id != spelerB->id)
			{
				cout << "FAILED" << endl
					<< "\tCopy error: player ID mismatch (found " << spelerB->id << ", original was " << spelerA->id << ")." << endl;
				fails++;
			}
			if(spelerA->naam != spelerB->naam)
			{
				cout << "FAILED" << endl
					<< "\tCopy error: player name mismatch (found " << spelerB->naam << ", original was " << spelerA->naam << ")." << endl;
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
		spelerslijst->_verwijderAlleSpelers();
		if(spelerslijst->getNrSpelers() != 0)
		{
			cout << "FAILED" << endl
					<< "\tList isn't zero-length yet." << endl;
			fails++;
		}
		if(spelerslijst->_spelers.size() != 0)
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
		*spelerslijst = *lijstTwee;
		for(unsigned int i = 0; i < aantalSpelers; i++)
		{
			Speler* spelerA = spelerslijst->getSpelerById(i);
			Speler* spelerB = lijstTwee->getSpelerById(i);
			if(spelerA == spelerB)
			{
				cout << "FAILED" << endl
					<< "\tFailed to make deep copy; pointers are identical (" << spelerA << " and " << spelerB << ")." << endl;
				fails++;
			}
			if(spelerA->id != spelerB->id)
			{
				cout << "FAILED" << endl
					<< "\tCopy error: player ID mismatch (found " << spelerA->id << ", original was " << spelerB->id << ")." << endl;
				fails++;
			}
			if(spelerA->naam != spelerB->naam)
			{
				cout << "FAILED" << endl
					<< "\tCopy error: player name mismatch (found " << spelerA->naam << ", original was " << spelerB->naam << ")." << endl;
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
		delete spelerslijst;
		spelerslijst = NULL;
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
		delete lijstTwee;
		lijstTwee = NULL;
	}
	catch(std::exception& e)
	{
		cout << "EXCEPTION: " << e.what() << endl;
		fails++;
	}
	cout << "DONE" << endl;
	
	return fails;
}



int FreeKeizerUnittest::testRanglijst()
{
	int fails = 0;
	const char* rawNamedata[] = {"Jan", "Marjan", "Pia", "Piet", "Klaas", "Jan-Peter", "Victor", "Rudolf", "Eric", "Kees", "Wouter", "Jolande", "Annemarie"};
	unsigned int nrNames = 13;
	vector<string> namedata;
	Spelerslijst* spelerslijst = NULL;
	
	cout << endl << "----- Unittest for RANGLIJST module -----" << endl;
	
	// Prepare testing data
	namedata.reserve(nrNames);
	spelerslijst = new Spelerslijst();
	for(unsigned int n = 0; n < nrNames; n++)
	{
		namedata.push_back(rawNamedata[n]);
		Speler* speler = new Speler(namedata.at(n));
		spelerslijst->voegSpelerToe(speler);
	}
	assert(spelerslijst->getNrSpelers() == nrNames);
	
	cout << "Testing maakStartRanglijst() and destructor..."; cout.flush();
	Ranglijst* ranglijst = NULL;
	try
	{
		for(unsigned int hoogsteEigenwaarde = nrNames; hoogsteEigenwaarde < nrNames + 100; hoogsteEigenwaarde++)
		{
			ranglijst = Ranglijst::maakStartRanglijst(hoogsteEigenwaarde, spelerslijst);
			for(unsigned int plaats = 1; plaats <= nrNames; plaats++) // Plaatsen beginnen bij 1
			{
				RanglijstItem* ranglijstItem = ranglijst->getItemOpPlaats(plaats);
				if(ranglijstItem->spelerId != plaats - 1)
				{
					cout << "FAILED" << endl
					     << "\tPlayer ID mismatch (found " << ranglijstItem->spelerId << ", expected " << plaats - 1 << ")." << endl;
					fails++;
				}
				if(static_cast<unsigned int>(ranglijstItem->eigenwaarde) != hoogsteEigenwaarde + 1 - plaats)   // +1 omdat plaatsen beginnen bij 1
				{
					cout << "FAILED" << endl
					     << "\tIncorrect eigenvalue (found " << ranglijstItem->eigenwaarde << ", expected " << hoogsteEigenwaarde + 1 - plaats
					     << " for place " << plaats << " and max eigenvalue " << hoogsteEigenwaarde << ")." << endl;
					fails++;
				}
				if(ranglijstItem->punten != ranglijstItem->eigenwaarde)
				{
					cout << "FAILED" << endl
					     << "\tPoints (" << ranglijstItem->punten << ") do not equal eigenvalue (" << ranglijstItem->eigenwaarde << ")." << endl;
					fails++;
				}
			}
			delete ranglijst;
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
		unsigned int hoogsteEigenwaarde = 2 * nrNames;
		ranglijst = Ranglijst::maakStartRanglijst(hoogsteEigenwaarde, spelerslijst);
		delete ranglijst;
		ranglijst = Ranglijst::maakStartRanglijst(hoogsteEigenwaarde, spelerslijst);
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
		Ranglijst* ranglijstCopies[2];
		ranglijstCopies[0] = new Ranglijst(*ranglijst);  // Copy contructor
		ranglijstCopies[1] = new Ranglijst();
		*ranglijstCopies[1] = *ranglijst;  // Assignment operator
		for(int r = 0; r < 2; r++)
		{
			if(ranglijstCopies[r]->getLengte() != ranglijst->getLengte())
			{
				cout << "FAILED" << endl
				     << "\tList length mismatch (found " << ranglijstCopies[r]->getLengte() << ", expected " << ranglijst->getLengte() << ")." << endl;
				fails++;
			}
			for(unsigned int plaats = 1; plaats <= ranglijst->getLengte(); plaats++)
			{
				RanglijstItem* originalItem = ranglijst->getItemOpPlaats(plaats);
				RanglijstItem* clonedItem = ranglijstCopies[r]->getItemOpPlaats(plaats);
				if(originalItem == clonedItem)
				{
					cout << "FAILED" << endl
						 << "\tFailed to make deep copy; pointers are identical (" << originalItem << " and " << clonedItem << ")." << endl;
				}
				if(originalItem->spelerId != clonedItem->spelerId)
				{
					cout << "FAILED" << endl
					     << "\tPlayer ID mismatch (found " << clonedItem->spelerId << ", expected " << originalItem->spelerId << ")." << endl;
					fails++;
				}
				if(originalItem->punten != clonedItem->punten)
				{
					cout << "FAILED" << endl
					     << "\tPlayer points mismatch (found " << clonedItem->punten << ", expected " << originalItem->punten << ")." << endl;
					fails++;
				}
				if(originalItem->eigenwaarde != clonedItem->eigenwaarde)
				{
					cout << "FAILED" << endl
					     << "\tPlayer eigenvalue mismatch (found " << clonedItem->eigenwaarde << ", expected " << originalItem->eigenwaarde << ")." << endl;
					fails++;
				}
			}
			delete ranglijstCopies[r];
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
		Ranglijst* lijstTwee = new Ranglijst(*ranglijst);
		int swapDistance[] = {5,  7,  7, 1, 0,
		                      4, 12, 11, 6, 3,
							  8,  0,  8, 3, 7,
							  5,  8}; // Items are moved this far.
		int nrSwapDistances = 17;
		// Do some non-random permutations on the copy
		for(unsigned int i = 0; i < lijstTwee->getLengte(); i++)
		{
			RanglijstItem* itemAtI = lijstTwee->getItemOpPlaats(i + 1); // Plaats param is base 1
			int j = (i + swapDistance[i % nrSwapDistances]) % (lijstTwee->getLengte());
			RanglijstItem* itemAtJ = lijstTwee->getItemOpPlaats(j + 1); // Idem
			//cout << "Swapping items at " << i + 1 << " and " << j + 1 << "." << endl;
			lijstTwee->_items[i] = itemAtJ;
			lijstTwee->_items[j] = itemAtI;
		}
		// Test if sorting results in the original list
		lijstTwee->sorteer();
		if(lijstTwee->getLengte() != ranglijst->getLengte())
		{
			cout << "FAILED" << endl
			     << "\tList length mismatch (found " << lijstTwee->getLengte() << ", expected " << ranglijst->getLengte() << ")." << endl;
			fails++;
		}
		for(unsigned int plaats = 1; plaats <= ranglijst->getLengte(); plaats++)
		{
			RanglijstItem* originalItem = ranglijst->getItemOpPlaats(plaats);
			RanglijstItem* clonedItem = lijstTwee->getItemOpPlaats(plaats);
			if(originalItem->spelerId != clonedItem->spelerId)
			{
				cout << "FAILED" << endl
						<< "\tPlayer ID mismatch (found " << clonedItem->spelerId << ", expected " << originalItem->spelerId << ")." << endl;
				fails++;
			}
			if(originalItem->punten != clonedItem->punten)
			{
				cout << "FAILED" << endl
						<< "\tPlayer points mismatch (found " << clonedItem->punten << ", expected " << originalItem->punten << ")." << endl;
				fails++;
			}
			if(originalItem->eigenwaarde != clonedItem->eigenwaarde)
			{
				cout << "FAILED" << endl
						<< "\tPlayer eigenvalue mismatch (found " << clonedItem->eigenwaarde << ", expected " << originalItem->eigenwaarde << ")." << endl;
				fails++;
			}
		}
		delete lijstTwee;
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
	fails += unittest.testSpelerslijst();
	fails += unittest.testRanglijst();
	
	cout << endl << "TOTAL FAILS: " << fails << endl;
}

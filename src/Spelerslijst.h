#ifndef SPELERSLIJST_H
#define SPELERSLIJST_H

#include <iostream>
#include <vector>

#include "Speler.h"
#include "Unittest.h"



// TODO: IMPORTANT: add a (dummy) player 0 to the player database, now that player IDs start at 0!!!!!

class Spelerslijst
{
	friend class FreeKeizerUnittest;
	
	public:
	
	/**
	 * Default constructor.
	 */
	Spelerslijst();
	
	/**
	 * Copy constructor.
	 */
	Spelerslijst(Spelerslijst& original);
	
	/**
	 * Destructor.
	 */
	virtual ~Spelerslijst();
	
	/**
	 * Assignment operator.
	 */
	Spelerslijst& operator=(const Spelerslijst& original);
	
	virtual Speler* getSpelerById(unsigned int spelerId);
	
	/**
	 * Returns:
	 * De speler met de naam in kwestie, of NULL indien niet gevonden.
	 */
	virtual Speler* getSpelerByNaam(const std::string& spelerNaam);

	virtual unsigned int getNrSpelers();
	
	virtual void voegSpelerToe(Speler* speler);
	
	/**
	 * Geeft de spelerslijst weer op de gegeven outputstream.
	 */
	virtual void dump(std::ostream* outputStream);
	
	protected:
	
	virtual void _verwijderAlleSpelers();
	
	std::vector<Speler*> _spelers;
};

#endif // SPELERSLIJST_H


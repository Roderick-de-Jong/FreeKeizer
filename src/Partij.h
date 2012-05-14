#ifndef PARTIJ_H
#define PARTIJ_H

#include "SchaakConstantes.h"

/**
 * Representatie van de uitslag van één individuele schaakpartij.
 */
class Partij
{
	public:
	Partij();
	virtual ~Partij();
	
	/**
	* Het unieke ID van de witspeler.
	*/
	int idWit;
	
	/**
	* Het unieke ID van de zwartspeler, of -1 bij geen zwartspeler, bijv. bij een vrije ronde.
	*/
	int idZwart;
	
	/**
	* Code die aangeeft wat de uitslag van de partij was.
	*/
	enum PartijResultaat resultaat;
};

#endif // PARTIJ_H


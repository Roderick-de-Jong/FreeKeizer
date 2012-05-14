#ifndef RONDE_H
#define RONDE_H

#include <iostream>
#include <string>
#include <vector>

#include "Partij.h"
#include "Spelerslijst.h"

/**
 * Representeert de nog te spelen of reeds gespeelde partijen in een ronde van de competitie.
 * Kan worden gebruikt voor zowel het registreren en opslaan van uitslagen als het maken
 * van een indeling voor een ronde.
 */
class Ronde
{
	public:
	Ronde();
	virtual ~Ronde();
	virtual void dump(Spelerslijst* spelerslijst, std::ostream* outputStream);

	std::string datum;
	std::vector<Partij*> partijen;
};

#endif // RONDE_H


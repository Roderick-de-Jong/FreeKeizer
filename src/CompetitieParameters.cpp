#include <iostream>

#include "CompetitieParameters.h"

CompetitieParameters::CompetitieParameters()
{
}

CompetitieParameters::~CompetitieParameters()
{
}

CompetitieParameters::CompetitieParameters(CompetitieParameters& original)
{
	naam = original.naam;
	startDatum = original.startDatum;
	hoogsteEigenwaarde = original.hoogsteEigenwaarde;
	aantalRondes = original.aantalRondes;
}

CompetitieParameters& CompetitieParameters::operator=(const CompetitieParameters& original)
{
	if(this != &original)
	{
		naam = original.naam;
		startDatum = original.startDatum;
		hoogsteEigenwaarde = original.hoogsteEigenwaarde;
		aantalRondes = original.aantalRondes;
	}
	
	return *this;
}

void CompetitieParameters::dump(std::ostream* outputStream)
{
  (*outputStream) << "CompetitieParameters:" << std::endl
       << "Naam: \"" << naam << "\"" << std::endl
       << "Startdatum: \"" << startDatum << "\"" << std::endl
       << "Hoogste eigenwaarde: " << hoogsteEigenwaarde << std::endl
       << "Aantal rondes: " << aantalRondes << std::endl;
}


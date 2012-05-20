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


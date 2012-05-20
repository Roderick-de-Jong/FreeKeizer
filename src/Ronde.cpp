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

#include "Ronde.h"
#include "Spelerslijst.h"

Ronde::Ronde()
{
}

Ronde::~Ronde()
{
  for(unsigned int u = 0; u < partijen.size(); u++)
  {
    delete partijen[u];
    partijen[u] = NULL;
  }
  partijen.clear();
}

void Ronde::dump(Spelerslijst* spelerslijst, std::ostream* outputStream)
{
  Partij* partij = NULL;
  Speler* witSpeler = NULL;
  Speler* zwartSpeler = NULL;
  
  if(outputStream != NULL)
  {  
	for(unsigned int p = 0; p < partijen.size(); p++)
	{
		partij = partijen[p];
		(*outputStream) << partij->resultaat << " " << partij->idWit << " " << partij->idZwart << " ---> ";
			witSpeler = spelerslijst->getSpelerById(partij->idWit);
			zwartSpeler = NULL;
			if(partij->idZwart >= 0)
				zwartSpeler = spelerslijst->getSpelerById(partij->idZwart);
		switch(partij->resultaat)
		{
		case WIT_WINT:
			(*outputStream) << witSpeler->naam << " - " << zwartSpeler->naam << " 1 - 0" << std::endl;
			break;
		case ZWART_WINT:
			(*outputStream) << witSpeler->naam << " - " << zwartSpeler->naam << " 0 - 1" << std::endl;
			break;
		case REMISE:
			(*outputStream) << witSpeler->naam << " - " << zwartSpeler->naam << " 1/2 - 1/2" << std::endl;
			break;
		case VRIJE_RONDE:
			(*outputStream) << witSpeler->naam  << " vrij" << std::endl;
			break;
		default:
			(*outputStream) << "Ongedefiniëerd resultaat." << std::endl;
			break;
		}
	}
  }
}

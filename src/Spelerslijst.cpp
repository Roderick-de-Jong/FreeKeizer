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

#include <stdexcept>

#include "Spelerslijst.h"

Spelerslijst::Spelerslijst()
{
}

Spelerslijst::Spelerslijst(Spelerslijst& original)
{
	_spelers.resize(original._spelers.size());
	for(unsigned int s = 0; s < original._spelers.size(); s++)
		_spelers.at(s) = new Speler(*original._spelers.at(s));
}

Spelerslijst::~Spelerslijst()
{
	_verwijderAlleSpelers();
}

Spelerslijst& Spelerslijst::operator=(const Spelerslijst& original)
{
	if(this != &original)
	{
		_verwijderAlleSpelers();
		_spelers.resize(original._spelers.size());
		for(unsigned int s = 0; s < original._spelers.size(); s++)
			_spelers.at(s) = new Speler(*original._spelers.at(s));
	}
	
	return *this;
}

Speler* Spelerslijst::getSpelerById(unsigned int spelerId)
{
	return _spelers.at(spelerId);
}

Speler* Spelerslijst::getSpelerByNaam(const std::string& spelerNaam)
{
	for(unsigned int s = 0; s < _spelers.size(); s++)
		if(_spelers[s]->naam == spelerNaam)
			return _spelers[s];
	return NULL;
}

unsigned int Spelerslijst::getNrSpelers()
{
	return _spelers.size();
}

void Spelerslijst::voegSpelerToe(Speler* speler)
{
	_spelers.push_back(speler);
	speler->id = _spelers.size() - 1;
}

void Spelerslijst::dump(std::ostream* outputStream)
{
  Speler* speler = NULL;
  
  if(outputStream != NULL)
  {
	(*outputStream) << "Spelerslijst:" << std::endl;
	for(unsigned int s = 0; s < _spelers.size(); s++)
	{
		speler = _spelers.at(s);
		(*outputStream) << speler->id << " " << speler->naam << std::endl;
	}
  }
}

void Spelerslijst::_verwijderAlleSpelers()
{
	for(unsigned int s = 0; s < _spelers.size(); s++)
	{
		if(_spelers[s] != NULL)
		{
			delete _spelers[s];
			_spelers[s] = NULL;
		}
	}
	_spelers.clear();
}


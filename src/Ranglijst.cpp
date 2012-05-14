#include <algorithm>
#include <stdexcept>
#include <sstream>

#include "Ranglijst.h"
#include "RanglijstItem.h"

Ranglijst::Ranglijst()
{
}

Ranglijst::~Ranglijst()
{
	_verwijderAlleItems();
}



Ranglijst::Ranglijst(Ranglijst& original)
{
	_items.resize(original._items.size());
	for(unsigned int i = 0; i < original._items.size(); i++)
		_items[i] = new RanglijstItem(*original._items[i]);
}



Ranglijst& Ranglijst::operator=(const Ranglijst& original)
{
	if(this != &original)
	{
		_verwijderAlleItems();
		_items.resize(original._items.size());
		for(unsigned int i = 0; i < original._items.size(); i++)
			_items[i] = new RanglijstItem(*original._items[i]);
	}
	
	return *this;
}



unsigned int Ranglijst::getLengte()
{
	return _items.size();
}



RanglijstItem* Ranglijst::getItemOpPlaats(unsigned int plaats)
{
	if(plaats == 0)
		throw std::invalid_argument("Argument 'plaats' is 0, maar de ranglijst begint bij plaats 1.");
	if(plaats > _items.size())
		throw std::invalid_argument("Argument 'plaats' is groter dan het aantal spelers op de ranglijst.");

	return _items.at(plaats - 1); // -1 omdat de eerste plek op de ranglijst in element 0 wordt opgeslagen
}



RanglijstItem* Ranglijst::getRanglijstItemBySpelerId(unsigned int spelerId)
{
  RanglijstItem* ranglijstItem = NULL;
  for(unsigned int r = 0; r < _items.size(); r++)
  {
    ranglijstItem = _items.at(r);
    if(ranglijstItem->spelerId == spelerId)
      return ranglijstItem;
  }

	std::ostringstream message;
	message << "Speler ID " << spelerId << " niet gevonden in ranglijst.";
	throw std::invalid_argument(message.str());
}



void Ranglijst::voegItemToe(RanglijstItem* item)
{
	_items.push_back(item);
}



void Ranglijst::sorteer()
{
	// Zoek hoogste eigenwaarde
	int hoogsteEigenwaarde = -1;
	for(unsigned int p = 0; p < _items.size(); p++)
		if(_items[p]->eigenwaarde > hoogsteEigenwaarde)
			hoogsteEigenwaarde = _items[p]->eigenwaarde;

	// Sorteer lijst op keizerpunten
	sort(_items.begin(), _items.end(), compareRanglijstItems);
	
	// Pas eigenwaardes aan
	for(unsigned int s = 0; s < _items.size(); s++)
	{
		_items.at(s)->eigenwaarde = hoogsteEigenwaarde - s;
	}
}



bool Ranglijst::compareRanglijstItems(RanglijstItem* a, RanglijstItem* b)
{
	return a->punten > b->punten;
}



void Ranglijst::dump(Spelerslijst* spelersLijst, std::ostream* outputStream)
{
	RanglijstItem* ranglijstItem = NULL;
	Speler* speler = NULL;

	if(outputStream != NULL)
		(*outputStream) << "Ranglijst:" << std::endl;

	for(unsigned int r = 0; r < _items.size(); r++)
	{
		ranglijstItem = _items.at(r);
		speler = spelersLijst->getSpelerById(ranglijstItem->spelerId);
		if(outputStream != NULL)
		{
			(*outputStream) << r + 1 << ". " << speler->naam << " " << ranglijstItem->punten << " "
			                << ranglijstItem->eigenwaarde << "   gsp:" << ranglijstItem->nrPartijenGespeeld << " gw:" << ranglijstItem->nrPartijenGewonnen
			                << " rem:" << ranglijstItem->nrPartijenRemise << " vrl:" << ranglijstItem->nrPartijenVerloren
			                << " vrij:" << ranglijstItem->nrVrijeRondes << " wpt:" << ranglijstItem->wedstrijdPunten
			                << "    (id: " << ranglijstItem->spelerId << ")" << std::endl;
		}
	}
}



void Ranglijst::_verwijderAlleItems()
{
	for(unsigned int i = 0; i < _items.size(); i++)
	{
		if(_items[i] != NULL)
		{
			delete _items[i];
			_items[i] = NULL;
		}
	}
	_items.clear();
}



Ranglijst* Ranglijst::maakStartRanglijst(int hoogsteEigenwaarde, Spelerslijst* spelerslijst)
{
  Ranglijst* startRanglijst = new Ranglijst();
  RanglijstItem* ranglijstItem = NULL;
  
  for(unsigned int s = 0; s < spelerslijst->getNrSpelers(); s++)
  {
  	Speler* speler = spelerslijst->getSpelerById(s);
    ranglijstItem = new RanglijstItem();
    ranglijstItem->spelerId = speler->id;
    ranglijstItem->eigenwaarde = hoogsteEigenwaarde - s;
    ranglijstItem->punten = ranglijstItem->eigenwaarde;
    startRanglijst->voegItemToe(ranglijstItem);
  }
  
  return startRanglijst;
}

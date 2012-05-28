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

#include <cmath>
#include <memory>
#include <set>
#include <stdexcept>
#include <iostream>

#include "Competitie.h"
#include "CompetitieParameters.h"
#include "Ranglijst.h"
#include "SchaakConstantes.h"
#include "IndelerGeneticAlgorithm.h"

//#define DEBUG_DUMP_COMPETITIEPARAMETERS
//#define DEBUG_DUMP_SPELERSLIJST
//#define DEBUG_DUMP_STARTRANGLIJST
//#define DEBUG_DUMP_UITSLAGEN

Competitie::Competitie()
{
	_logStream = NULL;
	_nrRondes = 0;
	_keizerIO = new KeizerIO();
	_rondes.push_back(NULL); // Ronde 0 bestaat niet.
}

Competitie::~Competitie()
{
	delete _keizerIO;
	_keizerIO = NULL; // TODO: make it an auto_ptr
}



void Competitie::leesCompetitieDatabase()
{
	if(_logStream != NULL)
	{
		(*_logStream) << "Bezig met lezen van competitieparameters...";
		_logStream->flush();
	}
	
	_competitieParameters = _keizerIO->leesCompetitieParameters();
	_nrRondes = _competitieParameters->aantalRondes;
	
	if(_logStream != NULL)
	{
#ifdef DEBUG_DUMP_COMPETITIEPARAMETERS
		(*_logStream) << std::endl;
		_competitieParameters->dump(_logStream);
#endif
		(*_logStream) << std::endl << "Bezig met lezen van spelerslijst...";
		_logStream->flush();
	}
		
	_keizerIO->leesSpelers(&_spelerslijst);
	
	if(_logStream != NULL)
	{
#ifdef DEBUG_DUMP_SPELERSLIJST
		(*_logStream) << std::endl;
		_spelerslijst.dump(_logStream);
#endif		
		(*_logStream) << std::endl << "Bezig met opstellen van startranglijst..."; 
		_logStream->flush();
	}
	
	_ranglijstNaRonde.clear(); // TODO: vervangen door VOLLEDIGE RESET-FUNCTIE!!
	_ranglijstNaRonde.push_back(Ranglijst::maakStartRanglijst(_competitieParameters->hoogsteEigenwaarde, &_spelerslijst));
	
	if(_logStream != NULL)
	{
#ifdef DEBUG_DUMP_STARTRANGLIJST
		(*_logStream) << std::endl;
		_ranglijstNaRonde[0]->dump(&_spelerslijst, _logStream);
#endif
		(*_logStream) << std::endl << "Bezig met lezen van ronde-uitslagen..." << std::endl;
	}
	
  for(int r = 1; r <= _competitieParameters->aantalRondes; r++)
  {
    if(_logStream != NULL)
    {
    	(*_logStream) << "Bezig met lezen van uitslag voor ronde: " << r;
    	_logStream->flush();
    }
    Ronde* ronde = _keizerIO->leesRonde(r);
    _rondes.push_back(ronde);
    if(_logStream != NULL)
	{
    	(*_logStream) << std::endl;
#ifdef DEBUG_DUMP_UITSLAGEN
		ronde->dump(&_spelerslijst, _logStream);
#endif
	}
  }
  
  if(_logStream != NULL)
		(*_logStream) << "Klaar met inlezen van competitiedatabase. Bezig met berekenen van competitiestand..." << std::endl;
  
  _berekenStand();
  
  if(_logStream != NULL)
		(*_logStream) << "Klaar met berekenen." << std::endl;
}



void Competitie::nieuweRonde()
{
	if(_logStream != NULL)
	{
		(*_logStream) << "Niewe ronde toevoegen...";
		_logStream->flush();
	}
	
	_rondes.push_back(new Ronde());
	
	if(_logStream != NULL)
		(*_logStream) << "OK" << std::endl;
}



void Competitie::maakIndeling()
{
	if(_logStream != NULL)
		(*_logStream) << "Bezig met maken van indeling..." << std::endl;
	
	std::auto_ptr<IndelerGeneticAlgorithm> indeler(new IndelerGeneticAlgorithm(this));
	
	// Neem de partiële indeling:
	Ronde* vastgelegdePartijen = _rondes.back();
	// Verkrijg een volledige indeling
	Ronde* ronde = indeler->maakIndeling();
	
	// Vervang de oude indeling door de nieuwe
	_rondes.pop_back();
	delete vastgelegdePartijen;
	vastgelegdePartijen = NULL;
	_rondes.push_back(ronde);
	
	if(_logStream != NULL)
		(*_logStream) << "Klaar met maken van indeling." << std::endl;
}



std::auto_ptr<CompetitieParameters> Competitie::getCompetitieParameters()
{
	return std::auto_ptr<CompetitieParameters>(new CompetitieParameters(*_competitieParameters));
}



unsigned int Competitie::getNrRondes()
{
	return _nrRondes;
}



std::auto_ptr<Spelerslijst> Competitie::getSpelerslijst()
{
	return std::auto_ptr<Spelerslijst>(new Spelerslijst(_spelerslijst));
}



void Competitie::setLogStream(std::ostream* stream)
{
	_logStream = stream;
}



std::auto_ptr<Ranglijst> Competitie::getRanglijstNaRonde(unsigned int ronde)
{
	if(ronde > _nrRondes)
		throw std::invalid_argument("Kan geen ranglijst geven voor een nog niet gespeelde ronde.");
		
	return std::auto_ptr<Ranglijst>(new Ranglijst(*_ranglijstNaRonde.at(ronde)));
}



std::auto_ptr<Ronde> Competitie::getRonde(unsigned int ronde)
{
	if(ronde > _rondes.size())
		throw std::invalid_argument("Kan geen rondes uit de toekomst geven.");
	
	return std::auto_ptr<Ronde>(new Ronde(*_rondes.at(ronde)));
}



void Competitie::writeRanglijstDocument(unsigned int ronde)
{
	if(_logStream != NULL)
		(*_logStream) << "Bezig met schrijven van ranglijstdocument..."; _logStream->flush();
	
	_keizerIO->writeRanglijstDocument(this, ronde);
	
	if(_logStream != NULL)
		(*_logStream) << "OK" << std::endl;
}



void Competitie::_berekenStand()
{
	if(_logStream != NULL)
		(*_logStream) << "Bezig met doorberekenen van de competitie..." << std::endl;
	
	// Sanity checks
	if(_rondes.size() < _nrRondes)
		throw std::logic_error("Kan stand niet berekenen - er missen uitslagen in de competitie.");

	// Vorige versie van de ranglijsten verwijderen
	for(unsigned int r = 1; r < _ranglijstNaRonde.size(); r++)  // Sla index 0 over (bewaar de startranglijst)
	{
		delete _ranglijstNaRonde[r];
		_ranglijstNaRonde[r] = NULL;
	}
	_ranglijstNaRonde.resize(_nrRondes + 1); // Startranglijst + nieuwe ranglijst na elke ronde
	
	// Nieuwe ranglijsten berekenen
	// TODO: eerst nagaan of de laatste ronde wel helemaal klaar is (middels .isVoltooid())!
	for(unsigned int r = 1; r <= _nrRondes; r++)
		_berekenStandNaRonde(r);
}



void Competitie::_berekenStandNaRonde(unsigned int ronde)
{
	Ranglijst* waarderingsRanglijst;
	
	if(_logStream != NULL)
		(*_logStream) << "Bezig met berekenen van stand na ronde " << ronde << "..." << std::endl;
	
	waarderingsRanglijst = _ranglijstNaRonde[ronde - 1];

	// Begin met een kopie van de startranglijst
	Ranglijst* nieuweRanglijst = new Ranglijst(*_ranglijstNaRonde[0]);

	// Verwerk alle partijen van de betreffende ronde
	for(unsigned int r = 1; r <= ronde; r++)
	{
		Ronde* objRonde = _rondes[r];
		for(unsigned int p = 0; p < objRonde->partijen.size(); p++)
			_verwerkPartij(objRonde->partijen.at(p), waarderingsRanglijst, nieuweRanglijst);
		_verwerkAfwezigeSpelers(objRonde, waarderingsRanglijst, nieuweRanglijst);
	}
	
	// Sorteer op keizerpunten en update alle eigenwaarden
	nieuweRanglijst->sorteer();
	
	// Sla de ranglijst op.
	_ranglijstNaRonde[ronde] = nieuweRanglijst;
}



void Competitie::_verwerkPartij(Partij* partij, Ranglijst* waarderingsRanglijst, Ranglijst* nieuweRanglijst)
{
	Speler* witSpeler = _spelerslijst.getSpelerById(partij->idWit);
	Speler* zwartSpeler = NULL;
	RanglijstItem* waarderingsRanglijstItemWit = waarderingsRanglijst->getRanglijstItemBySpelerId(witSpeler->id);
	RanglijstItem* waarderingsRanglijstItemZwart = NULL;
	RanglijstItem* nieuweRanglijstItemWit = nieuweRanglijst->getRanglijstItemBySpelerId(witSpeler->id);
	RanglijstItem* nieuweRanglijstItemZwart = NULL;
	if(partij->idZwart >= 0)
	{
		zwartSpeler = _spelerslijst.getSpelerById(partij->idZwart);
		waarderingsRanglijstItemZwart = waarderingsRanglijst->getRanglijstItemBySpelerId(zwartSpeler->id);
		nieuweRanglijstItemZwart = nieuweRanglijst->getRanglijstItemBySpelerId(zwartSpeler->id);
	}
	
	double puntenVerhogingWit = 0;
	double puntenVerhogingZwart = 0;
	switch(partij->resultaat)
	{
		case WIT_WINT:
			puntenVerhogingWit = WINST_FACTOR * waarderingsRanglijstItemZwart->eigenwaarde;
			puntenVerhogingZwart = VERLIES_FACTOR * waarderingsRanglijstItemWit->eigenwaarde;
			nieuweRanglijstItemWit->nrPartijenGewonnen++;
			nieuweRanglijstItemZwart->nrPartijenVerloren++;
			nieuweRanglijstItemWit->wedstrijdPunten += 1;
			break;
		case ZWART_WINT:
			puntenVerhogingWit = VERLIES_FACTOR * waarderingsRanglijstItemZwart->eigenwaarde;
			puntenVerhogingZwart = WINST_FACTOR * waarderingsRanglijstItemWit->eigenwaarde;
			nieuweRanglijstItemWit->nrPartijenVerloren++;
			nieuweRanglijstItemZwart->nrPartijenGewonnen++;
			nieuweRanglijstItemZwart->wedstrijdPunten += 1;
			break;
		case REMISE:
			puntenVerhogingWit = REMISE_FACTOR * waarderingsRanglijstItemZwart->eigenwaarde;
			puntenVerhogingZwart = REMISE_FACTOR * waarderingsRanglijstItemWit->eigenwaarde;
			nieuweRanglijstItemWit->nrPartijenRemise++;
			nieuweRanglijstItemZwart->nrPartijenRemise++;
			nieuweRanglijstItemWit->wedstrijdPunten += 0.5;
			nieuweRanglijstItemZwart->wedstrijdPunten += 0.5;
			break;
		case VRIJE_RONDE:
			puntenVerhogingWit = VRIJE_RONDE_FACTOR * waarderingsRanglijstItemWit->eigenwaarde;
			nieuweRanglijstItemWit->nrVrijeRondes++;
			break;
		default:
			throw std::logic_error("Ongedefiniëerd resultaat tegengekomen bij verwerken partij-uitslag.");
			break;
	}
	
	if(partij->resultaat != VRIJE_RONDE)
	{
		nieuweRanglijstItemWit->nrPartijenGespeeld++;
		nieuweRanglijstItemZwart->nrPartijenGespeeld++;
		nieuweRanglijstItemWit->nrPartijenWit++;
		nieuweRanglijstItemZwart->nrPartijenZwart++;
	}
	
	nieuweRanglijstItemWit->punten = lround(nieuweRanglijstItemWit->punten + puntenVerhogingWit);
	if(zwartSpeler != NULL)
		nieuweRanglijstItemZwart->punten = lround(nieuweRanglijstItemZwart->punten + puntenVerhogingZwart);
}



void Competitie::_verwerkAfwezigeSpelers(Ronde* ronde, Ranglijst* waarderingsRanglijst, Ranglijst* nieuweRanglijst)
{
	// Aanwezige spelers bepalen
	std::set<int> aanwezigeSpelers;
	for(unsigned int p = 0; p < ronde->partijen.size(); p++)
	{
		Partij* partij = ronde->partijen[p];
		aanwezigeSpelers.insert(partij->idWit);
		if(partij->idZwart >= 0)
			aanwezigeSpelers.insert(partij->idZwart);
	}

	// Ranglijst doorlopen op zoek naar afwezige spelers
	for(unsigned int plaats = 1; plaats <= nieuweRanglijst->getLengte(); plaats++)
	{
		RanglijstItem* ranglijstItem = nieuweRanglijst->getItemOpPlaats(plaats);
		int spelerId = ranglijstItem->spelerId;
		if(aanwezigeSpelers.find(spelerId) == aanwezigeSpelers.end()) // Speler afwezig?
		{
			ranglijstItem->nrAfwezig++;
			// Eventueel een beetje punten toekennen voor afwezigheid, indien niet te vaak afwezig geweest en indien reeds 1 keer aanwezig geweest dit seizoen
			if(ranglijstItem->nrAfwezig <= MAX_AFWEZIG && ranglijstItem->nrPartijenGespeeld > 0)
			{
				int eigenwaarde = waarderingsRanglijst->getRanglijstItemBySpelerId(spelerId)->eigenwaarde;
				ranglijstItem->punten = lround(ranglijstItem->punten + AFWEZIG_FACTOR * eigenwaarde);
			}
		}
	}
}



void Competitie::_vulTegenstanderLijsten()
{
	std::vector<int> emptyVector;
	
	_tegenstandersPerSpeler.clear();
	
	// Initialiseer alle spelerslijsten met de waarde voor "geen tegenstander"
	for(unsigned int s = 0; s < _spelerslijst.getNrSpelers(); s++)
	{
		_tegenstandersPerSpeler.push_back(emptyVector);
		_tegenstandersPerSpeler.at(s).resize(_nrRondes + 1, GEEN_SPELER); // _nrRondes + 1 omdat ronde 0 niet bestaat
	}
	
	// Verwerk de uitslagen
	for(unsigned int r = 1; r <= _nrRondes; r++)
	{
		Ronde* ronde = _rondes.at(r);
		for(unsigned int p = 0; p < ronde->partijen.size(); p++)
		{
			Partij* partij = ronde->partijen.at(p);
			_tegenstandersPerSpeler.at(partij->idWit).at(r) = partij->idZwart; // Werkt omdat idWit altijd een geldig speler-ID is
			if(partij->idZwart != GEEN_SPELER)
				_tegenstandersPerSpeler.at(partij->idZwart).at(r) = partij->idWit;
		}
	}
}



std::vector<int> Competitie::getTegenstanderLijst(unsigned int spelerId)
{
	if(spelerId >= _tegenstandersPerSpeler.size())
		throw std::invalid_argument("Speler ID buiten bereik.");
	
	return _tegenstandersPerSpeler.at(spelerId);  // <3 Copy constructors
}



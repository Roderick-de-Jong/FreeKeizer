#include "RanglijstItem.h"

RanglijstItem::RanglijstItem()
{
	plaats = 0;
	spelerId = -1;
	punten = 0;
	eigenwaarde = 0;
	nrPartijenGespeeld = 0;
	nrPartijenGewonnen = 0;
	nrPartijenRemise = 0;
	nrPartijenVerloren = 0;
	nrVrijeRondes = 0;
	nrAfwezig = 0;
	wedstrijdPunten = 0;
	nrPartijenWit = 0;
	nrPartijenZwart = 0;
}

RanglijstItem::~RanglijstItem()
{
}

RanglijstItem& RanglijstItem::operator=(const RanglijstItem& original)
{
	if(this != &original)
	{
		plaats = original.plaats;
		spelerId = original.spelerId;
		punten = original.punten;
		eigenwaarde = original.eigenwaarde;
		nrPartijenGespeeld = original.nrPartijenGespeeld;
		nrPartijenGewonnen = original.nrPartijenGewonnen;
		nrPartijenRemise = original.nrPartijenRemise;
		nrPartijenVerloren = original.nrPartijenVerloren;
		nrVrijeRondes = original.nrVrijeRondes;
		nrAfwezig = original.nrAfwezig;
		wedstrijdPunten = original.wedstrijdPunten;
		nrPartijenWit = original.nrPartijenWit;
		nrPartijenZwart = original.nrPartijenZwart;		
	}
	
	return *this;
}




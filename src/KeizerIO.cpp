#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <memory>
#include <iomanip>
#include <assert.h>
#include <string>
#include <stdexcept>

#include "KeizerIO.h"
#include "CompetitieParameters.h"
#include "Partij.h"
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

KeizerIO::KeizerIO()
{
}



KeizerIO::~KeizerIO()
{
}



std::auto_ptr<CompetitieParameters> KeizerIO::leesCompetitieParameters()
{
  std::auto_ptr<CompetitieParameters> competitieParameters;
  std::ifstream invoerBestand;
  char tempNaam[256];
  char tempDatum[256];
  
  invoerBestand.exceptions(std::ifstream::failbit | std::ifstream::badbit);  // Treat everything except EOF as an exception.
  invoerBestand.open("input/CompetitieParameters.txt", std::ios::in);

  competitieParameters = std::auto_ptr<CompetitieParameters>(new CompetitieParameters());
  invoerBestand.getline(tempNaam, 256);
  competitieParameters->naam = std::string(tempNaam);
  invoerBestand.getline(tempDatum, 256);
  competitieParameters->startDatum = std::string(tempDatum);
  invoerBestand >> competitieParameters->hoogsteEigenwaarde;
  invoerBestand >> competitieParameters->aantalRondes;
  
  invoerBestand.close();
  
  return competitieParameters;
}



void KeizerIO::leesSpelers(Spelerslijst* spelerslijst)
{
  std::ifstream invoerBestand;
  int nrSpelers = 0;
  Speler* speler = NULL;
  char tempNaam[256];
  
  invoerBestand.exceptions(std::ifstream::failbit | std::ifstream::badbit);  // Treat everything except EOF as an exception.
  invoerBestand.open("input/Spelers.txt", std::ios::in);
  
  invoerBestand >> nrSpelers;
  assert(nrSpelers > 0);
  
  for(int s = 0; s < nrSpelers; s++)
  {
    speler = new Speler();
    invoerBestand >> speler->id;   // TODO: remove this, since player IDs are automatically assigned in order of addition
    // Eet spaties alvorens getline te doen
    while(invoerBestand.peek() == ' ') { invoerBestand.get(); }
    invoerBestand.getline(tempNaam, 256);
    speler->naam = std::string(tempNaam);
    spelerslijst->voegSpelerToe(speler);
  }
  
  invoerBestand.close();
}



Ronde* KeizerIO::leesRonde(unsigned int r)
{
	Ronde* ronde = new Ronde();
	std::ostringstream bestandsnaam;
	std::ifstream invoerBestand;
	char datumBuffer[256];
	int nrUitslagen = 0;
	Partij* partij = NULL;
	int intResultaat;
	
	invoerBestand.exceptions(std::ifstream::failbit | std::ifstream::badbit);  // Treat everything except EOF as an exception.
	bestandsnaam << "input/Uitslag" << r << ".txt";
	
	invoerBestand.open(bestandsnaam.str().c_str(), std::ios::in);
	invoerBestand.getline(datumBuffer, 256);
	ronde->datum = datumBuffer;
	
	invoerBestand >> nrUitslagen;
	for(int u = 0; u < nrUitslagen; u++)
	{
		partij = new Partij();
		invoerBestand >> intResultaat;
		partij->resultaat = static_cast<enum PartijResultaat>(intResultaat);
		invoerBestand >> partij->idWit;
		if(partij->resultaat == WIT_WINT || partij->resultaat == ZWART_WINT || partij->resultaat == REMISE)
		{
			// Betekent dat er een zwartspeler geweest moet zijn.
			invoerBestand >> partij->idZwart;
		}
		ronde->partijen.push_back(partij);
	}
	
	invoerBestand.close();
	
	return ronde;
}



void KeizerIO::writeRanglijstDocument(Competitie* competitie, unsigned int ronde)
{
	std::ostringstream bestandsnaam;
	std::auto_ptr<CompetitieParameters> competitieParameters;
	std::string competitieTitel;
	std::ofstream document;
	std::auto_ptr<Ranglijst> ranglijst;
	std::auto_ptr<Ronde> uitslagAfgelopenRonde;
	std::string datumAfgelopenRonde;
	std::auto_ptr<Spelerslijst> spelerslijst;

	// Sanity checks
	if(competitie == NULL)
		throw std::invalid_argument("Competitie is NULL");
	if(ronde > competitie->getNrRondes())
		throw std::invalid_argument("Gevraagde ronde is nog niet gespeeld.");
	
	// Set up error handling
	document.exceptions(std::ofstream::failbit | std::ofstream::badbit);  // Treat everything except EOF as an exception.
	
	// Gather some data
	competitieParameters = competitie->getCompetitieParameters();
	if(competitieParameters->naam.empty())
		competitieTitel = "Naamloze Competitie";
	else
		competitieTitel = competitieParameters->naam;
		
	ranglijst = competitie->getRanglijstNaRonde(ronde);
	
	if(ronde > 0)
	{
		uitslagAfgelopenRonde = competitie->getUitslagVanRonde(ronde);
		datumAfgelopenRonde = uitslagAfgelopenRonde->datum;
	}
	else
	{
		datumAfgelopenRonde = competitieParameters->startDatum;
	}
	
	spelerslijst = competitie->getSpelerslijst();

	// Prepare output
	bestandsnaam << "output/Ranglijst na ronde " << std::setw(4) << std::setfill('0') << ronde << ".html";
	document.open(bestandsnaam.str().c_str(), std::ios::out);
	
	document << "<html>\n<head>\n<title>Ranglijst na ronde " << ronde << "</title>\n"
	         << "<style type=\"text/css\">\n"
	         << "table {border: dotted; border-width: 1}\n"
	         << "td {border: dotted; border-width: 1}\n"
	         << "</style>\n"
	         << "</head>\n"
	         << "<body>\n<h1>" << competitieTitel << "</h1>\n"
	         << "<h2>Ranglijst na ronde " << ronde << ", gespeeld op " << datumAfgelopenRonde << "</h2>\n"
	         << "<table>\n"
	         << "<tr><th>Plaats</th> <th>Naam speler</th> <th>Keizerpunten</th> <th>Eigenwaarde</th> "
	         << "<th>Aantal gespeelde partijen</th> <th>Aantal gewonnen</th> <th>Aantal remises</th> "
	         << "<th>Aantal verloren</th> <th>Aantal vrije rondes</th> <th>Wedstrijdpunten</th> "
	         << "<th>Wit</th> <th>Zwart</th></tr>\n"
	         ;
	
	for(unsigned int p = 1; p <= ranglijst->getLengte(); p++)
	{
		RanglijstItem* ranglijstItem = ranglijst->getItemOpPlaats(p);
		Speler* speler = spelerslijst->getSpelerById(ranglijstItem->spelerId);
		document << "<tr><td style=\"text-align:right\">" << p << "</td> <td>" << speler->naam << "</td> "
		         << "<td>" << ranglijstItem->punten << "</td> <td>" << ranglijstItem->eigenwaarde << "</td> "
		         << "<td>" << ranglijstItem->nrPartijenGespeeld << "</td> <td>" << ranglijstItem->nrPartijenGewonnen << "</td> "
		         << "<td>" << ranglijstItem->nrPartijenRemise << "</td> <td>" << ranglijstItem->nrPartijenVerloren << "</td>"
		         << "<td>" << ranglijstItem->nrVrijeRondes << "</td> <td>" << ranglijstItem->wedstrijdPunten << "</td>"
		         << "<td>" << ranglijstItem->nrPartijenWit << "</td> <td>" << ranglijstItem->nrPartijenZwart << "</td>"
		         << "</tr>\n";
	}

	document << "</table>\n";
	
	// Is er al minstens 1 ronde gespeeld?
	if(ronde > 0)
	{
		document << "<h2>Uitslagen ronde " << ronde << "</h2>\n"
		         << "<table>\n"
		         << "<tr><th>Wit</th> <th>Zwart</th> <th>Uitslag</th></tr>\n";

		for(unsigned int p = 0; p < uitslagAfgelopenRonde->partijen.size(); p++)
		{
			std::string uitslagTekst;
			Partij* partij = uitslagAfgelopenRonde->partijen.at(p);
			std::string naamWit = spelerslijst->getSpelerById(partij->idWit)->naam;
			std::string naamZwart;
			if(partij->idZwart >= 0)
				naamZwart = spelerslijst->getSpelerById(partij->idZwart)->naam;
			
			switch(partij->resultaat)
			{
				case ZWART_WINT:
					uitslagTekst = "0 - 1";
					break;
				case WIT_WINT:
					uitslagTekst = "1 - 0";
					break;
				case REMISE:
					uitslagTekst = "1/2 - 1/2";
					break;
				case VRIJE_RONDE:
					uitslagTekst = "vrij";
					break;
			}
			document << "<tr><td>" << naamWit << "</td> <td>" << naamZwart << "</td> <td>" << uitslagTekst << "</td>\n";
		}
		
		document << "</table>\n";
	}

	document << "</body>\n"
	         << "</html>";
	
	document.close();
}



void KeizerIO::writeKruistabel(Competitie* competitie, unsigned int ronde)
{
	std::ostringstream bestandsnaam;
	std::ofstream document;
	std::string competitieTitel;
	std::auto_ptr<Spelerslijst> spelerslijst = competitie->getSpelerslijst();
	std::auto_ptr<CompetitieParameters> competitieParameters = competitie->getCompetitieParameters();
	unsigned int aantalSpelers = spelerslijst->getNrSpelers();
	
	// Set up error handling.
	document.exceptions(std::ofstream::failbit | std::ofstream::badbit);  // Treat everything except EOF as an exception.
	
	if(competitieParameters->naam.empty())
		competitieTitel = "Naamloze Competitie";
	else
		competitieTitel = competitieParameters->naam;
	
	// Kruistabel voorbereiden
	float** kruistabel = new float*[aantalSpelers + 1];  // Kruistabel[s][t] is resultaat van speler s behaald tegen speler t.
	for(unsigned int s = 0; s < aantalSpelers + 1; s++)	 // laatste index is voor vrije rondes, vandaar arraylengte aantalSpelers+1
	{
		kruistabel[s] = new float[aantalSpelers + 1];
		for(unsigned int t = 0; t < aantalSpelers + 1; t++)
			kruistabel[s][t] = 0;
	}
	
	// Kruistabel vullen
	for(unsigned int r = 0; r < ronde; r++)
	{
		std::auto_ptr<Ronde> uitslagDezeRonde = competitie->getUitslagVanRonde(r);
		for(unsigned int p = 0; p < uitslagDezeRonde->partijen.size(); p++)
		{
			Partij* partij = uitslagDezeRonde->partijen[p];
			if(partij->idWit < 0 || static_cast<unsigned int>(partij->idWit) >= aantalSpelers)
				throw std::logic_error("Ongeldig speler-ID voor witspeler gevonden bij vullen van kruistabel.");
			switch(partij->resultaat)
			{
				case WIT_WINT:
					if(partij->idZwart < 0 || static_cast<unsigned int>(partij->idZwart) >= aantalSpelers)
						throw std::logic_error("Ongeldig speler-ID voor zwartspeler gevonden bij vullen van kruistabel.");
					kruistabel[partij->idWit][partij->idZwart] += 1;
					break;
				case ZWART_WINT:
					if(partij->idZwart < 0 || static_cast<unsigned int>(partij->idZwart) >= aantalSpelers)
						throw std::logic_error("Ongeldig speler-ID voor zwartspeler gevonden bij vullen van kruistabel.");
					kruistabel[partij->idZwart][partij->idWit] += 1;
					break;
				case REMISE:
					if(partij->idZwart < 0 || static_cast<unsigned int>(partij->idZwart) >= aantalSpelers)
						throw std::logic_error("Ongeldig speler-ID voor zwartspeler gevonden bij vullen van kruistabel.");
					kruistabel[partij->idWit][partij->idZwart] += 0.5;
					kruistabel[partij->idZwart][partij->idWit] += 0.5;
					break;
				case VRIJE_RONDE:
					kruistabel[partij->idWit][aantalSpelers] += 1;
					break;
				default:
					std::cerr << "Onbekend resultaat in uitslagen gevonden bij vullen van kruistabel!" << std::endl;
					assert(partij->resultaat >= 0 && partij->resultaat <= 3);
					break;
			}
		}
	}
	
	// Kruistabel uitvoeren
	bestandsnaam << "output/Kruistabel na ronde " << std::setw(4) << std::setfill('0') << ronde << ".html";
	document.open(bestandsnaam.str().c_str(), std::ios::out);
	
	document << "<html>\n<head>\n<title>Kruistabel na ronde " << ronde << "</title>\n"
	         << "<style type=\"text/css\">\n"
	         << "table {border: dotted; border-width: 1}\n"
	         << "td {border: dotted; border-width: 1}\n"
	         << "div.naamTegenstander { -webkit-transform:rotate(90deg); -moz-transform:rotate(90deg);"
	         << "	-o-transform: rotate(90deg); display:block }\n"
	         << "td.blackout { background-color: black }\n"
	         << "td.score { text-align: center}\n"
	         << "</style>\n"
	         << "</head>\n"
	         << "<body>\n<h1>" << competitieTitel << "</h1>\n"
	         << "<h2>Kruistabel na ronde " << ronde << ", gespeeld op "
	         << competitie->getUitslagVanRonde(ronde)->datum << "</h2>\n"
	         << "<table>\n"
	         << "<tr>\n"
	         << "  <th rowspan=\"2\" style=\"vertical-align:bottom\">Naam speler</td>\n"
	         << "  <th colspan=\"" << aantalSpelers + 1 << "\" style=\"text-align:center\">Naam tegenstander</th>\n"
	         << "</tr>\n";

	document << "<tr style=\"height:100px\">\n";
	for(unsigned int s = 0; s < aantalSpelers; s++)
	{
		document << "  <th><div class=\"naamTegenstander\">" << spelerslijst->getSpelerById(s)->naam << "</div></th>\n";
	}
	document << "<th><div class=\"naamTegenstander\">Vrije ronde</div></th>\n"
           << "</tr>\n";
		
	for(unsigned int s = 0; s < aantalSpelers; s++)
	{
		document << "<tr><td>" << spelerslijst->getSpelerById(s)->naam << "</td>";
		for(unsigned int t = 0; t < aantalSpelers + 1; t++)
		{
			if(s == t)
				document << "<td class=\"blackout\"></td>";
			else
				document << "<td class=\"score\">" << kruistabel[s][t] << "</td>";
		}
		document << "</tr>\n";
	}
	document << "<tr><td>Vrije ronde</td>";
	for(unsigned int t = 0; t < aantalSpelers; t++)
		document << "<td class=\"score\">" << kruistabel[aantalSpelers][t] << "</td>";
	document << "<td class=\"blackout\"></td></tr>\n";
	
	document << "</table>\n"
	         << "</body>\n"
	         << "</html>";

	// Clean-up
	for(unsigned int s = 0; s < aantalSpelers + 1; s++)
	{
		delete kruistabel[s];
		kruistabel[s] = NULL;
	}
	delete kruistabel;
	kruistabel = NULL;

	document.close();
}


#ifndef RANGLIJST_H
#define RANGLIJST_H

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

#include <vector>

#include "RankingItem.h"
#include "Unittest.h"

/**
 * Representatie van de stand in de competitie na een bepaalde ronde.
 */
class Ranglijst
{
	friend class FreeKeizerUnittest;
	
	public:
	/**
	 * Default constructor. Construeert een lege ranglijst.
	 */
	Ranglijst();
	
	/**
	 * Copy constructor. Dupliceert de gegeven ranglijst.
	 * De individuele RanglijstItem objecten worden gekopiëerd, dus er wordt geen geheugen gedeelt tussen de originele en nieuwe Ranglijst objecten.
	 */
	Ranglijst(Ranglijst& original);
	
	/**
	 * Destructor.
	 */
	virtual ~Ranglijst();

	/**
	 * Assignment operator.
	 */
	Ranglijst& operator=(const Ranglijst& original);

	/**
	 * Geeft het aantal plaatsen op de ranglijst.
	 * Dit is hetzelfde als het aantal spelers in de competitie.
	 */
	virtual unsigned int getLengte();

	/**
	 * Geeft een pointer naar de data voor de speler die op de gegeven plaats op de ranglijst staat.
	 * @param plaats De plaats op de ranglijst, bereik [1;n] waarbij n het aantal spelers in de competitie is.
	 */
	virtual RanglijstItem* getItemOpPlaats(unsigned int plaats);
	
	/**
	 * Geeft een pointer naar de data voor de gevraagde speler.
	 * @param spelerId Het unieke id van de speler in kwestie.
	 * @throw invalid_argument Als de gegeven speler niet voorkomt in de ranglijst.
	 */
	virtual RanglijstItem* getRanglijstItemBySpelerId(unsigned int spelerId);

	virtual void voegItemToe(RanglijstItem* item);

	/**
	 * Sorteert de ranglijst op Keizerpunten en past daarop de eigenwaarden aan.
	 */
	virtual void sorteer();
	
	/**
	 * Geeft de ranglijst weer op ge gegeven outputstream.
	 * @param spelersLijst Spelerslijst om de namen van spelers in op te zoeken.
	 * @param outputStream Een pointer naar een willekeurig ostream object (bijvoorbeeld cout); mag NULL zijn (dan komt er geen output).
	 */
	virtual void dump(Spelerslijst* spelersLijst, std::ostream* outputStream);
	
	/**
	 * Hulpfunctie voor sorteer().
	 */
	static bool compareRanglijstItems(RanglijstItem* a, RanglijstItem* b);
	
	static Ranglijst* maakStartRanglijst(int hoogsteEigenwaarde, Spelerslijst* spelerslijst);
	
	protected:
	
	/**
	 * Maakt de ranglijst leeg. Hulpfunctie voor de destructor en assignment operator.
	 */
	virtual void _verwijderAlleItems();
	
	/**
	 * De datastructuur met de daadwerkelijke ranglijst.
	 * Het RanglijstItem voor de hoogst geplaatste / beste speler staat in _items[0], de op-één-na-beste speler in _items[1], etc. 
	 */
	std::vector<RanglijstItem*> _items;
};

#endif // RANGLIJST_H


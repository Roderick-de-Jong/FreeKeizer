#ifndef RANKING_H
#define RANKING_H

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
class Ranking
{
	friend class FreeKeizerUnittest;
	
	public:
	/**
	 * Default constructor. Construeert een lege ranglijst.
	 */
	Ranking();
	
	/**
	 * Copy constructor. Dupliceert de gegeven ranglijst.
	 * De individuele RanglijstItem objecten worden gekopiëerd, dus er wordt geen geheugen gedeelt tussen de originele en nieuwe Ranglijst objecten.
	 */
	Ranking(Ranking& original);
	
	/**
	 * Destructor.
	 */
	virtual ~Ranking();

	/**
	 * Assignment operator.
	 */
	Ranking& operator=(const Ranking& original);

	/**
	 * Geeft het aantal plaatsen op de ranglijst.
	 * Dit is hetzelfde als het aantal spelers in de competitie.
	 */
	virtual unsigned int getLength();

	/**
	 * Geeft een pointer naar de data voor de speler die op de gegeven plaats op de ranglijst staat.
	 * @param plaats De plaats op de ranglijst, bereik [1;n] waarbij n het aantal spelers in de competitie is.
	 */
	virtual RankingItem* getItemAtPlace(unsigned int place);
	
	/**
	 * Geeft een pointer naar de data voor de gevraagde speler.
	 * @param spelerId Het unieke id van de speler in kwestie.
	 * @throw invalid_argument Als de gegeven speler niet voorkomt in de ranglijst.
	 */
	virtual RankingItem* getRankingItemByPlayerId(unsigned int playerId);

	virtual void addItem(RankingItem* item);

	/**
	 * Sorteert de ranglijst op Keizerpunten en past daarop de eigenwaarden aan.
	 */
	virtual void sort();
	
	/**
	 * Geeft de ranglijst weer op ge gegeven outputstream.
	 * @param spelersLijst Spelerslijst om de namen van spelers in op te zoeken.
	 * @param outputStream Een pointer naar een willekeurig ostream object (bijvoorbeeld cout); mag NULL zijn (dan komt er geen output).
	 */
	virtual void dump(PlayerList* playerList, std::ostream* outputStream);
	
	/**
	 * Hulpfunctie voor sorteer().
	 */
	static bool compareRankingItems(RankingItem* a, RankingItem* b);
	
	static Ranking* createInitialRanking(int highestEigenvalue, PlayerList* playerList);
	
	protected:
	
	/**
	 * Maakt de ranglijst leeg. Hulpfunctie voor de destructor en assignment operator.
	 */
	virtual void _deleteAllItems();
	
	/**
	 * De datastructuur met de daadwerkelijke ranglijst.
	 * Het RanglijstItem voor de hoogst geplaatste / beste speler staat in _items[0], de op-één-na-beste speler in _items[1], etc. 
	 */
	std::vector<RankingItem*> _items;
};

#endif // RANKING_H


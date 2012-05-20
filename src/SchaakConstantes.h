#ifndef SCHAAKCONSTANTES_H
#define SCHAAKCONSTANTES_H

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

/**
 * Constantes ter representatie van uitslagen van partijen
 */
enum PartijResultaat
{
	ZWART_WINT = 0,
	WIT_WINT = 1,
	REMISE = 2,
	VRIJE_RONDE = 3
};

/**
 * Waarde voor Speler-ID integers die 'niemand' betekent.
 */
const int GEEN_SPELER = -1;

const double WINST_FACTOR       = (double) 6 / 6;  // Deel van eigenwaarde tegenstander dat bij winst wordt toegekend
const double VERLIES_FACTOR     = (double) 1 / 6;  // Deel van eigenwaarde tegenstander dat bij verlies wordt toegekend
const double REMISE_FACTOR      = (double) 3 / 6;  // Deek van eigenwaarde tegenstander dat bij remise wordt toegekend
const double VRIJE_RONDE_FACTOR = (double) 4 / 6;  // Deel van eigen eigenwaarde dat bij vrije ronde (oneven) wordt toegekend
const double AFWEZIG_FACTOR     = (double) 2 / 6;  // Deel van eigen eigenwaarde dat bij incidentele afwezigheid wordt toegekend
const int    MAX_AFWEZIG        =              5;  // Maximaal aantal keer per seizoen dat afwezigheid punten oplevert

#endif // SCHAAKCONSTANTES_H


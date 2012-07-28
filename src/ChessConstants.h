#ifndef CHESSCONSTANTS_H
#define CHESSCONSTANTS_H

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
 * Constants representing the result of a single game of chess.
 * TODO: replace by more extensible class, for user-defined values
 */
enum GameResult
{
	BLACK_WINS = 0,
	WHITE_WINS = 1,
	DRAW = 2,
	FREE = 3, // Indicates that WHITE has no opponent in the current round.
	YET_TO_BE_PLAYED = 4, // Indicates this game hasn't been played (or finished) yet.
	ABSENT_WITH_VALID_REASON = 5, // "WHITE is absent (with a valid reason) and thus hasn't been paired up with an opponent this round."
	ABSENT_WITHOUT_VALID_REASON = 6 // "WHITE is absent (with no valid reason) and thus hasn't been paired up with an opponent this round."
};

/**
 * Special value for Player-ID integers meaning 'nobody'.
 */
const int NO_PLAYER = -1;

const double WIN_FACTOR         = (double) 6 / 6;  // Portion of opponent's eigenvalue awarded for a win
const double LOSS_FACTOR        = (double) 1 / 6;  // Portion of opponent's eigenvalue awarded for a loss
const double DRAW_FACTOR        = (double) 3 / 6;  // Portion of opponent's eigenvalue awarded for a draw
const double FREE_FACTOR        = (double) 4 / 6;  // Portion of own eigenvalue awarded for a free round
const double ABSENT_FACTOR      = (double) 2 / 6;  // Portion of own eigenvalue awarded when one is absent for a valid reason
const int    MAX_ROUNDS_ABSENT  =              5;  // Max times someone is allowed to receive points while absent

#endif // CHESSCONSTANTS_H


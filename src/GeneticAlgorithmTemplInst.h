#ifndef GENETIC_ALGORITHM_TEMPLINST_H
#define GENETIC_ALGORITHM_TEMPLINST_H

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

///// This file is included by GeneticAlgorithm.cpp to know which templates to instantiate! /////

// Request instantion of the following template classes:
template class FitnessFunction<Genotype, char>;
template class MutationFunction<Genotype, char>;
template class Crossover<Genotype, char>;
template class GeneticAlgorithm<Genotype, char>;

#endif /* GENETIC_ALGORITHM_TEMPLINST_H */
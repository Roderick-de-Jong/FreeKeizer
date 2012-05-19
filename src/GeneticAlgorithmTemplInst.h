#ifndef GENETIC_ALGORITHM_TEMPLINST_H
#define GENETIC_ALGORITHM_TEMPLINST_H

///// This file is included by GeneticAlgorithm.cpp to know which templates to instantiate! /////

// Request instantion of the following template classes:
template class FitnessFunction<Genotype, char>;
template class MutationFunction<Genotype, char>;
template class Crossover<Genotype, char>;
template class GeneticAlgorithm<Genotype, char>;

#endif /* GENETIC_ALGORITHM_TEMPLINST_H */
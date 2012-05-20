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

#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <math.h>

#include "GeneticAlgorithm.h"



///// SimpleCrossover implementation /////

template<template<class> class T_GENOTYPE, class T_GENE>
T_GENOTYPE<T_GENE> SimpleCrossover<T_GENOTYPE, T_GENE>::createChild(const std::vector<T_GENOTYPE<T_GENE> >& parents)
{
	if(parents.size() == 0)
		throw std::invalid_argument("createChild(): No parents given.");
	
	unsigned int geneCount = parents.at(0).getGeneCount();
	std::vector<T_GENE> childGenes;
	
	// Figure out how many genes will come from each parent
	unsigned int genesPerParent = geneCount / parents.size();
	// If the genotype length isn't an exact multiple of the number of parents, then the first X parents will each give 1 extra gene:
	unsigned int parentsGivingExtraGene = geneCount % parents.size();
	
	// Collect each parent's contribution:
	for(unsigned int p = 0; p < parents.size(); p++)
	{
		unsigned int genesFromThisParent = genesPerParent + (p < parentsGivingExtraGene ? 1 : 0);
		for(unsigned int g = 0; g < genesFromThisParent; g++)
		{
			childGenes.push_back(parents.at(p).getGeneAt(g));
		}
	}
	
	// Create the actual child genotype
	return T_GENOTYPE<T_GENE>(childGenes);
}


///// UniformMutationFunction implementation /////
template<template<class> class T_GENOTYPE, class T_GENE>
void UniformMutationFunction<T_GENOTYPE, T_GENE>::setMutationRate(double newRate)
{
	if(newRate < 0 || newRate > 1)
		throw std::invalid_argument("Mutation rate out of range. Valid range is [0;1].");
	
	_mutationRate = newRate;
}



template<template<class> class T_GENOTYPE, class T_GENE>
void UniformMutationFunction<T_GENOTYPE, T_GENE>::mutate(T_GENOTYPE<T_GENE>& genotype)
{
	for(unsigned int g = 0; g < genotype.getGeneCount(); g++)
	{
		double randomValue = double(rand()) / RAND_MAX; // Random value in range [0;1]
		if(randomValue < _mutationRate)
		{
			genotype.setGeneAt(g, mutate(genotype.getGeneAt(g)));
		}
	}
}



///// GeneticAlgorithm implementation

template<template<class> class T_GENOTYPE, class T_GENE>
GeneticAlgorithm<T_GENOTYPE, T_GENE>::GeneticAlgorithm()
{
}

template<template<class> class T_GENOTYPE, class T_GENE>
GeneticAlgorithm<T_GENOTYPE, T_GENE>::~GeneticAlgorithm()
{
	_population.clear();
	_populationFitness.clear();
	_oldPopulation.clear();
	_oldPopulationFitness.clear();
	_selectionPercentage = 0.0;
}

template<template<class> class T_GENOTYPE, class T_GENE>
void GeneticAlgorithm<T_GENOTYPE, T_GENE>::seed(const std::vector<T_GENOTYPE<T_GENE> >& initialPopulation,
                                                std::auto_ptr<FitnessFunction<T_GENOTYPE, T_GENE> > fitnessFunction,
                                                std::auto_ptr<Crossover<T_GENOTYPE, T_GENE> > crossover,
                                                std::auto_ptr<MutationFunction<T_GENOTYPE, T_GENE> > mutationFunction,
                                                const double selectionPercentage)
{
	_population = initialPopulation;
	_oldPopulation.clear();
	
	_populationFitness.clear();
	_oldPopulationFitness.clear();
	
	_fitnessFunction = fitnessFunction;
	_crossover = crossover;
	_mutationFunction = mutationFunction;
	_selectionPercentage = selectionPercentage;
}

template<template<class> class T_GENOTYPE, class T_GENE>
void GeneticAlgorithm<T_GENOTYPE, T_GENE>::calculateFitnessValues()
{
	_populationFitness.clear();
	
	typename std::vector<T_GENOTYPE<T_GENE> >::iterator genotype;
	for(genotype = _population.begin(); genotype != _population.end(); genotype++)
	{
		double fitness = _fitnessFunction->calculateFitness(*genotype);
		_populationFitness.push_back(fitness);
	}
}

// TODO: allow for variable number of parents!
// TODO: allow for population growth and population reduction!
template<template<class> class T_GENOTYPE, class T_GENE>
void GeneticAlgorithm<T_GENOTYPE, T_GENE>::createNextGeneration()
{
	// Selection
	unsigned int selectionSize = (unsigned int)std::max((double)0, std::min((double)_population.size(), (double)floor(_selectionPercentage * _population.size() + 0.5)));
	std::vector<unsigned int> indicesOfSelectedGenotypes = _getIndicesOfTopFitnesses(selectionSize, _populationFitness);
	std::vector<T_GENOTYPE<T_GENE> > selectedGenotypes;
	for(std::vector<unsigned int>::iterator i = indicesOfSelectedGenotypes.begin(); i != indicesOfSelectedGenotypes.end(); i++)
		selectedGenotypes.push_back(_population[*i]);

	// Make room for the new generation
	_oldPopulation = _population;
	_oldPopulationFitness = _populationFitness;
	_population.clear();
	_populationFitness.clear();
	
	// Crossover & mutation
	while(_population.size() < _oldPopulation.size())
	{
		// Randomly pair up all selected genotypes
		random_shuffle(selectedGenotypes.begin(), selectedGenotypes.end());
		for(unsigned int g = 1; g < selectedGenotypes.size(); g += 2)
		{
			std::vector<T_GENOTYPE<T_GENE> > parents;
			parents.push_back(selectedGenotypes.at(g));
			parents.push_back(selectedGenotypes.at(g - 1));
			T_GENOTYPE<T_GENE> child = _crossover->createChild(parents);
			_mutationFunction->mutate(child);
			_population.push_back(child);
			if(_population.size() >= _oldPopulation.size())
				break;
		}
	}
}

template<template<class> class T_GENOTYPE, class T_GENE>
std::vector<T_GENOTYPE<T_GENE> > GeneticAlgorithm<T_GENOTYPE, T_GENE>::getPopulation()
{
	std::vector<T_GENOTYPE<T_GENE> > populationCopy;
	for(unsigned int p = 0; p < _population.size(); p++)
		populationCopy.push_back(T_GENOTYPE<T_GENE>(_population[p]));
	return populationCopy;
}

template<template<class> class T_GENOTYPE, class T_GENE>
std::vector<double> GeneticAlgorithm<T_GENOTYPE, T_GENE>::getPopulationFitness()
{
	return _populationFitness;
}

template<template<class> class T_GENOTYPE, class T_GENE>
std::vector<unsigned int> GeneticAlgorithm<T_GENOTYPE, T_GENE>::_getIndicesOfTopFitnesses(const unsigned int count, const std::vector<double>& fitnesses)
{
	// Sort a shadow copy of the vector
	std::vector<unsigned int> shadowVector;
	for(unsigned int i = 0; i < fitnesses.size(); i++)
		shadowVector.push_back(i);
	sort(shadowVector.begin(), shadowVector.end(), CompareFitness(&fitnesses));
	std::vector<unsigned int> indices;
	// Select the top N values now and return their indices
	for(unsigned int i = 0; i < count; i++)
		indices.push_back(shadowVector[i]);
	return indices;
}



///// Genotype implementation /////

template<typename T_GENE>
Genotype<T_GENE>::Genotype(const std::vector<T_GENE>& genes)
{
	_genes = genes;
}

template<typename T_GENE>
Genotype<T_GENE>::Genotype(const Genotype& source)
{
	_genes = source._genes;
}

template<typename T_GENE>
Genotype<T_GENE>::~Genotype()
{
	_genes.clear();
}

template<typename T_GENE>
unsigned int Genotype<T_GENE>::getGeneCount() const
{
	return _genes.size();
}

template<typename T_GENE>
T_GENE Genotype<T_GENE>::getGeneAt(unsigned int index) const
{
	return _genes.at(index);
}

template<typename T_GENE>
void Genotype<T_GENE>::setGeneAt(unsigned int index, T_GENE geneValue)
{
	_genes.at(index) = geneValue;
}

template<typename T_GENE>
std::vector<T_GENE> Genotype<T_GENE>::getAllGenes() const
{
	std::vector<T_GENE> copyOfGenes;
	for(unsigned int g = 0; g < _genes.size(); g++)
		copyOfGenes.push_back(_genes[g]);
	return copyOfGenes;
}



// Instantiate the above template according to the following customized header file:
#include "GeneticAlgorithmTemplInst.h"

#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <math.h>

#include "GeneticAlgorithm.h"



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
	_fitnessFunction = NULL;
	_selectionPercentage = 0.0;
}

template<template<class> class T_GENOTYPE, class T_GENE>
void GeneticAlgorithm<T_GENOTYPE, T_GENE>::seed(const std::vector<T_GENOTYPE<T_GENE> >& initialPopulation, double (*fitnessFunction)(T_GENOTYPE<T_GENE>), const double selectionPercentage)
{
	_population = initialPopulation;
	_oldPopulation.clear();
	
	_populationFitness.clear();
	_oldPopulationFitness.clear();
	
	_fitnessFunction = fitnessFunction;
	_selectionPercentage = selectionPercentage;
}

template<template<class> class T_GENOTYPE, class T_GENE>
void GeneticAlgorithm<T_GENOTYPE, T_GENE>::calculateFitnessValues()
{
	_populationFitness.clear();
	
	typename std::vector<T_GENOTYPE<T_GENE> >::iterator genotype;
	for(genotype = _population.begin(); genotype != _population.end(); genotype++)
	{
		double fitness = _fitnessFunction(*genotype);
		_populationFitness.push_back(fitness);
	}
}

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
	std::vector<double> geneMutationProbabilities;
	unsigned int geneCount = 0;
	if(selectedGenotypes.size() > 0)
		geneCount = selectedGenotypes[0].getGeneCount();
	for(unsigned int i = 0; i < geneCount; i++)
		geneMutationProbabilities.push_back(0.05); // 5% mutation rate. TODO: make this dynamic & configurable etc.
	
	while(_population.size() < _oldPopulation.size())
	{
		// Randomly pair up all selected genotypes
		random_shuffle(selectedGenotypes.begin(), selectedGenotypes.end());
		for(unsigned int g = 1; g < selectedGenotypes.size(); g += 2)
		{
			// Create the child of selectedGenotypes[g-1] and selectedGenotypes[g]
			unsigned int crossoverPoint = (geneCount + 1) * double(rand()) / RAND_MAX;  // geneCount + 1 because otherwise the only way of getting the crossover point beyond the last gene would be if rand() == RAND_MAX exactly
			bool startValue = rand() > (double(RAND_MAX) / 2);
			std::vector<bool> crossoverPattern = _createSinglePointCrossover(geneCount, crossoverPoint, startValue);
			T_GENOTYPE<T_GENE> child = selectedGenotypes[g - 1].crossoverWith(selectedGenotypes[g], crossoverPattern, geneMutationProbabilities);
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

template<template<class> class T_GENOTYPE, class T_GENE>
std::vector<bool> GeneticAlgorithm<T_GENOTYPE, T_GENE>::_createSinglePointCrossover(const unsigned int geneCount, const unsigned int crossoverPoint, const bool startValue)
{
	std::vector<bool> crossoverPattern;
	for(unsigned int i = 0; i < geneCount; i++)
		crossoverPattern.push_back(i < crossoverPoint ? startValue : !startValue);
	return crossoverPattern;
}



///// Genotype implementation /////

template<typename T_GENE>
Genotype<T_GENE>::Genotype(const std::vector<T_GENE>& genes, T_GENE (*mutationFunction)(T_GENE))
{
	_genes = genes;
	_mutationFunction = mutationFunction;
}

template<typename T_GENE>
Genotype<T_GENE>::Genotype(const Genotype& source)
{
	_genes = source._genes;
	_mutationFunction = source._mutationFunction;
}

template<typename T_GENE>
Genotype<T_GENE>::~Genotype()
{
	_genes.clear();
	_mutationFunction = NULL;
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
std::vector<T_GENE> Genotype<T_GENE>::getAllGenes() const
{
	std::vector<T_GENE> copyOfGenes;
	for(unsigned int g = 0; g < _genes.size(); g++)
		copyOfGenes.push_back(_genes[g]);
	return copyOfGenes;
}

template<typename T_GENE>
Genotype<T_GENE> Genotype<T_GENE>::crossoverWith(const Genotype& otherParent, const std::vector<bool>& crossoverPattern, const std::vector<double>& geneMutationProbabilities)
{
	std::vector<T_GENE> childGenes;
	
	if(getGeneCount() != otherParent.getGeneCount())
		throw std::invalid_argument("Other parent's genotype is incompatible due to different gene count.");
	
	if(crossoverPattern.size() != getGeneCount())
		throw std::invalid_argument("Crossover pattern length doesn't match genotype length.");
	
	if(geneMutationProbabilities.size() != getGeneCount())
		throw std::invalid_argument("List of mutation probabilities doesn't match genotype length.");
	
	std::vector<double>::const_iterator mutationProbabilityIterator;
	for(mutationProbabilityIterator = geneMutationProbabilities.begin(); mutationProbabilityIterator != geneMutationProbabilities.end(); mutationProbabilityIterator++)
		if(*mutationProbabilityIterator < 0 || *mutationProbabilityIterator > 1)
			throw std::invalid_argument("Gene mutation probability out of range. Probability value must be in range [0;1].");
	
#if 0
	std::cout << "Creating child. Parents: ";
	for(unsigned int g = 0; g < getGeneCount(); g++)
		cout << getGeneAt(g);
	std::cout << ", ";
	for(unsigned int g = 0; g < getGeneCount(); g++)
		cout << otherParent.getGeneAt(g);
	std::cout << std::endl;
	
	std::cout << "Mutation probabilities: ";
	for(unsigned int m = 0; m < geneMutationProbabilities.size(); m++)
		cout << geneMutationProbabilities[m] << ' ';
	cout << endl;
#endif

	for(unsigned int i = 0; i < getGeneCount(); i++)
	{
		T_GENE gene = crossoverPattern[i] ? otherParent.getGeneAt(i) : getGeneAt(i);
		//std::cout << "selected gene: " << gene;
		double randomValue = double(rand()) / RAND_MAX; // Random value in range [0;1]
		if(randomValue < geneMutationProbabilities[i])
			gene = _mutationFunction(gene);
		//std::cout << ", result after optional mutation: " << gene << std::endl;
		childGenes.push_back(gene);
	}

#if 0
	std::cout << "Child: ";
	for(unsigned int g = 0; g < getGeneCount(); g++)
		std::cout << childGenes[g];
	std::cout << std::endl;
#endif

	return Genotype(childGenes, _mutationFunction);
}



// Instantiate the above template according to the following customized header file:
#include "GeneticAlgorithmTemplInst.h"

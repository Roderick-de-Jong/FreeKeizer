#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

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

#include <memory>
#include <vector>

// USAGE:
// - Create a starting population:
//   Fill a std::vector<Genotype<x>> with individuals. You can choose class x to suit your problem set, or even create a custom Genotype subclass.
//
// - Create a fitness function:
//   Subclass FitnessFunction and implement its calculateFitness() method according to your needs.
//
// - Create a GeneticAlgorithm object
//   Instantiate a GeneticAlgorithm<Genotype, x> and call its seed() function with your initial population and your fitness function.
//
// - Start evolving potential solutions!
//   Do some calls to the GeneticAlgorithm's createNextGeneration() method.



/**
 * Abstract base class for genotype fitness calculators.
 * @see http://en.wikipedia.org/wiki/Fitness_function
 */
template<template<class> class T_GENOTYPE, class T_GENE>
class FitnessFunction
{
	public:
	FitnessFunction() {}
	virtual ~FitnessFunction() {}
	
	virtual double calculateFitness(const T_GENOTYPE<T_GENE>& genotype) = 0;
};



/**
 * Abstract base class for various crossover implementations.
 */
template<template<class> class T_GENOTYPE, class T_GENE>
class Crossover
{
	public:
	Crossover() {}
	virtual ~Crossover() {}
	
	/**
	 * Creates a child genotype based on the genotypes of any positive number of parents.
	 * @param parents A vector of size 1 or larger containing the parent genotypes.
	 *                The number of parents depends on the GeneticAlgorithm's parents-per-child setting.
	 */
	virtual T_GENOTYPE<T_GENE> createChild(const std::vector<T_GENOTYPE<T_GENE> >& parents) = 0;
};



/**
 * Simple crossover system, using equal length parts of each parent's genotype.
 * Requires that parents all have the same genotype length!
 * 
 * With genotype length N and P parents, the first P mod N parents will donate
 * P / N + 1 genes each (integer division!), and the remaining parents will each
 * donate P / N genes (again integer division).
 * 
 * Parents donate genes in the order as they appear in the parents vector supplied
 * to the createChild method.
 */
template<template<class> class T_GENOTYPE, class T_GENE>
class SimpleCrossover : public Crossover<T_GENOTYPE, T_GENE>
{
	public:
	SimpleCrossover() {}
	virtual ~SimpleCrossover() {}
	
	/**
	 * @see Crossover::createChild
	 */
	virtual T_GENOTYPE<T_GENE> createChild(const std::vector<T_GENOTYPE<T_GENE> >& parents);
};



/**
 * Abstract base class for various mutation functions.
 * @see http://en.wikipedia.org/wiki/Mutation_%28genetic_algorithm%29
 */
template<template<class> class T_GENOTYPE, class T_GENE>
class MutationFunction
{
	public:
	MutationFunction() {}
	virtual ~MutationFunction() {}
	
	/**
	 * Mutates the given genotype.
	 */
	virtual void mutate(T_GENOTYPE<T_GENE>& genotype) = 0;
};



/**
 * Simple uniform mutation, using a fixed mutation rate that is applied to each gene.
 * Usage: create a subclass and implement mutate(gene), call setMutationRate
 *        and then calls can be made to mutate(genotype).
 */
template<template<class> class T_GENOTYPE, class T_GENE>
class UniformMutationFunction : public MutationFunction<T_GENOTYPE, T_GENE>
{
	public:
	UniformMutationFunction() : _mutationRate(0) {}
	UniformMutationFunction(const double mutationRate) : _mutationRate(mutationRate) {}
	virtual ~UniformMutationFunction() {}
	
	/**
	 * Retrieves the current mutation rate.
	 * @see setMutationRate
	 * @return The mutation rate, range [0;1].
	 */
	virtual double getMutationRate() { return _mutationRate; }
	
	/**
	 * Sets the mutation rate per gene to be used.
	 * The mutation rate is the chance for any given gene to be mutated during a call to mutate(genotype).
	 * @param newRate The mutation rate, valid range is [0;1] where 0 means 'never mutate' and 1 means 'always mutate'.
	 */
	virtual void setMutationRate(double newRate);
	
	/**
	 * @see MutationFunction::mutate
	 * Implementation: for each gene, calls mutate(gene) with chance _mutationRate.
	 */
	virtual void mutate(T_GENOTYPE<T_GENE>& genotype);
	
	/**
	 * Mutates a single gene.
	 * Provide your own implementation of this.
	 */
	virtual T_GENE mutate(const T_GENE& gene) = 0;
	
	protected:
	
	/**
	 * The mutation rate per gene.
	 * @see getMutationRate
	 */
	double _mutationRate;
};



/**
 * Encompasses everything needed to apply a Genetic Algorithm to a particular problem.
 * Provide a population and a fitness function, and this class will do the rest.
 * @see http://en.wikipedia.org/wiki/Genetic_algorithm
 */
template<template<class> class T_GENOTYPE, class T_GENE>
class GeneticAlgorithm
{
	public:
	
	/**
	 * Helper class for fitness-based Genotype selection.
	 */
	class CompareFitness
	{
		public:
		CompareFitness(const std::vector<double>* const fitnesses) { _fitnesses = fitnesses; }
		virtual ~CompareFitness() { _fitnesses = NULL; }
		virtual bool operator()(const unsigned int indexA, const unsigned int indexB)
		{
			return _fitnesses->at(indexA) > _fitnesses->at(indexB);
		}
		const std::vector<double>* _fitnesses;
	};
	
	/**
	 * Default constructor. Creates an empty GeneticAlgorithm. You'll need to set an initial population and
	 * provide a fitness function before you can do anything useful with the object created by this constructor.
	 */
	GeneticAlgorithm();
	
	virtual ~GeneticAlgorithm();
	
	/**
	 * Initializes the Genetic Algorithm, setting the population to the genotypes contained in initialPopulation,
	 * accepting the fitness function to be used for selection, and clearing any pre-existing data concerning
	 * the last generation and any fitness values.
	 * 
	 * @param fitnessFunction A pointer to an object of class FitnessFunction. The GeneticAlgorithm takes ownership of the object.
	 */
	virtual void seed(const std::vector<T_GENOTYPE<T_GENE> >& initialPopulation,
	                  std::auto_ptr<FitnessFunction<T_GENOTYPE, T_GENE> > fitnessFunction,
	                  std::auto_ptr<Crossover<T_GENOTYPE, T_GENE> > crossover,
	                  std::auto_ptr<MutationFunction<T_GENOTYPE, T_GENE> > mutationFunction,
	                  const double selectionPercentage);
	
	/**
	 * Calculates the fitness value for each of the genotypes in the current population
	 * and stores the result in the _populationFitness vector.
	 */
	virtual void calculateFitnessValues();
	
	/**
	 * Performs (fitness-based) Selection on the current population, followed by Crossover
	 * and Mutation in order to create a new population of Genotypes: the next generation.
	 *
	 * The parent population will be moved to the _oldPopulation vector, and its fitness values to _oldPopulationFitness.
	 * The _populationFitness vector will be cleared, and the _population vector will contain the new generation.
	 */
	virtual void createNextGeneration();

	/**
	 * Retrieves a copy of the current population.
	 */	
	virtual std::vector<T_GENOTYPE<T_GENE> > getPopulation();
	
	/**
	 * Retrieves a copy of the current population fitness data.
	 * Requires a call to calculateFitnessValues() after any population change (including the initial seeding) or an empty vector will be returned.
	 */
	virtual std::vector<double> getPopulationFitness();
	
	protected:
	
	/**
	 * Utility function that selects the top N values from the given vector and returns a vector containing the indices for those top N values.
	 */
	virtual std::vector<unsigned int> _getIndicesOfTopFitnesses(const unsigned int count, const std::vector<double>& fitnesses);
	
	/**
	 * The current genotype population.
	 */
	std::vector<T_GENOTYPE<T_GENE> > _population;

	/**
	 * The fitness values for all genotypes in the current generation.
	 * Uses the same indices as the population vector.
	 */
	std::vector<double> _populationFitness;

	/**
	 * The genotype population of the last generation.
	 * Empty if the current generation is the first generation.
	 */
	std::vector<T_GENOTYPE<T_GENE> > _oldPopulation;

	/**
	 * The fitness values for all genotypes in the last generation.
	 * Uses the same indices as the oldPopulation vector.
	 */
	std::vector<double> _oldPopulationFitness;

	/**
	 * The fitness function to be applied to the genotypes.
	 */
	std::auto_ptr<FitnessFunction<T_GENOTYPE, T_GENE> > _fitnessFunction;
	
	/**
	 * The object that takes care of (a)sexual reproduction.
	 */
	std::auto_ptr<Crossover<T_GENOTYPE, T_GENE> > _crossover;
	
	/**
	 * The mutation function to be applied to newly created genotypes during reproduction.
	 */
	std::auto_ptr<MutationFunction<T_GENOTYPE, T_GENE> > _mutationFunction;
	
	/**
	 * The top percentage of the population (in terms of fitness) that will be selected for procreation.
	 * The value of _selectionPercentage is the percentage divided by 100, resulting in a value between 0 and 1 (inclusive).
	 * Example: a _selectionPercentage of 0.5 means the fittest 50% of the population will be selected,
	 * whereas a _selectionPercentage of 0.1 means the fittest 10% will be selected.
	 */
	double _selectionPercentage;
};



/**
 * Representation of the genetic data of one individual candidate solution.
 * The T_GENE template parameter should be either a basic type, or a class with an assignment operator.
 */
template<typename T_GENE>
class Genotype
{
	public:
	/**
	 * Creates a Genotype for the given genes.
	 */
	Genotype(const std::vector<T_GENE>& genes);
	
	/**
	 * Creates a copy of the given genotype.
	 * The copy will have the same genes and the same mutation function.
	 */
	Genotype(const Genotype& source);
	
	virtual ~Genotype();
	
	/**
	 * Returns the number of genes in this genotype.
	 * This number is always greater than or equal to zero.
	 */
	virtual unsigned int getGeneCount() const;
	
	/**
	 * Retrieves a copy of the gene at the given index.
	 * Valid indices are [0;n> where n is the number of genes in this genotype.
	 */
	virtual T_GENE getGeneAt(unsigned int index) const;
	
	virtual void setGeneAt(unsigned int index, T_GENE geneValue);
	
	/**
	 * Returns a copy of the vector containing all of this Genotype's genes.
	 */
	virtual std::vector<T_GENE> getAllGenes() const;
	
	protected:
	
	/**
	 * Datastructure to hold the gene data.
	 */
	std::vector<T_GENE> _genes;
};

#endif /* GENETIC_ALGORITHM_H */


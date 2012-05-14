#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include <vector>

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
	
	GeneticAlgorithm();
	virtual ~GeneticAlgorithm();
	
	/**
	 * Initializes the Genetic Algorithm, setting the population to the genotypes contained in initialPopulation,
	 * accepting the fitness function to be used for selection, and clearing any pre-existing data concerning
	 * the last generation and any fitness values.
	 */
	virtual void seed(const std::vector<T_GENOTYPE<T_GENE> >& initialPopulation, double (*fitnessFunction)(T_GENOTYPE<T_GENE>), const double selectionPercentage);
	
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
	 * Creates a simple crossover pattern of length geneCount and a single crossover point, before which the crossover pattern will be filled
	 * with startValue and after which the crossover pattern will be filled with !startValue.
	 *
	 * Formally:
	 * crossoverPoint <= 0              the entire vector will be filled with !startValue
	 * crossoverPoint >= geneCount      the entire vector will be filled with startValue
	 * 0 < crossoverPoint < geneCount   elements [0;crossoverPoint> will be filled with startValue, elements [crossoverPoint;geneCount> will be
	 *                                  filled with !startValue
	 * 
	 * Parameters:
	 * geneCount The size of the required vector<bool>. Has to be greater than zero.
	 * crossoverPoint The first index where !startValue is to be used rather than startValue.
	 * startValue The value to start filling the crossover pattern with until index crossoverPoint is reached.
	 *
	 * Returns:
	 * A crossover pattern as described above.
	 */
	virtual std::vector<bool> _createSinglePointCrossover(const unsigned int geneCount, const unsigned int crossoverPoint, const bool startValue);
	
	
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
	double (*_fitnessFunction)(T_GENOTYPE<T_GENE>);
	
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
 */
template<typename T_GENE>
class Genotype
{
	public:
	/**
	 * Creates a Genotype for the given genes, storing the mutation function provided for its reproduction.
	 */
	Genotype(const std::vector<T_GENE>& genes, T_GENE (*mutationFunction)(T_GENE));
	
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
	 * Retrieves the gene at the given index.
	 * Valid indices are [0;n> where n is the number of genes in this genotype.
	 */
	virtual T_GENE getGeneAt(unsigned int index) const;
	
	/**
	 * Returns a copy of the vector containing all of this Genotype's genes.
	 */
	virtual std::vector<T_GENE> getAllGenes() const;
	
	/**
	 * Creates and returns a genotype representing the child of this genotype
	 * and the given other parent, using material from both parents depending
	 * on the crossover pattern provided.
	 *
	 * Parameters:
	 * otherParent the genetic information to merge with this genotype in order to produce a child
	 * crossoverPattern a vector indicating which parent to take each gene from. For any gene index, a value of false means using this genotype's gene, a value of true means using the other parent's gene.
	 * geneMutationProbabilities indicates the individual mutation rate for each gene. Each value has to be in range [0;1], where 0 means no chance of mutation, and 1 means guaranteed mutation.
	 *
	 * Exceptions:
	 * std::invalid_argument if otherParent doesn't have the same number of genes as this genotype.
	 */
	virtual Genotype crossoverWith(const Genotype& otherParent, const std::vector<bool>& crossoverPattern, const std::vector<double>& geneMutationProbabilities);
	
	protected:
	/**
	 * Datastructure to hold the gene data.
	 */
	std::vector<T_GENE> _genes;
	
	/**
	 * The gene mutation function provided for this Genotype object.
	 * Takes a gene as input and returns the mutated version of that gene.
	 */
	T_GENE (*_mutationFunction)(T_GENE);
};

#endif /* GENETIC_ALGORITHM_H */


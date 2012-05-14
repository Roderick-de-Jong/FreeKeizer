#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <math.h>
#include <stdexcept>

#include "GeneticAlgorithm.h"

using namespace std;

char createRandomLetter(char gene = ' ')
{
	gene = 'a' + char(rand() % 26); // not very random, but good enough for this purpose
	return gene;
}

// Defines fitness roughly as "how well sorted (ascending) are the characters?"
double calculateFitness(Genotype<char> genotype)
{
	int fitness = 0;
	for(unsigned int i = 0; i < genotype.getGeneCount() - 1; i++)
	{
		if(genotype.getGeneAt(i) < genotype.getGeneAt(i + 1))
			fitness++;
	}
	return double(fitness) / genotype.getGeneCount();
}

void printPopulation(const vector<Genotype<char> >& population, const vector<double>& fitness)
{
	if(population.size() != fitness.size())
		throw invalid_argument("Fitness data doesn't match population size.");
	
	cout << "POPULATION:" << endl;
	for(unsigned int p = 0; p < population.size(); p++)
	{
		vector<char> genes = population[p].getAllGenes();
		cout << "Individual: [";
		for(unsigned int g = 0; g < genes.size(); g++)
		{
			if(g > 0)
				cout << ' ';
			cout << genes[g];
		}
		cout << "], fitness: " << fitness[p] << endl;
	}
}

void printPopulationStatistics(const vector<Genotype<char> >& population, const vector<double>& fitness)
{
	double totalFitness = 0.0;
	for(unsigned int p = 0; p < population.size(); p++)
		totalFitness += fitness[p];
	double averageFitness = totalFitness / population.size();

	double maxFitness = *max_element(fitness.begin(), fitness.end());
	double minFitness = *min_element(fitness.begin(), fitness.end());

	double variance = 0.0;
	for(unsigned int p = 0; p < population.size(); p++)
	{
		double difference = fitness[p] - averageFitness;
		variance += difference * difference;
	}
	double sigma = sqrt(variance);
	
	cout << "Statistics:" << endl
	     << "Average fitness: " << averageFitness << "     Std.Dev.: " << sigma << endl
	     << "Min/max fitness: " << minFitness << "   /   " << maxFitness << endl;
}

int main(void)
{
	GeneticAlgorithm<Genotype, char> ga;
	int genotypeLength = 12;
	int populationSize = 20;
	double selectionPercentage = 0.30;
	int maxGeneration = 10;
	vector<Genotype<char> > initialPopulation;

	cout << "Seeding initial population..." << endl;
	srand(time(NULL));
	for(int i = 0; i < populationSize; i++)
	{
		vector<char> genes;
		for(int g = 0; g < genotypeLength; g++)
		{
			char c = createRandomLetter();
			genes.push_back(c);
		}
		Genotype<char> individual = Genotype<char>(genes, &createRandomLetter);
		initialPopulation.push_back(individual);
	}
	ga.seed(initialPopulation, &calculateFitness, selectionPercentage);

	cout << "Starting genetic algorithm..." << endl;;
	int generation;
	for(generation = 0; generation < maxGeneration; generation++)
	{
	  cout << "GENERATION " << generation << ":" << endl;
		ga.calculateFitnessValues();
		vector<Genotype<char> > population = ga.getPopulation();
		vector<double> populationFitness = ga.getPopulationFitness();
		printPopulation(population, populationFitness);
		printPopulationStatistics(population, populationFitness);
		cout << "Creating next generation..." << endl;
		ga.createNextGeneration();
	}
	
	cout << endl
	     << "Evolution complete." << endl
	     << "FINAL POPULATION (generation " << generation << "):" << endl;
	
	vector<Genotype<char> > population = ga.getPopulation();
	ga.calculateFitnessValues();
	vector<double> populationFitness = ga.getPopulationFitness();
	printPopulation(population, populationFitness);
	printPopulationStatistics(population, populationFitness);
	
	return 0;
}


// Import template implementation, for lack of EXPORT keyword support
// SHOULD NO LONGER BE NECESSARY BECAUSE OF GeneticAlgorithmTemplInst.h !!
//#include "GeneticAlgorithm.cpp"



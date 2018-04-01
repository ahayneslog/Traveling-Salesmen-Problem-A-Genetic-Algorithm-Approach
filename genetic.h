#ifndef _genetic_h
#define _genetic_h

/**
 * The genetic algorithm function will take in the number of cities that are in this 
 * local search problem, the starting city in this 'n' range of cities, the number
 * of individuals in each generation, the number of generation to run, and the percentage
 * chance of mutation for each generation. 
 */
void geneticAlgorithm(int, int, int, int, double);

/**
 * The matrix grabs data from the cityweights.txt file 
 * and creates a table of the data for the tour (graph) to use
 * as weights (distance) between each city. 
 */
void createMatrix(void);

/**
 * Evaluation is as the name implies. It will
 * evaluate each individual in a generation and the fitness function
 * will determine the elites. 
 */
void evaluation(void);

/**
 * The fitness function evaluate the current individual and determine
 * if it is worthy of being an elite or not. 
 */
void fitnessFunction(int *);

/**
 * It will initialize the population of the first generation depending on
 * the start city the user specifies and the number of individuals is created
 * as specified from the user, as well. 
 */
void initializePopulation(void);

//frees up the matrix of data
void freeMatrix(void);

/**
 * Regeneration takes in the generation and determine who gets mutated
 * or not with the value specified from the user! (mutation rate is the value)
 */
void regeneration(void);

#endif

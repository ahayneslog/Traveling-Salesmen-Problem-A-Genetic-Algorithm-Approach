/*************************************************************** 
  Student Name: Andrew Haynes
  File Name: genetic.c
  Assignment number: Project 3

  Other comments regarding the file - This is the genetic algorithm file for the brute force and genetic algorithm project. 
***************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "genetic.h"

/**
 * GLOBALS
 */
double **matrix; ////Graph Matrix
int numOfCities; //number of cities in tours
int startCity; //start city of tours
int *eliteOne; //optimal path one
int *eliteTwo; //optimal path two
double eliteOneScore = 0; //score for path one
double eliteTwoScore = 0; //score for path two
int individuals; //number of individuals in a generation
int generations; //number of generations to occur (fixed termination of generator)
int preemp = 0; //preemp variable is used for initialization of eliteOne and eliteTwo
int **currentGeneration; //currentGeneration
int **newGeneration; //new Generation
double mutationRate; //mutation rate of new mutation

/**
 * The genetic algorithm function will take in the number of cities that are in this 
 * local search problem, the starting city in this 'n' range of cities, the number
 * of individuals in each generation, the number of generation to run, and the percentage
 * chance of mutation for each generation. 
 */
void geneticAlgorithm(int cities, int sCity, int ind, int generation, double mutation)
{
	createMatrix();
	numOfCities = cities;
	startCity = sCity;
	individuals = ind;
	generations = generation;
	mutationRate = mutation; //needs to be within 0 to 1 (ex: 0.2)
	
	/**
	 * Timer creation starts here
         */
	struct timeval *t;
	struct timeval *t1;
	struct timezone *z;

	t = (struct timeval *)malloc(sizeof(struct timeval));
	t1 = (struct timeval *)malloc(sizeof(struct timeval));
	z =(struct timezone *)malloc(sizeof(struct timezone));	
	/**
	 * Timer creation ends here
	 */
	
	//start of timer
	gettimeofday(t,z);
	t1->tv_sec = t->tv_sec;
	t1->tv_usec = t->tv_usec;

	initializePopulation();
	int ibc = 0;
	while(ibc < generations)
	{
		evaluation();	
		regeneration();
		ibc++;	
	}
	//end of timer
	gettimeofday(t, z);

	//print results here
	int i; 
	printf("GENETIC ALGORITHM: \n");
	printf("Number of Cities: %d\n", numOfCities);
	printf("Start City: %d\n", startCity);
	printf("Number of individuals: %d\n", individuals);
	printf("Number of generations: %d\n", generations);
	printf("Mutation Rate: %lf\n", mutationRate);
	printf("OPTIMAL PATH [elite one]: ");
	for(i = 0; i < numOfCities+1; i++)
		printf("[%i] ", eliteOne[i]);
	printf("\n");
	printf("FITNESS SCORE: [%lf]\n", eliteOneScore);
	printf("OPTIMAL PATH [elite two]: ");	
	for(i = 0; i < numOfCities+1; i++)
		printf("[%i] ", eliteTwo[i]);
	printf("\n");
	printf("FITNESS SCORE: [%lf]\n", eliteTwoScore);
	printf("Seconds Elapsed: %d \t", (t->tv_sec - t1->tv_sec));
	printf("Microseconds elapsed: %d \n", (t->tv_usec - t1->tv_usec));		
	freeMatrix();
	
	int dc; 
	for(dc = 0; dc < individuals; dc++)
	{
		free(currentGeneration[dc]);
		free(newGeneration[dc]);	
	}
	free(currentGeneration);
	free(newGeneration);
}

/**
 * The matrix grabs data from the cityweights.txt file 
 * and creates a table of the data for the tour (graph) to use
 * as weights (distance) between each city. 
 */
void createMatrix(void)
{
	//THIS TEST FILE FILLS THE MATRIX CORRECTLY WITH THE FILE SCANNER
	FILE *weights;
	weights = fopen("cityweights.txt", "r");
	if(weights == NULL)
		exit(1);

	int a, i, j; 

	//2-D array to hold the graph matrix
	matrix = (double **)malloc(sizeof(double *) * 20);
	for(i = 0; i < 20; i++)
		matrix[i] = (double *)malloc(sizeof(double *) * 20);
	
	//fills the array up with 0s, useful!
	for(i = 0; i < 20; i++)
	{
		for(j = 0; j < 20; j++)
			matrix[i][j] = 0;
	}

	//fills matrix appropriately
	for(a = 0; a < 20; a++)
	{
		 for(i = a + 1; i < 20; i++) 
		 {
			fscanf(weights, "%lf", &matrix[a][i]);
			matrix[i][a] = matrix[a][i];
		 }	
	}
	fclose(weights);
}

//frees up the matrix of data
void freeMatrix(void)
{
	int i; 
	for(i = 0; i < 20; i++)
		free(matrix[i]);
	free(matrix);
}

/**
 * It will initialize the population of the first generation depending on
 * the start city the user specifies and the number of individuals is created
 * as specified from the user, as well. 
 */
void initializePopulation()
{
	currentGeneration = (int **)malloc(sizeof(int *) * (individuals+1)); 
  	newGeneration = (int **)malloc(sizeof(int *) * (individuals+1));
	eliteOne = (int *)malloc(sizeof(int) * numOfCities + 1);
	eliteTwo = (int *)malloc(sizeof(int) * numOfCities + 1);	
	int i;
	for(i = 0; i < individuals; ++i)
	{
		currentGeneration[i] = (int *)malloc(sizeof(int) * (numOfCities+1));
		newGeneration[i] = (int *)malloc(sizeof(int) * (numOfCities+1));
	}


	//first individual that will contain 0 to n cities, Ex: 0 1 2 3 0
	int a, b;
	int counter = 1;
	currentGeneration[0][0] = 0;
	currentGeneration[0][numOfCities] = 0;
	for(a = 1; a < numOfCities; a++)
	{
		currentGeneration[0][a] = counter;
		counter++;
	}
	
	//If the start city is not 0, find where start city is from the original tour
	//and switch it around to allow permutation and mutation to work correctly
	if(startCity != 0)
	{
		int temp;
		for(a = 1; a < numOfCities; a++)
		{
			if(currentGeneration[0][a] == startCity)
			{
				temp = a;		
			}
		} 
		currentGeneration[0][0] = startCity;
		currentGeneration[0][temp] = 0;
		currentGeneration[0][numOfCities] = startCity;
	}

	//Copies first individuals to the rest, BREAKS RIGHT HERE WHEN INDIVIDUALS > CITIES
	for(a = 0; a < individuals-1; a++)
	{
		for(b = 0; b < numOfCities+1; b++)
		{
			currentGeneration[a+1][b] = currentGeneration[a][b];
		}
	}
	
	//randomly changes cities correctly to create different permutations of the individuals!
	for(a = 1; a < individuals; a++)
	{
		for(b = 1; b < numOfCities; b++)
		{
			int locA = rand() % (numOfCities-1) + 1;  
			int temp = currentGeneration[a][b];
			currentGeneration[a][b] = currentGeneration[a][locA];
			currentGeneration[a][locA] = temp;
		}
	}
}

/**
 * Evaluation is as the name implies. It will
 * evaluate each individual in a generation and the fitness function
 * will determine the elites. 
 */
void evaluation(void)
{
	int i; 
	while( i < individuals)
	{		
		fitnessFunction(currentGeneration[i]);
		i++;
	}
}

/**
 * The fitness function evaluate the current individual and determine
 * if it is worthy of being an elite or not. 
 */
void fitnessFunction(int *array)
{
	int i;
	double total = 0.0;

	int g, h;
	double *test = (double *)malloc(sizeof(double) * (numOfCities+1));
	
		
	//loop to fill in the test array
	for(g = 0, h = 1; h < numOfCities+1; g++, h++)
	{
		//this statements grabs data from the matrix and puts it in
		//the city numbers are used to grab the correct weight between the cities
		//from the array
		test[g] = matrix[ (array[g]) ][ (array[h]) ];  		
	} 
	
	for(i = 0; i < numOfCities; i++)
	{
		total += test[i]; //accumulation of weights for fitness score
	}

	/** 
	 * Pre-emptive filling of the two elites. 
	 * First run through gets Elite One. 
	 * Second run through gets Elite Two. 
	 */ 
	if(preemp == 0)
	{
		eliteOneScore = total;
		for(i = 0; i < numOfCities+1; i++)
		{	
			eliteOne[i] = array[i];		
		}
		preemp++;
	}
	else if(preemp == 1)
	{
		eliteTwoScore = total;
		for(i = 0; i < numOfCities+1; i++)
		{	
			eliteTwo[i] = array[i];		
		}
		preemp++;	
	}


	/*****
	 * The actual scoring. Since we're using the elite method, 
         * the scoring is pretty straight forward in this GA Evaluation.
	 * EliteOne will be the lowest, and EliteTwo will be the second lowest. 
	 */

	if(total < eliteOneScore)
	{
		eliteOneScore = total;
		for(i = 0; i < numOfCities+1; i++)
		{	
			eliteOne[i] = array[i];		
		}	

	}
	else if((total > eliteOneScore) && (total < eliteTwoScore))
	{
		eliteTwoScore = total;
		for(i = 0; i < numOfCities+1; i++)
		{	
			eliteTwo[i] = array[i];		
		}
	}
}	

/**
 * Regeneration takes in the generation and determine who gets mutated
 * or not with the value specified from the user! (mutation rate is the value)
 */
void regeneration(void)
{
	//copy old generation into new
	int a, b;
	int mutateThisGene = 0;
	for(a = 0; a < individuals; a++)
	{
		for(b = 0; b < numOfCities+1; b++)
			newGeneration[a][b] = currentGeneration[a][b];
	}

	//Critical Area that imploys GA, uses mutation
	for(a = 0; a < individuals; a++)
	{
		for(b = 1; b < numOfCities; b++)
		{
			//MUTATION: randomly swapping two cities with a random location
			mutateThisGene = rand() % (int)(1.0 / mutationRate);			
			if(mutateThisGene == 0)
			{
				//range of 1 to n-1; so that we don't touch the start
				//and end city of the tour
				int locA = rand() % (numOfCities-1) +1;  
				int temp = newGeneration[a][b];
				newGeneration[a][b] = newGeneration[a][locA];
				newGeneration[a][locA] = temp;
			}
		}
	}

	//copy new generation back into old
	for(a = 0; a < individuals; a++)
	{
		for(b = 0; b < numOfCities+1; b++)
		{
			currentGeneration[a][b] = newGeneration[a][b];
		}
	}
}

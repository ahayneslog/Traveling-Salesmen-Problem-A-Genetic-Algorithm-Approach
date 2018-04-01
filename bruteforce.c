/*************************************************************** 
  Student Name: Andrew Haynes
  File Name: bruteforce.c
  Assignment number: Project 3

  Other comments regarding the file - This is the brute force algorithm file for the brute force and genetic algorithm project. 
***************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "bruteforce.h"
/**
 * GLOBALS
 */

//fitness Score
double fitnessScore = 9999999999;
//matrix
double **matrix;
//Number of Cities
int numOfCities;
//optimal path
int *optimalPath;

//The entire implementation of the brute force search algorithm
void bruteForce(int cities, int sCity)
{
	//creates a 20x20 matrix graph
	createMatrix();	
	numOfCities = cities; //number of cities in the tour
	//need to allocate memory for the array holder
	int *currentGeneration; //2-D array to hold the first tour
	currentGeneration = (int *)malloc(sizeof(int) * (numOfCities+1));
	//allocated memory for optimal path
	optimalPath = (int *)malloc(sizeof(int) * (numOfCities+1));
	int startCity = sCity; //start city for the tour
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

	//start timer here
	gettimeofday(t,z);
	t1->tv_sec = t->tv_sec;

	//BRUTE FORCE STARTS HERE
	tourStartCity(startCity, currentGeneration);
	permutations(numOfCities, currentGeneration); 

	//end timer here
	gettimeofday(t, z);

	//PRINT RESULTS
	printf("BRUTE FORCE: \n");
	printf("Number of Cities: %d\n", numOfCities);
	printf("Start City: %d\n", startCity);
	printf("OPTIMAL PATH: ");
	int c;
	for(c = 0; c < numOfCities+1; c++)
		printf("[%i] ", optimalPath[c]);
	printf("\n");
	printf("FITNESS SCORE: [%lf]\n", fitnessScore);	
	printf("Seconds Elapsed: %d \n", (t->tv_sec - t1->tv_sec));

	//free every allocated memory
	free(t);
	free(t1);
	free(z);
	free(optimalPath);
	free(currentGeneration);
	freeMatrix();
}

/**
 * The tourStartCity takes in a number that will be called 'n', 
 * and make it the start and end city of the tour! Please
 * put in the array you wish to make a tour out of with 
 * the start city. 
 */
void tourStartCity(int n, int *array)
{
	int i;
	int counter = 0;
	array[0] = n;
	array[numOfCities] = n;
	for(i = 1; i < numOfCities; i++)
	{
		if(counter == n)
		{
			counter++;
			array[i] = counter;
			counter++;
		}
		else
		{
			array[i] = counter;
			counter++;
		}
	}
}

/**
 * n is number of cities in tour, array is the generation, and this function creates
 * all possible permutations of the current tour, hence a brute force search algorithm.
 * WARNING: If you make 'n' big, it will take awhile for the algorithm to finish. 
 */
void permutations(int n, int *array)
{
	if(n == 1)
	{
		bruteFitnessEval(array);	
	}
	else
	{
		int a;
		for(a = 0; a < n-1; a++)
		{
			int temp = array[a+1];
			array[a+1] = array[n-1];
			array[n-1] = temp;
			permutations(n-1, array);
			int temp1 = array[a+1];
			array[a+1] = array[n-1];
			array[n-1] = temp1;
		}
	}
}	

/**
 * This function evaluates each permutation and determine if it is optimal 
 * or not. If it is, it will become the new optimal path and the score will be recorded. 
 */
void bruteFitnessEval(int *array)
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
	if(total < fitnessScore)
	{
		fitnessScore = total;
		//Optimal Path is also found here!
		for(i = 0; i < numOfCities+1; i++)
			optimalPath[i] = array[i];
	}
	free(test);
}

/**
 * The matrix grabs data from the cityweights.txt file 
 * and creates a table of the data for the tour (graph) to use
 * as weights (distance) between each city. 
 */
void createTheMatrix(void)
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
void freeTheMatrix(void)
{
	int i; 
	for(i = 0; i < 20; i++)
		free(matrix[i]);
	free(matrix);
}

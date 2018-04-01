#ifndef _bruteforce_h
#define _bruteforce_h

//The entire implementation of the brute force search algorithm
void bruteForce(int, int);

/**
 * The tourStartCity takes in a number that will be called 'n', 
 * and make it the start and end city of the tour! Please
 * put in the array you wish to make a tour out of with 
 * the start city. 
 */
void tourStartCity(int, int *);

/**
 * n is number of cities in tour, array is the generation, and this function creates
 * all possible permutations of the current tour, hence a brute force search algorithm.
 * WARNING: If you make 'n' big, it will take awhile for the algorithm to finish. 
 */
void permutations(int, int *);

/**
 * This function evaluates each permutation and determine if it is optimal 
 * or not. If it is, it will become the new optimal path and the score will be recorded. 
 */
void bruteFitnessEval(int *);

/**
 * The matrix grabs data from the cityweights.txt file 
 * and creates a table of the data for the tour (graph) to use
 * as weights (distance) between each city. 
 */
void createTheMatrix(void);

//frees up the matrix of data	
void freeTheMatrix(void);

#endif


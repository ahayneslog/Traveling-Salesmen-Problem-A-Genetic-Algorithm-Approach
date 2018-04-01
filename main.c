/*************************************************************** 
  Student Name: Andrew Haynes
  File Name: main.c
  Assignment number: Project 3

  Other comments regarding the file - This is the main file for the brute force and genetic algorithm project where we make a call to both algorithms and show the results for us to compare!
***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "bruteforce.h"
#include "genetic.h"


int main()
{
	printf("Welcome to the TSP Generator. Please select your choice of interest.\n");
	printf("1: Brute Force Algorithm\n");
	printf("2: Genetic Algorithm\n");
	printf("3: Brute Force and Genetic\n");
	int choice;
	int cities;
	int startCity;
	int gen;
	int individuals;
	double mutationRate;

	scanf("%d", &choice);
	if(choice == 1)
	{
		printf("You chose the Brute Force Algorithm.\n");
		printf("How many cities are in the current run?\n");
		scanf("%d", &cities);
		printf("Pick your starting city (Must be less than number of cities)!\n");
		scanf("%d", &startCity); //don't forget to implement a check for this!
		if(startCity > cities)
		{
			printf("Start city is out of bound. Try again...\n");
			exit(1);
		}
		bruteForce(cities, startCity);	
	}
	else if(choice == 2)
	{
		printf("You chose the Genetic Algorithm.\n");
		printf("How many cities are in the current run?\n");
		scanf("%d", &cities);
		printf("Pick your starting city (Must be less than number of cities)!\n");
		scanf("%d", &startCity); //implement a check for this!
		if(startCity > cities)
		{
			printf("Start city is out of bound. Try again...\n");
			exit(1);
		}		
		printf("How many generations to run?\n");
		scanf("%d", &gen);
		printf("How many individuals should be in a generation?\n");
		scanf("%d", &individuals);
		printf("Mutation Rate?\n");
		scanf("%lf", &mutationRate);
		geneticAlgorithm(cities, startCity, individuals, gen, mutationRate);
	}
	else if(choice == 3)
	{
		printf("This run will show the comparison between Brute Force\n"
			"and Genetic Algorithm by running them both with the\n"
			"parameters you give.\n");	
		printf("How many cities are in the current run?\n");
		scanf("%d", &cities);
		printf("Pick your starting city (Must be less than number of cities)!\n");
		scanf("%d", &startCity); 
		if(startCity > cities)
		{
			printf("Start city is out of bound. Try again...\n");
			exit(1);
		}
		printf("How many generations to run?\n");
		scanf("%d", &gen);
		printf("How many individuals should be in a generation?\n");
		scanf("%d", &individuals);
		printf("Mutation Rate?\n");
		scanf("%lf", &mutationRate);
		bruteForce(cities, startCity);
		geneticAlgorithm(cities, startCity, individuals, gen, mutationRate);
	}
	else
	{
		printf("Incorrect input.\n");
	}
	return 0;
}

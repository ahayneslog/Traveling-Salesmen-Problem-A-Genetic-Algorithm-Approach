TSP : main.o genetic.o bruteforce.o 
	gcc -o TSP main.o genetic.o bruteforce.o

genetic.o : genetic.c
	gcc -c genetic.c
bruteforce.o : bruteforce.c
	gcc -c bruteforce.c
main.o : main.c
	gcc -c main.c

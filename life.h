#include<cilk/cilk.h>
#include<iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<numeric>
#include <ctime>
#include "example_util_gettime.h"


int cellValue(int i, int j, int size, int * arr);
int numNeighbors(int i, int j, int size, int * arr);
void printArray(int * arr, int size);
void readlife(int *a,unsigned int n, char *filename);
void genlife(int *a,unsigned int n);
void life(int *a,unsigned int n, unsigned int iter, int *livecount);
int countlive(int *a, unsigned int n);

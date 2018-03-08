/**
 * Assignment 5: Life in Cilk
 * Team Member 1 Name:
 * Team Member 2 Name: 
 *
 */

#include "life.h"
#include <time.h>
#include <cilk/reducer_opadd.h>

//Generate the life matrix any way you want. We would highly recommend that you print the generated
//matrix into a file, so that you can share it with other teams for checking correctness.

static int COARSENESS = 10;
static const int OCCUPIED_VALUE = 10;

void updateNeighborsAlive(cilk::reducer< cilk::op_add<int> > neighbors[], int x, int y, int n);
int* updateNeighborsArray(cilk::reducer< cilk::op_add<int> >  neighbors[], int n);
void addToCell(cilk::reducer< cilk::op_add<int> > a[], int x, int y, int n, int val);
void setCell(cilk::reducer< cilk::op_add<int> > a[], int x, int y, int n, int val);
cilk::reducer< cilk::op_add<int> >* getCellPtr(cilk::reducer< cilk::op_add<int> > a[],int x, int y, int n);
int countLiving(int* array);


void genlife(int *a, unsigned int n)
{
    int nSquaredDivCoarseness = n*n/ COARSENESS;
    std::cout << "test thing is " << n << std::endl;

    cilk_for(int count = 0; count < nSquaredDivCoarseness; ++count){
        for(int count2 = 0; count2 < COARSENESS; ++count2 ){

                std::cout << count * COARSENESS + count2 << std::endl;
             a[((count * COARSENESS) + count2)] = rand() % 2;

        }

    }
    std::cout<< "genLifeDone";

}

//Read the life matrix from a file

void readlife(int *a, unsigned int n, char *filename)
{
    FILE * pFile;
    pFile = fopen (filename,"r");

    int nextItem = 0;

for (int outerCounter = 0; outerCounter < n; ++outerCounter) {
    for (int innerCounter = 0; innerCounter < n; ++innerCounter) {
        fscanf(pFile, "%i", &nextItem);
        a[innerCounter * n +outerCounter] = nextItem;
    }
}




}

//Life function
void life(int *a, unsigned int n, unsigned int iter, int *livecount)
{

    int nSquaredDivCoarseness = n*n/COARSENESS;

livecount = new int [10];
    int spotInLiveCount = 0;

    for(int iterCount = 0; iterCount < iter; ++iterCount) {
        cilk::reducer< cilk::op_add<int> > neighbors[n*n];
        //setting neighbors array to Zero.
        cilk_for(int count = 0; count < nSquaredDivCoarseness; ++count){
            for (int count2 = 0; count2 < COARSENESS; ++count2) {
                (neighbors[((count * COARSENESS) + count2) ]).set_value(0);
            }
        }
/*
        cilk_for(int count = 0; count < nSquaredDivCoarseness; ++count){
            for (int count2 = 0; count2 < COARSENESS; ++count2) {

                for (int yCount = 0; yCount < n; yCount++) {

                    if ((a[n * ((count * COARSENESS) + count2) + yCount]) == 1) {
                        updateNeighborsAlive(neighbors, (count * COARSENESS) + count2, yCount, n);
                    }
                }
            }
        }
        int * newA;
        newA = updateNeighborsArray(neighbors, n);
        delete a;




*/
        #if DEBUG == 1
        if(iterCount % (iter / 10)-1 == 0 && iterCount != 0 ){
           livecount[spotInLiveCount]= countLiving(a);
            ++spotInLiveCount;
        }
        #endif

    }
    // You need to store the total number of livecounts for every 1/0th of the total iterations into the livecount array.
	// For example, if there are 50 iterations in your code, you need to store the livecount for iteration number 5 10 15
	// 20 ... 50. The countlive function is defined in life.cpp, which you can use. Note that you can
	// do the debugging only if the number of iterations is a multiple of 10.
	// Furthermore, you will need to wrap your counting code inside the wrapper #if DEBUG == 1 .. #endif to remove
	// it during performance evaluation.
	// For example, your code in this function could look like -
	//
	//
	//	for(each iteration)
	//      {
	//
	//		Calculate_next_life();
	//
	//		#if DEBUG == 1
	//		  if_current_iteration == debug_iteration
	//		  total_lives = countlive();
	//		  Store_into_livecount(total_lives);
	//		#ENDIF
	//
	//	}
}
int countLiving(int array[]){
    int aliveCount = 0;
    int n = sizeof(array)/ sizeof(int);
    for(int count =0; count < n*n; ++count){
        if(array[count] == 1){
            aliveCount +=1;
        }

    }
    std::cout << aliveCount << std::endl;
return aliveCount;
}


void updateNeighborsAlive(cilk::reducer< cilk::op_add<int> > neighbors[], int x, int y, int n){

    // Diagonal cases
    addToCell(neighbors,x+1,y+1,n,1);
    addToCell(neighbors,x+1,y-1,n,1);
    addToCell(neighbors,x-1,y+1,n,1);
    addToCell(neighbors,x-1,y+1,n,1);

    // Middle spots
    addToCell(neighbors,x+1,y,n,1);
    addToCell(neighbors,x,y+1,n,1);
    addToCell(neighbors,x-1,y,n,1);
    addToCell(neighbors,x,y-1,n,1);

    // Update x, y cuz is occupied
    addToCell(neighbors,x,y,n,OCCUPIED_VALUE);

}



void setCell(cilk::reducer< cilk::op_add<int> > a[], int x, int y, int n, int val){
    cilk::reducer< cilk::op_add<int> > * ptr = getCellPtr(a,x,y,n);
    ptr->set_value(val);
}


void addToCell(cilk::reducer< cilk::op_add<int> > a[], int x, int y, int n, int val){
    cilk::reducer< cilk::op_add<int> > *ptr = getCellPtr(a,x,y,n);
    ptr += val;
}



cilk::reducer< cilk::op_add<int> >* getCellPtr(cilk::reducer< cilk::op_add<int> > a[],int x, int y, int n){
    if(x < 0){
        x = n-1;
    }
    else if( x >= n){
        x = 0;
    }

    if(y < 0){
        y = n-1;
    }
    else if (y >= n){
        y = 0;
    }

    cilk::reducer< cilk::op_add<int> > * ptr;
    ptr = &a[x*n + y];


    return ptr;

}



int* updateNeighborsArray(cilk::reducer< cilk::op_add<int> >  neighbors[], int n){
    int nSquaredDivCoarseness = n*n/COARSENESS;
    int nDivCoarseness = n/COARSENESS;
    int* value;
    value = new int [n*n];
    int * ptr;

    cilk_for(int count = 0; count < nSquaredDivCoarseness; ++count){
        for(int count2 = 0; count2 < COARSENESS; ++count2 ){

            for (int yCount = 0; yCount < n ; yCount ++) {

                *ptr = neighbors[n * ((count * COARSENESS) + count2) + yCount].get_value();
                if (*ptr >= OCCUPIED_VALUE){
                    *value -= OCCUPIED_VALUE;

                    if( *ptr ==2 || *value == 3){
                        *value = 1;
                    }
                    else if(*ptr > 3 || *value < 2){
                        *value = 0;

                    }

                } else{
                    if (*ptr == 3){
                        *value = 1;

                } else {
                        *value = 0;
                    }

                }

            }

        }
    }

}

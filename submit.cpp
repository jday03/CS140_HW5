/**
 * Assignment 5: Life in Cilk
 * Team Member 1 Name: Jonathan Day
 * Team Member 2 Name: Noel Vargas
 *
 */

#include "life.h"
#include <time.h>
#include <cilk/reducer_opadd.h>

//Generate the life matrix any way you want. We would highly recommend that you print the generated
//matrix into a file, so that you can share it with other teams for checking correctness.

static int COARSENESS = 25;
static const int OCCUPIED_VALUE = 10;

void updateNeighborsAlive(cilk::reducer< cilk::op_add<int> > neighbors[], int x, int y, int n);
int* updateNeighborsArray(cilk::reducer< cilk::op_add<int> >  neighbors[], int n);
void addToCell(cilk::reducer< cilk::op_add<int> > a[], int x, int y, int n, int val);
void setCell(cilk::reducer< cilk::op_add<int> > a[], int x, int y, int n, int val);
cilk::reducer< cilk::op_add<int> >* getCellPtr(cilk::reducer< cilk::op_add<int> > a[],int x, int y, int n);
void printGrid(int* a, int n);
int countAlive(int *a, unsigned int n);
void printGrid(cilk::reducer< cilk::op_add<int> >  a [], int n);


void genlife(int *a, unsigned int n)
{

    cilk_for(int count = 0; count < n/COARSENESS; ++count){
        for (int xCount = count * COARSENESS; xCount < (count + 1) * COARSENESS; ++xCount)
            for (int yCount = 0; yCount < n; ++yCount) {
                a[xCount * n + yCount] = rand() % 2;
            }
    }





}

//Read the life matrix from a file

void readlife(int *a, unsigned int n, char *filename) {
    FILE *pFile;
    pFile = fopen(filename, "r");

    int nextItem = 0;

    for(int count = 0; count < n/COARSENESS; ++count){
        for (int xCount = count * COARSENESS; xCount < (count + 1) * COARSENESS; ++xCount)
            for (int yCount = 0; yCount < n; ++yCount) {
                fscanf(pFile, "%i", &nextItem);
                a[xCount * n + yCount] = nextItem;
            }
    }
}





//Life function
void life(int *a, unsigned int n, unsigned int iter, int *livecount) {


    int nSquaredDivCoarseness = n * n / COARSENESS;
    int nDivCoarseness = n / COARSENESS;

    int spotInLiveCount = 1;
    int *newA;
   // printGrid(a, n);

    for (int iterCount = 0; iterCount < iter; ++iterCount) {
        cilk::reducer <cilk::op_add <int> > neighbors[n * n];
        //setting neighbors array to Zero and copying over newA to a.

        cilk_for(int count = 0; count < nDivCoarseness; ++count){
            for (int xCount = count * COARSENESS; xCount < (count + 1) * COARSENESS; ++xCount)
                for (int yCount = 0; yCount < n; ++yCount) {
                    (neighbors[xCount * n + yCount]).set_value(0);


                }
        }

        delete[] newA;
        int *newA;

        cilk_for(int count = 0; count < nDivCoarseness; ++count){
            for (int xCount = count * COARSENESS; xCount < (count + 1) * COARSENESS; ++xCount)
                for (int yCount = 0; yCount < n; ++yCount) {

                    if (a[xCount * n + yCount] == 1) {

                        updateNeighborsAlive(neighbors, xCount, yCount, n);
                    }
                }
        }
        newA = updateNeighborsArray(neighbors, n);
        // delete a;

        cilk_for(int count = 0; count < nDivCoarseness; ++count){
            for (int xCount = count * COARSENESS; xCount < (count + 1) * COARSENESS; ++xCount)
                for (int yCount = 0; yCount < n; ++yCount) {
                    a[xCount * n + yCount] = newA[xCount * n + yCount];
                }
        }


        #if DEBUG == 1

        if((iterCount == spotInLiveCount * iter/10 - 1 )||iter <= 10){
           int summation= countlive(a,n);
          // std::cout << "SUMMATION IS: " << summation << std::endl;
           livecount[spotInLiveCount - 1] = summation;
            //std::cout << "ITERCOUNT = " << iterCount << "... count is: livecount[" << spotInLiveCount << "] = " << livecount[spotInLiveCount -1]<<std::endl;
            ++spotInLiveCount;

        }


       #endif

       // printGrid(a, n);

    }

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



/*int countLiving(int* array, int n){

    cilk::reducer< cilk::op_add<int> > aliveCount;
    aliveCount.set_value(0);
std::cout<< "START" << std::endl;
    cilk_for(int count = 0; count < n*n/COARSENESS; ++count){
        for (int count2 = 0; count2 < COARSENESS; ++count2) {
            std::cout << count * COARSENESS + count2 << std::endl;
            if(*(array + count * COARSENESS + count2)  == 1) {
              //  *aliveCount += 1;
            }
        }
    }






    std::cout << std::endl << aliveCount.get_value() << std::endl;
return aliveCount.get_value();
}
*/



int countAlive(int *a, unsigned int n)
{
    int sum = 0;
    for(int i = 0; i < n*n; i++)
    {
        sum += *(a+n);
    }

    return sum;
}





void updateNeighborsAlive(cilk::reducer< cilk::op_add<int> > neighbors[], int x, int y, int n){

    // Diagonal cases
    //std::cout << "Alive cell is: (" << x << " , " << y << " ) "  << std::endl;
    addToCell(neighbors,x+1,y+1,n,1);
    addToCell(neighbors,x+1,y-1,n,1);
    addToCell(neighbors,x-1,y+1,n,1);
    addToCell(neighbors,x-1,y-1,n,1);

    // Middle spots
    addToCell(neighbors,x+1,y,n,1);
    addToCell(neighbors,x,y+1,n,1);
    addToCell(neighbors,x-1,y,n,1);
    addToCell(neighbors,x,y-1,n,1);

    // Update x, y cuz is occupied
    addToCell(neighbors,x,y,n,OCCUPIED_VALUE);

}




void addToCell(cilk::reducer< cilk::op_add<int> > a[], int x, int y, int n, int val){

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

    *(a[x*n + y]) +=val;


}







int* updateNeighborsArray(cilk::reducer< cilk::op_add<int> >  neighbors[], int n) {
    int nSquaredDivCoarseness = n * n / COARSENESS;
    int nDivCoarseness = n / COARSENESS;
    int * value = new int[n*n];
    //printGrid(neighbors, n);

    for(int x = 0; x < n; x ++){
        for(int y = 0; y < n; ++y){
            value[x*n + y] = neighbors[x * n + y].get_value();
        }
    }

    cilk_for(int count = 0; count < nDivCoarseness; ++count){
        for (int xCount = count * COARSENESS; xCount < (count + 1) * COARSENESS; ++xCount) {
            for (int yCount = 0; yCount < n; ++yCount) {

                int cellVal = value[xCount*n + yCount];

                if (cellVal >= 10) {

                    if (cellVal == 12 || cellVal == 13) {
                        //value[xCount * n + yCount].set_value(1);
                        value[xCount * n + yCount] = 1;
                        //*(value + ((count * COARSENESS) + count2)) = 1;
                    } else if (cellVal > 13 || cellVal < 12) {
                        value[xCount * n + yCount] = 0;
                        //(value + ((count * COARSENESS) + count2)) = 0;
                        //value[xCount * n + yCount].set_value(0);

                    }

                } else {
                    if (cellVal == 3) {
                        //*(value + ((count * COARSENESS) + count2)) = 1;
                        value[xCount * n + yCount] = 1;

                    } else {
                        value[xCount * n + yCount] = 0;
                        //*(value + ((count * COARSENESS) + count2)) = 0;
                    }

                }

            }

        }
    }




        //std::cout << "VALUE GRID:" << std::endl;
       // printGrid(value, n);
        return value;
    }


void printGrid(int* a, int n){
std::cout << "RESULT ARRAY: " << std::endl;
    for(int count1 = 0; count1 < n; ++count1){
        for(int count2 = 0; count2 < n; ++count2){
           std::cout<< " " << a[count1* n + count2] << " ";
        }
        std::cout<< std::endl;
    }
    std::cout<< "---" << std::endl;
}




void printGrid(cilk::reducer< cilk::op_add<int> >  a [], int n){
std::cout << " NEIGHBOR ARRAY: " << std::endl;
    for(int count1 = 0; count1 < n; ++count1){
        for(int count2 = 0; count2 < n; ++count2){
            std::cout<< " " << a[count1* n + count2].get_value() << " ";
        }
        std::cout<< std::endl;
    }
    std::cout<< "---" << std::endl;
}



//CSCI 474 Project 3
//Peter Fraser, Jackson Gravel, Matthew Greatens, Tom Hoskins
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
// #include <math.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
// #include <time.h>
// #include <wait.h>
// #include <sys/time.h>

void makeRandomTextFiles();
void makeTextFileTemporalLocality();
bool assessTemporalLocality();

void calculateDifference();
int findMin();
void shortestSeekTimeFirst();
void firstInFirstOut();
void lastInFirstOut();
int ArrayContains(int arr[], int pageReference, int size);
void scanAlgorithm(int requests[], bool startDirection, int startTrack, int size);
void CScanAlgorithm(int requests[], bool startDirection, int startTrack, int size);
bool continueDirection(int arr[], int lastTrack, int size, bool currentDirection);
int nextTrackSCAN(int arr[], int lastTrack, int size, bool currentDirection);
void findNumber(int arr[], int num, int *const low, int *const high, int size);
void nStepScan(int arr[], int startVal, int size, int N, int M);

struct node {
    int distance;
    bool accessed;
};

#define SMALL 15
#define MEDIUM 30
#define LARGE 60

/* Main Function */
int main(int argc, char *argv[])
{
    //Arguments do not match requirements specified in project.
    //Exit file.
    if (argc != 1)
    {
        printf("1 argument expected in format: \n programName");
        exit(1);
    }
    int tableRequests[9] = {55,58,39,18,90,160,150,38,184};

    //Save fileName to variable for readability
    const char *p_fileName1 = "random1.txt";

    //File doesn't exist
    if (!(access(p_fileName1, F_OK) == 0))
    {
        //Generate text file
        makeRandomTextFiles(p_fileName1, 199);
    }
    //Save fileName to variable for readability
    const char *p_fileName2 = "random2.txt";

    //File doesn't exist
    if (!(access(p_fileName2, F_OK) == 0))
    {
        //Generate text file
        makeRandomTextFiles(p_fileName2, 199);
    }
    //Save fileName to variable for readability
    const char *p_fileName3 = "random3.txt";

    //File doesn't exist
    if (!(access(p_fileName3, F_OK) == 0))
    {
        //Generate text file
        makeRandomTextFiles(p_fileName3, 199);
    }
        //Save fileName to variable for readability
    const char *p_fileName4 = "random4.txt";

    //File doesn't exist
    if (!(access(p_fileName4, F_OK) == 0))
    {
        //Generate text file
        makeRandomTextFiles(p_fileName4, 199);
    }
        //Save fileName to variable for readability
    const char *p_fileName5 = "random5.txt";

    //File doesn't exist
    if (!(access(p_fileName5, F_OK) == 0))
    {
        //Generate text file
        makeRandomTextFiles(p_fileName5, 199);
    }


    shortestSeekTimeFirst(tableRequests, 100, 9);
    printf("\n------------------------------------\n");
    firstInFirstOut(tableRequests, 100, 9);
    printf("\n------------------------------------\n");
    lastInFirstOut(tableRequests, 100, 9);
    printf("\n------------------------------------\n");
    scanAlgorithm(tableRequests, true, 100, 9);
    printf("\n------------------------------------\n");
    CScanAlgorithm(tableRequests, true, 100, 9);
}

void makeRandomTextFiles(const char *p_fileName, int pages)
{
    //Intializes random number generator
    time_t t;
    srand((unsigned)time(&t));

    //Create File pointer
    FILE *p_file;

    int tempInt;

    //Open file
    p_file = fopen(p_fileName, "w");
    for (int i = 0; i < 1000; i++)
    {
        //Generates integer in range {0 , 1, 2, ... pages - 1, pages}
        tempInt = rand() % pages;

        //Write to file
        fprintf(p_file, "%d ", tempInt);
    }
    fclose(p_file);
}

void makeTextFileTemporalLocality(const char *p_fileName, int pages)
{
    //Intializes random number generator
    time_t t;
    srand((unsigned)time(&t));

    FILE *p_file;

    int tempInt;

    int locality[10];
    int *p_locality = locality;

    int index = 0;

    //small dataset
    p_file = fopen(p_fileName, "w");
    for (int i = 0; i < 1000; i++)
    {
        float temporalLocality = (float)rand() / RAND_MAX;
        if (temporalLocality < 0.3 && i > 10)
        {
            tempInt = rand() % 10;
            fprintf(p_file, "%d ", locality[tempInt]); //Ideally this saves a random number already in array
        }
        else
        {
            tempInt = rand() % pages; //Creates random integer between 0 and 14 inclusive
            p_locality[index] = tempInt;
            index = ((index + 1) % 10);
            fprintf(p_file, "%d ", tempInt);
        }
    }
    fclose(p_file);
}

bool assessTemporalLocality()
{
    time_t t;

    //Intializes random number generator
    srand((unsigned)time(&t));

    float tempFloat = (float)rand() / RAND_MAX;

    return tempFloat < 0.3;
}

void calculateDifference(int queue[], int head, struct node diff[], int requestSize) 
{
    for (int i = 0; i < requestSize; i++)
    {
        diff[i].distance = abs(queue[i] - head);
    }
}

int findMin(struct node diff[], int requestSize)
{
    int index = -1, minimum = INT_MAX;
    for (int i = 0; i < requestSize; i++)
    {
        if (!diff[i].accessed && minimum > diff[i].distance)
        {
            minimum = diff[i].distance;
            index = i;
        }
    }
    return index;
}
// SSTF - Peter Fraser
void shortestSeekTimeFirst(int request[], int head, int size)
{   
    struct node diff[size];


    for (int i = 0; i< size; i++) 
    {
        diff[i].distance = 0;
        diff[i].accessed = false;
    }

    int seekCount = 0;
    int seekDistance[size];

    int seekSequence[size];

    for (int i = 0; i < size; i++) 
    {
        seekSequence[i] = head;
        calculateDifference(request, head, diff, size);

        int index = findMin(diff, size);

        diff[index].accessed = true;

        seekCount += diff[index].distance;
        seekDistance[i] = diff[index].distance;

        head = request[index];
    }

    seekSequence[size - 1] = head;

    printf("Seeking sequence for SSTF is: \n");
    printf("Head \t|\t Distance\n");
    printf("--------------------------\n");
    for (int i = 0; i < size; i++)
    {
        printf("%d \t|\t %d\n", seekSequence[i], seekDistance[i]);
    }

    int seekAverage = seekCount / size;
    printf("--------------------------\n");
    printf("Total number of seek time for SSTF: %d \n", seekCount);
    printf("Average seek time for SSTF: %d\n\n", seekAverage);
}

//Fifo - Peter Fraser
void firstInFirstOut(int request[], int head, int size) {
    int seekCount = 0;
    int distance, currentTrack;

    printf("Seeking sequence for FIFO is: \n");
    printf("Head \t|\t Distance\n");
    printf("--------------------------\n");
    for (int i = 0; i < size; i++) {
        currentTrack = request[i];

        distance = abs(currentTrack - head);

        seekCount += distance;

        head = currentTrack;
        printf("%d \t|\t %d\n",head, distance);
    }

    int seekAverage = seekCount / size;
    printf("--------------------------\n");
    printf("Total number of seek time for FIFO: %d \n", seekCount);
    printf("Average seek time for FIFO: %d \n\n", seekAverage);
}

// LIFO - Peter Fraser
void lastInFirstOut(int request[], int head, int size) {
    int seekCount = 0;
    int distance, currentTrack;

    printf("Seeking sequence for LIFO is: \n");
    printf("Head \t|\t Distance\n");
    printf("--------------------------\n");
    for (int i = size-1; i >= 0; i-- ) {
        currentTrack = request[i];

        distance = abs(currentTrack - head);

        seekCount += distance;

        head = currentTrack;
        printf("%d \t|\t %d\n",head, distance);
    }

    int seekAverage = seekCount / size;
    printf("--------------------------\n");
    printf("Total number of seek time for LIFO: %d \n", seekCount);
    printf("Average seek time for LIFO: %d \n", seekAverage);
}


//SCAN - Tom Hoskins
void scanAlgorithm(int requests[], bool startDirection, int startTrack, int size) 
{
    bool currDirection = startDirection;
    int alreadyUsed[size];
    int currTrack = startTrack;
    int prevTrack = startTrack;
    int nextTrack, tracksTraversed;
    for (int i = 0; i < size; i++)
    {
        if (!continueDirection(requests, currTrack, size, currDirection))
        {
            currDirection = !currDirection;
        }
        if (currDirection)
        {
            nextTrack = nextTrackSCAN(requests, currTrack, size, currDirection);
            if (ArrayContains(alreadyUsed, nextTrack, size) != -1)
            {
                i--;
                currTrack = nextTrack;
                continue;
            }
            tracksTraversed = nextTrack - prevTrack;
        }
        if (!currDirection)
        {
            nextTrack = nextTrackSCAN(requests, currTrack, size, currDirection);
            if (ArrayContains(alreadyUsed, nextTrack, size) != -1)
            {
                i--;
                currTrack = nextTrack;
                continue;
            }
            tracksTraversed = prevTrack - nextTrack;
        }
        printf("%d %d\n", nextTrack, tracksTraversed);
        prevTrack = nextTrack;
        alreadyUsed[i] = nextTrack;
        currTrack = nextTrack;
    }
}
//C-SCAN - Tom Hoskins
void CScanAlgorithm(int requests[], bool startDirection, int startTrack, int size)
{
    bool currDirection = startDirection;
    int alreadyUsed[size];
    int currTrack = startTrack;
    int prevTrack = startTrack;
    int nextTrack, tracksTraversed;
    for (int i = 0; i < size; i++)
    {
        if (currDirection)
        {
            nextTrack = nextTrackSCAN(requests, currTrack, size, currDirection);
            if ((nextTrack == 199) && (currTrack != 199))
                nextTrack = nextTrackSCAN(requests, 0, size, currDirection);
            if (currTrack == 199)
                nextTrack = nextTrackSCAN(requests, 0, size, currDirection);
            if (ArrayContains(alreadyUsed, nextTrack, size) != -1)
            {
                i--;
                currTrack = nextTrack;
                continue;
            }
            tracksTraversed = abs(nextTrack - prevTrack);
        }
        if (!currDirection)
        {
            nextTrack = nextTrackSCAN(requests, currTrack, size, currDirection);
            if ((nextTrack == 0) && (currTrack != 0))
                nextTrack = nextTrackSCAN(requests, 199, size, currDirection);
            if(currTrack == 0)
                nextTrack = nextTrackSCAN(requests, 199, size, currDirection);
            if (ArrayContains(alreadyUsed, nextTrack, size) != -1)
            {
                i--;
                currTrack = nextTrack;
                continue;
            }
            tracksTraversed = abs(prevTrack - nextTrack);
        }
        printf("%d %d\n", nextTrack, tracksTraversed);
        prevTrack = nextTrack;
        alreadyUsed[i] = nextTrack;
        currTrack = nextTrack;
    }
}

//N-step-scan
void nStepScan(int arr[], int startVal, int size, int N, int M) {
    int requests[];
    for (int i = M; i< N, i++)
        requests[i] = arr[i];
    scanAlgorithm(requests, true, int startVal, size);
    if (M = 0)
        nStepScan(requests, startVal, size, N + N, N);
    else
        nStepScan(requests, startVal, size, N + M, M);
}

int ArrayContains(int arr[], int pageReference, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i] == pageReference)
            return i; //return index containing value
    }
    return -1; //-1 = page is not yet in array
}
//Continue track direction method for SCAN algorithm
bool continueDirection(int arr[], int lastTrack, int size, bool currentDirection)
{
    //Downward direction
    if (currentDirection == false)
    {
        if (lastTrack == 0) {
            return false;
        }
        for (int i = 0; i < size; i++)
        {
            if (arr[i] < lastTrack)
                return true; //There is still a smaller value
        }
    }
    if (currentDirection == true)
    {
        if (lastTrack == 199) {
            return false;
        }
        for (int i = 0; i < size; i++)
        {
            if (arr[i] > lastTrack)
                return true; //There is still a larger value 
        }
    }
    return false;
}

int nextTrackSCAN(int arr[], int lastTrack, int size, bool currentDirection)
{
    int temp = lastTrack;
    int low;
    int high;
    findNumber(arr, lastTrack, &low, &high, size);
    if (currentDirection == false)
    {
        temp = low;
    }
    if (currentDirection == true)
    {
        temp = high;
    }
    return temp;
}
void findNumber(int arr[], int num, int* low, int* high, int size) {
    *low = 0; //largest number lower than num
    *high = 199; //smallest number higher than num
    for (int i = 0; i < size; i++) {
        if ((arr[i] > *low) && (arr[i] < num) && (arr[i] != num)) {
            *low = arr[i];
        }
        if ((arr[i] < *high) && (arr[i] > num) && (arr[i] != num)) {
            *high = arr[i];
        }
    }
}

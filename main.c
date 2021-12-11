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

int time();
void calculateDifference();
int findMin();
int shortestSeekTimeFirst();
int firstInFirstOut();
int lastInFirstOut();
int ArrayContains(int arr[], int pageReference, int size);
int scanAlgorithm(int requests[], bool startDirection, int startTrack, int size);
int CScanAlgorithm(int requests[], bool startDirection, int startTrack, int size);
int FScanAlgorithm(int requests[], bool startDirection, int startTrack, int size, int queueLength);
bool continueDirection(int arr[], int alreadyUsed[], int lastTrack, int size, bool currentDirection);
int nextTrackSCAN(int arr[], int lastTrack, int size, bool currentDirection);
void findNumber(int arr[], int num, int *const low, int *const high, int size);
int nStepScan(int arr[], int startVal, int size, int N, int M, int totalTracksTraversed);

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

    int random1[1000];
    int random2[1000];
    int random3[1000];
    int random4[1000];
    int random5[1000];


    int counter = 0;
    FILE *file1 = fopen(p_fileName1, "r");;
    while (!feof(file1))
    {
        int value = 0;
        fscanf(file1, "%d", &value);
        random1[counter] = value;
        counter++;
    }
    counter = 0;
    FILE *file2 = fopen(p_fileName2, "r");
    while (!feof(file2))
    {
        int value = 0;
        fscanf(file2, "%d", &value);
        random2[counter] = value;
        counter++;
    }
    counter = 0;
    FILE *file3 = fopen(p_fileName3, "r");
    while (!feof(file3))
    {
        int value = 0;
        fscanf(file3, "%d", &value);
        random3[counter] = value;
        counter++;
    }
    counter = 0;
    FILE *file4 = fopen(p_fileName4, "r");
    while (!feof(file4))
    {
        int value = 0;
        fscanf(file4, "%d", &value);
        random4[counter] = value;
        counter++;
    }
    counter = 0;
    FILE *file5 = fopen(p_fileName5, "r");
    while (!feof(file5))
    {
        int value = 0;
        fscanf(file5, "%d", &value);
        random5[counter] = value;
        counter++;
    }

    printf("Given example array: {55,58,39,18,90,160,150,38,184}\nHead begins at track 0\n");
    shortestSeekTimeFirst(tableRequests, 0, 9);
    printf("\n------------------------------------\n");
    firstInFirstOut(tableRequests, 0, 9);
    printf("\n------------------------------------\n");
    lastInFirstOut(tableRequests, 0, 9);
    printf("\n------------------------------------\n");
    scanAlgorithm(tableRequests, true, 0, 9);
    printf("\n------------------------------------\n");
    CScanAlgorithm(tableRequests, true, 0, 9);
    printf("\n------------------------------------\n");
    FScanAlgorithm(tableRequests, true, 0, 9, 4);
    printf("\n------------------------------------\n");
    //nStepScan(tableRequests, 0, 9, 9, 0, 0/* Keep 0 */);

    printf("Given example array: {55,58,39,18,90,160,150,38,184}\nHead begins at track 199\n");
    shortestSeekTimeFirst(tableRequests, 199, 9);
    printf("\n------------------------------------\n");
    firstInFirstOut(tableRequests, 199, 9);
    printf("\n------------------------------------\n");
    lastInFirstOut(tableRequests, 199, 9);
    printf("\n------------------------------------\n");
    scanAlgorithm(tableRequests, true, 199, 9);
    printf("\n------------------------------------\n");
    CScanAlgorithm(tableRequests, true, 199, 9);
    printf("\n------------------------------------\n");
    FScanAlgorithm(tableRequests, true, 199, 9, 4);
    printf("\n------------------------------------\n");
    //nStepScan(tableRequests, 199, 9, 3, 0, 0/* Keep 0 */);

    int trackPointer[3] = {0, 100, 199};
    for(int i = 0; i < 3; i++)
    {
        printf("File 1 Track pointer beginning at %d:", trackPointer[i]);
        int temp = shortestSeekTimeFirst(random1, trackPointer[i], 1000);
        printf("SSTF:\n Total Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n------------------------------------\n");
        temp = firstInFirstOut(random1, trackPointer[i], 1000);
        printf("FIFO:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n------------------------------------\n");
        temp = lastInFirstOut(random1, trackPointer[i], 1000);
        printf("LIFO:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n------------------------------------\n");
        temp = scanAlgorithm(random1, true, trackPointer[i], 1000);
        printf("SCAN:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n------------------------------------\n");
        temp = CScanAlgorithm(random1, true, trackPointer[i], 1000);
        printf("CSCAN:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n------------------------------------\n");
        temp = FScanAlgorithm(random1, true, trackPointer[i], 1000, 4);
        printf("FSCAN:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n------------------------------------\n");
        // temp = nStepScan(random1, trackPointer[i], 1000, 3, 0, 0/* Keep 0 */);
        // printf("NSCAN:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n\n\n");

        printf("File 2 Track pointer beginning at %d:", trackPointer[i]);
        temp = shortestSeekTimeFirst(random2, trackPointer[i], 1000);
        printf("SSTF:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n------------------------------------\n");
        temp = firstInFirstOut(random2, trackPointer[i], 1000);
        printf("FIFO:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n------------------------------------\n");
        temp = lastInFirstOut(random2, trackPointer[i], 1000);
        printf("LIFO:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n------------------------------------\n");
        temp = scanAlgorithm(random2, true, trackPointer[i], 1000);
        printf("SCAN:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n------------------------------------\n");
        temp = CScanAlgorithm(random2, true, trackPointer[i], 1000);
        printf("CSCAN:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n------------------------------------\n");
        temp = FScanAlgorithm(random2, true, trackPointer[i], 1000, 4);
        printf("FSCAN:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n------------------------------------\n");
        // temp = nStepScan(random2, trackPointer[i], 1000, 3, 0, 0/* Keep 0 */);
        // printf("NSCAN:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n\n\n");

        printf("File 3 Track pointer beginning at %d:", trackPointer[i]);
        temp = shortestSeekTimeFirst(random3, trackPointer[i], 1000);
        printf("SSTF:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n------------------------------------\n");
        temp = firstInFirstOut(random3, trackPointer[i], 1000);
        printf("FIFO:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n------------------------------------\n");
        temp = lastInFirstOut(random3, trackPointer[i], 1000);
        printf("LIFO:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n------------------------------------\n");
        temp = scanAlgorithm(random3, true, trackPointer[i], 1000);
        printf("SCAN:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n------------------------------------\n");
        temp = CScanAlgorithm(random3, true, trackPointer[i], 1000);
        printf("CSCAN:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n------------------------------------\n");
        temp = FScanAlgorithm(random3, true, trackPointer[i], 1000, 4);
        printf("FSCAN:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n------------------------------------\n");
        // temp = nStepScan(random3, trackPointer[i], 1000, 3, 0, 0/* Keep 0 */);
        // printf("NSCAN:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n\n\n");

        printf("File 3 Track pointer beginning at %d:", trackPointer[i]);
        temp = shortestSeekTimeFirst(random4, trackPointer[i], 1000);
        printf("SSTF:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n------------------------------------\n");
        temp = firstInFirstOut(random4, trackPointer[i], 1000);
        printf("FIFO:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n------------------------------------\n");
        temp = lastInFirstOut(random4, trackPointer[i], 1000);
        printf("LIFO:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n------------------------------------\n");
        temp = scanAlgorithm(random4, true, trackPointer[i], 1000);
        printf("SCAN:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n------------------------------------\n");
        temp = CScanAlgorithm(random4, true, trackPointer[i], 1000);
        printf("CSCAN:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n------------------------------------\n");
        temp = FScanAlgorithm(random4, true, trackPointer[i], 1000, 4);
        printf("FSCAN:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n------------------------------------\n");
        // temp = nStepScan(random4, trackPointer[i], 1000, 3, 0, 0/* Keep 0 */);
        // printf("NSCAN:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n\n\n");

        printf("File 5 Track pointer beginning at %d:", trackPointer[i]);
        temp = shortestSeekTimeFirst(random5, trackPointer[i], 1000);
        printf("SSTF:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n------------------------------------\n");
        temp = firstInFirstOut(random5, trackPointer[i], 1000);
        printf("FIFO:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n------------------------------------\n");
        temp = lastInFirstOut(random5, trackPointer[i], 1000);
        printf("LIFO:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n------------------------------------\n");
        temp = scanAlgorithm(random5, true, trackPointer[i], 1000);
        printf("SCAN:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n------------------------------------\n");
        temp = CScanAlgorithm(random5, true, trackPointer[i], 1000);
        printf("CSCAN:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n------------------------------------\n");
        temp = FScanAlgorithm(random5, true, trackPointer[i], 1000, 4);
        printf("FSCAN:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n------------------------------------\n");
        // temp = nStepScan(random5, trackPointer[i], 1000, 3, 0, 0/* Keep 0 */);
        // printf("NSCAN:\nTotal Tracks Traversed : %d, Average Tracks Traversed per entry: %f", temp, (float) temp/1000);
        printf("\n\n\n");
    }


    
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
int shortestSeekTimeFirst(int request[], int head, int size)
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

    return seekCount;
}

//Fifo - Peter Fraser
int firstInFirstOut(int request[], int head, int size) {
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
    return seekCount;
}

// LIFO - Peter Fraser
int lastInFirstOut(int request[], int head, int size) {
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
    return seekCount;
}


//SCAN - Tom Hoskins
int scanAlgorithm(int requests[], bool startDirection, int startTrack, int size) 
{
    int totalTracksTraversed = 0;
    bool currDirection = startDirection;
    int alreadyUsed[size];
    for(int i = 0; i < size; i++)
    {
        alreadyUsed[i] = -1;
    }
    int currTrack = startTrack;
    int prevTrack = startTrack;
    int nextTrack, tracksTraversed;
    for (int i = 0; i < size; i++)
    {
        if (!continueDirection(requests, alreadyUsed, currTrack, size, currDirection))
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
            printf("%d ", nextTrack);
            if (ArrayContains(alreadyUsed, nextTrack, size) != -1)
            {
                i--;
                currTrack = nextTrack;
                continue;
            }
            tracksTraversed = prevTrack - nextTrack;
        }
        totalTracksTraversed += tracksTraversed;
        prevTrack = nextTrack;
        if(nextTrack == 199 || nextTrack == 0)
        {
            for(int j = 0; j < size; j++)
            {
                alreadyUsed[j] = -1;
            }
        }
        alreadyUsed[i] = nextTrack;
        currTrack = nextTrack;
    }

    return totalTracksTraversed;
}
//C-SCAN - Tom Hoskins
int CScanAlgorithm(int requests[], bool startDirection, int startTrack, int size)
{
    int totalTracksTraversed = 0;
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
            printf("%d ", nextTrack);
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
            printf("%d ", nextTrack);
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
        totalTracksTraversed += tracksTraversed;
        prevTrack = nextTrack;
        // if(nextTrack == 199 || nextTrack == 0)
        // {
        //     for(int j = 0; j < size; j++)
        //     {
        //         alreadyUsed[j] = -1;
        //     }
        // }
        alreadyUsed[i] = nextTrack;
        currTrack = nextTrack;
    }
    return totalTracksTraversed;
}


int FScanAlgorithm(int requests[], bool startDirection, int startTrack, int size, int queueLength)
{
    int totalTracksTraversed = 0;
    int numberOfQueueLoads = (int) size/queueLength;
    int requestRemainder = size % numberOfQueueLoads;
    int activeQueue[queueLength] ;
    int requestCounter = 0 ;

    for(int i = 0; i < numberOfQueueLoads; i++)
    {
        for(int j = 0; j < queueLength; j++)
        {
            activeQueue[j] = requests[j + requestCounter];
        }
        requestCounter += queueLength;
        totalTracksTraversed += scanAlgorithm(activeQueue, startDirection, startTrack, queueLength);
    }

    if(requestRemainder != 0)
    {
        int remainderQueue[requestRemainder];
        for(int k = 0; k < requestRemainder; k++)
        {
            remainderQueue[k] = requests[k + requestCounter];
        }
        totalTracksTraversed += scanAlgorithm(remainderQueue, startDirection, startTrack, requestRemainder);
    }
    return totalTracksTraversed;
}

//N-step-scan
// int nStepScan(int arr[], int startVal, int size, int N, int M, int totalTracksTraversed) 
// {
//     int requests[N - M];
//     for (int i = M; i< N; i++)
//         requests[i] = arr[i];
//     totalTracksTraversed += scanAlgorithm(requests, true, startVal, size);


//     if (M >= N)
//     {
//         printf("%d", M);
//         return totalTracksTraversed;
//     }

//     if (M = 0)
//     {
//         printf("%d", M);
//         nStepScan(arr, startVal, size, N + N, N, totalTracksTraversed);
//     }
        
//     else
//     {
//         printf("%d", M);
//         nStepScan(arr, startVal, size, N + M, N, totalTracksTraversed);
//     }
        
// }

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
bool continueDirection(int arr[], int alreadyUsed[], int lastTrack, int size, bool currentDirection)
{
    //Downward direction
    if (currentDirection == false)
    {
        if (lastTrack == 0) {
            return false;
        }
        for (int i = 0; i < size; i++)
        {
            if (arr[i] < lastTrack && (ArrayContains(alreadyUsed, arr[i], size) == -1))
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
            if (arr[i] > lastTrack && (ArrayContains(alreadyUsed, arr[i], size) == -1))
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
    *low = -1; //largest number lower than num
    *high = 200; //smallest number higher than num
    for (int i = 0; i < size; i++) {
        if ((arr[i] >= *low) && (arr[i] < num) && (arr[i] != num)) {
            *low = arr[i];
        }
        if ((arr[i] <= *high) && (arr[i] > num) && (arr[i] != num)) {
            *high = arr[i];
        }
    }
}

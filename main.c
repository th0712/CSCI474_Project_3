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
    int tableRequests[9] = {55,58,39,18,90,160,150,38,84};

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

void shortestSeekTimeFirst(int request[], int head, int requestSize)
{   
    struct node diff[requestSize];

    for (int i = 0; i< requestSize; i++) 
    {
        diff[i].distance = 0;
        diff[i].accessed = false;
    }

    int seek_count = 0;

    int seek_sequence[requestSize + 1];

    for (int i = 0; i < requestSize; i++) 
    {
        seek_sequence[i] = head;
        calculateDifference(request, head, diff, requestSize);

        int index = findMin(diff, requestSize);

        diff[index].accessed = true;

        seek_count += diff[index].distance;

        head = request[index];
    }

    seek_sequence[requestSize - 1] = head;

    printf("Total number of seek operations: %d \n", head);
    printf("Seeking sequence is: \n");
    for (int i = 0; i < 7; i++)
    {
        printf("%d \n", seek_sequence[i]);
    }

}

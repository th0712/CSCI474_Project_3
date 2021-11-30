//CSCI 474 Project 2
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

//structure to keep track of page and when it was last used
struct LRUVal
{
    int page;
    int access;
};

void makeRandomTextFiles();
void makeTextFileTemporalLocality();
bool assessTemporalLocality();
int time();
int incrementIndex();
int StructArrayContains(struct LRUVal arr[], int pageReference, int size);
int ArrayContains(int arr[], int pageReference, int size);
int FindLRU(struct LRUVal arr[], int size);
int LeastRecentlyUsedAlgorithm(unsigned int numFrames, const char *p_fileName);
void myClockAlgorithm(const char *p_filename, int outputArray[], int maxResSetSize);
int optReplacement(unsigned int numFrames, int pn, const char *p_fileName);

//Methods associated with Queue struct
//for First firstInFirstOut
int firstInFirstOut();
struct Queue *createQueue();
int isFull();
int isEmpty();
void enqueue();
int dequeue();
bool contains();

#define SMALL 15  //100,000,000
#define MEDIUM 30 //1,000,000,000
#define LARGE 60
#define LENGTH(x) (sizeof(x) / sizeof((x)[0])) //macro to find length of an array

/* Main Function */
int main(int argc, char *argv[])
{
    //Arguments do not match requirements specified in project.
    //Exit file.
    if (argc != 5)
    {
        printf("5 arguments expected in format: \n programName, inputFile.txt, minimumFramesAllocated, maximumFramesAllocated, pagesInProcess");
        exit(1);
    }

    //Save fileName to variable for readability
    const char *p_fileName = argv[1];

    //File doesn't exist
    if (!(access(p_fileName, F_OK) == 0))
    {
        //Extract max page number from arguments
        int pages = (int)strtol(argv[4], NULL, 10);

        //Generate text file
        makeTextFileTemporalLocality(p_fileName, pages);
    }

    int pages = (int)strtol(argv[4], NULL, 10);

    //Save min and max frames
    //We will simulate paging frame quantities between these values
    unsigned int minFrames = (int)strtol(argv[2], NULL, 10);
    unsigned int maxFrames = (int)strtol(argv[3], NULL, 10);

    int outputArray[maxFrames - 5];
    myClockAlgorithm(p_fileName, outputArray, maxFrames);
    /*for (int i = 0; i < maxFrames - 4+1; i++ ) {
        printf( "PageFaults with %d Frames: %d\n",i+4, outputArray[i]);
    }*/

    printf("\n\n");
    printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 Page Replacement Algorithm: \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\n");
    printf("-------------------------------------------------------------------------------\n");
    printf("\tFrames\t | \t FIFO \t |  \t CLOCK   |  \t LRU \t |\t OPT \t \n");
    printf("-------------------------------------------------------------------------------\n");
    for (int i = minFrames; i <= maxFrames; i++)
    {
        printf("%d frames \t | \t %d \t | \t %d \t | \t %d \t | \t %d \n", i, firstInFirstOut(i, p_fileName), outputArray[i - 4], LeastRecentlyUsedAlgorithm(i, p_fileName), optReplacement(i, pages, p_fileName));
    }
    printf("\n\n");
}

//Matt Greatens Algorithm
//Also the queue struct methods
int firstInFirstOut(unsigned int frames, const char *p_fileName)
{
    struct Queue *queue = createQueue(frames);
    FILE *file = fopen(p_fileName, "r");

    int pageFaults = 0;

    while (!feof(file))
    {

        int pageReference = 0;
        fscanf(file, "%d", &pageReference);

        //Triggers if we have a not full frame buffer or a page fault
        if (!(contains(queue, pageReference)))
        {
            if (isFull(queue))
            {
                //page fault
                pageFaults += 1;

                //Pop oldest value, add pageReference to queue
                dequeue(queue);
                enqueue(queue, pageReference);
            }
            else
            {
                //Fill frame buffer
                enqueue(queue, pageReference);
            }
        }
        fflush(stdout);
    }
    fclose(file);
    return pageFaults;
}

/*Tom Hoskins*/
int LeastRecentlyUsedAlgorithm(unsigned int numFrames, const char *p_fileName)
{
    struct LRUVal frames[numFrames];

    //initialize frames to default values
    for (int i = 0; i < numFrames; i++)
    {
        frames->page = -1;
        frames->access = -1;
    }

    FILE *file = fopen(p_fileName, "r");
    int pageReference;
    int accessNum = 0;
    int pageFaults = 0;
    int pagesAdded = 0;
    //Loop until we have read all values from page address stream
    while (!feof(file))
    {
        int pageReference = 0;
        fscanf(file, "%d", &pageReference);
        //printf("%d", pageReference);
        //There are still open frames
        if (pagesAdded < numFrames)
        {
            int pageLocation = StructArrayContains(frames, pageReference, numFrames); //location of page if in a frame, -1 otherwise
            if (pageLocation == -1)                                                   //add page to new frame
            {
                frames[pagesAdded].page = pageReference;
                frames[pagesAdded].access = accessNum;
                pagesAdded++;
            }
            else //update access number of page
            {
                frames[pageLocation].page = pageReference;
                frames[pageLocation].access = accessNum;
            }
        }
        else
        {
            int pageLocation = StructArrayContains(frames, pageReference, numFrames); //location of page if in a frame, -1 otherwise
            if (pageLocation == -1)                                                   //page not loaded, must replace LRU page
            {
                pageFaults++;
                int LRUPage = FindLRU(frames, numFrames);
                frames[LRUPage].page = pageReference;
                frames[LRUPage].access = accessNum;
            }
            else //update access number of page
            {
                frames[pageLocation].page = pageReference;
                frames[pageLocation].access = accessNum;
            }
        }
        accessNum++;
    }
    return pageFaults;
}

/*------------------------------------------------------------------------*/
//Clock Algorithm by Jackson Gravel
void myClockAlgorithm(const char *p_filename, int outputArray[], int maxResSetSize)
{
    FILE *in_file = fopen(p_filename, "r"); // read only file -> in_file
    int i = 0;
    int inputArray[1000];
    do
    { //Fill the inputStream with the ints from in_file
        if (feof(in_file))
            break;                             //if EOF break
        fscanf(in_file, "%d", &inputArray[i]); //put only ints into the array at index i
        i++;
    } while (1);
    fclose(in_file);

    for (int z = 0; z <= maxResSetSize - 4; z++)
    { // MASTER FOR LOOP
        int Frames[z + 4][2];
        int sizeofFrames = z + 4;
        int PageFaults = 1;
        int frameIndex = 0;
        int inputStreamIndex;
        bool skip = false;

        for (int x = 0; x < sizeofFrames; x++)
        { //cleans out the Frames array
            Frames[x][0] = 0;
            Frames[x][1] = 0;
        }

        for (inputStreamIndex = 0; inputStreamIndex < 1000; inputStreamIndex++)
        {
            //for loop for inputStreamIndex

            for (int x = 0; x < sizeofFrames; x++)
            {
                if (Frames[x][0] == inputArray[inputStreamIndex])
                {                //if input is already in the
                    skip = true; //Frames ignore the input
                    Frames[x][1] = 1;
                    break;
                }
            }

            for (frameIndex; frameIndex <= sizeofFrames + 1; frameIndex++)
            {
                if (frameIndex >= sizeofFrames) //if index is more than the
                    frameIndex = 0;             //array then reset it
                if (skip)
                {
                    skip = false;
                    break;
                }

                if (!skip)
                {
                    if (Frames[frameIndex][1] != 1)
                    {
                        Frames[frameIndex][0] = inputArray[inputStreamIndex];
                        Frames[frameIndex][1] = 1;
                        frameIndex++;
                        PageFaults++; //if input isn't in frame and clock bit is 0 then
                        break;
                    } //put input in Frame and inc PageFaults

                    else if (Frames[frameIndex][1] == 1) //if input isn't in frame and
                        Frames[frameIndex][1] = 0;       //clock bit is 1 then set to 0
                }
            }
        }
        outputArray[z] = PageFaults-sizeofFrames;
    }
}

//Peter Fraser
int optReplacement(unsigned int numFrames, int pages, const char *p_fileName)
{

    FILE *file = fopen(p_fileName, "r");

    int array[1000];
    int *a = array;
    int j = 0;
    int faults = 0;

    int frameArray[numFrames];

    while (!feof(file))
    {

        //Scan file
        int pageReference = 0;
        fscanf(file, "%d", &pageReference);

        //Save to an array so we can manipulate data.
        array[j] = pageReference;
        j++;
    }

    int x = 0;
    int b = 0;
    while (x < numFrames)
    {
        //ArrayContains returns -1 if arg[1] not in arg[0] array.
        int contains = ArrayContains(frameArray, array[b], numFrames);
        if (contains == -1)
        {
            //This is a new value.
            //Save to frame buffer
            frameArray[x] = array[b];
            b++;

            //Increment while loop counter.
            //Loop ends when frameBuffer is full
            x++;
        }
        b++;
    }

    //We use this value later to track optimal value to replace in frame buffer
    int pageArray[pages];

    //b marks the last location we read from file when reading into frame buffer
    //h will be used for while loop control
    int h = b;
    int index;
    int value;
    while (h < 1000)
    {

        //Setup pageArray.
        //pageArray has 0 for values not in frame buffer, -2 for values in frame buffer
        for (int i = 0; i < pages; i++)
        {
            if (ArrayContains(frameArray, i, numFrames) != -1)
            {
                pageArray[i] = -2;
            }
            else
            {
                pageArray[i] = 0;
            }
        }

        //Triggers when we encounter a page fault
        if (ArrayContains(frameArray, array[h], numFrames) == -1)
        {
            //Last value where pageArray[value] == -2 
            //maps to index where frameArray[index] == value
            //and frameArray[index] is the optimal frame replacement.
            while (ArrayContains(pageArray, -2, pages) != -1)
            {
                b++;

                //b has reached end of file
                if (b == 1000)
                {
                    //any remaining x where pageArray[x] == -2 is optimal page replacement
                    value = ArrayContains(pageArray, -2, pages);
                    break;
                }
                else
                {
                    //Value updated every iteration.
                    //Due to wider scope, value saved once we exit while loop.
                    value = array[b];
                    pageArray[value] = 1;
                }
            }
            //Grab index of optimal value in frameArrayto replace, and replace it.
            index = ArrayContains(frameArray, value, numFrames);
            frameArray[index] = array[h];
            faults++;
        }

        h++;
        b = h;
    }

    return faults;
}

struct Queue;
typedef struct Queue
{
    int front, back, size;
    unsigned int capacity;
    int *p_array;
};

struct Queue *createQueue(unsigned int capacity)
{
    //{Whatever the C-Equivalent of Instantiating is called} the queue
    struct Queue *queue = (struct Queue *)malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->back = capacity - 1;
    queue->p_array = (int *)malloc(queue->capacity * sizeof(int));
    return queue;
}

//Full when size = capacity
int isFull(struct Queue *queue)
{
    return (queue->size == queue->capacity);
}

int isEmpty(struct Queue *queue)
{
    return (queue->size == 0);
}

//Add new items to the queue
//Fifo indicates we should put new items in the back and remove from front
void enqueue(struct Queue *queue, int page)
{
    if (isFull(queue))
        return;
    //%capacity so rear can loop arount array
    queue->back = (queue->back + 1) % queue->capacity;
    queue->p_array[queue->back] = page;
    queue->size = queue->size + 1;
}

//Remove items from queue
//Sets dequeued item to -1, an impossible value for our use case
int dequeue(struct Queue *queue)
{
    if (isEmpty(queue))
        return INT_MIN;

    int item = queue->p_array[queue->front];

    //Set this to impossible value so our contains()
    //function doesn't get false positives
    queue->p_array[queue->front] = -1;

    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

//Returns true if pageReference is saved in p_array
bool contains(struct Queue *queue, int pageReference)
{
    for (int i = 0; i < queue->capacity; i++)
    {
        if (queue->p_array[i] == pageReference)
        {
            return true;
        }
    }
    return false;
}

int StructArrayContains(struct LRUVal arr[], int pageReference, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i].page == pageReference)
            return i; //return index containing value
    }
    return -1; //-1 = page is not yet in array
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

int FindLRU(struct LRUVal arr[], int size)
{
    int min = arr[0].access;
    int minIndex = 0;
    for (int i = 0; i < size; i++)
    {
        if (arr[i].access < min)
        {
            min = arr[i].access;
            minIndex = i;
        }
    }
    return minIndex;
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

int incrementIndex(int index)
{
    return ((index + 1) % 10);
}

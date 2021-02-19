/** @file SpreaderDetectorBackend.c
* @author  Yair Escott <yair.95@gmail.com>
*
* @brief the 2020 exam
*/

// ------------------------------ includes ------------------------------
#include <stdio.h>
#include "SpreaderDetectorParams.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief number of arguments in the main function
 */
#define ARGV 3

/**
 * @brief the maximum number of elements in a line
 */
#define LINE_LENGTH 1025

/**
 * @brief the usage error output
 */
#define USAGE "Usage: ./SpreaderDetectorBackend"

/**
 * @brief output for when there is a error with the input file
 */
#define INPUT_ERROR "Error in input files.\n"

/**
 * @brief output for when there is a error with the output file
 */
#define OUTPUT_ERROR "Error in output file.\n"

/**
 * @def struct person
 * @brief a struct which contains a person's information
 */
typedef struct Person
{
    char name[LINE_LENGTH];
    int age;
    unsigned long id;
    float probability;
}person;

/**
 * @brief global variable for the spreader of the virus
 */
person gSpreader;
/**
 * @brief an array of the people
 */
person * people;

/**
 * @brief flag to say if the file is empty
 */
int peopleFlag = 0;

/**
 * @brief flag to say if the file is empty
 */
int meetingFlag = 0;

/**
 * @brief the people counter
 */
int gNumberOfPeople = 0;

/**
 * @brief the capacity of the people array
 */
int gPeopleCapacity = 0;

/**
 * @brief free's the array of people
 */
void freePeople();

/**
 * @brief prints to a file the relevant message for every person
 */
int printing();

/**
 * @brief the quicksort algorithm- using partition- finds the pivots place then
 * recursively sorts the two halves of the array by the id
 */
void quickSortId(int left, int right);
/**
 * @brief the quicksort algorithm- using partition- finds the pivots place then
 * recursively sorts the two halves of the array by the probability
 */
void quickSortProb(int left, int right);

/**
 * @brief the functional part of the quicksort algorithm, sorts by id
 * @return the index if the pivot who is in its correct place
 */
int partitionId (int left, int right);

/**
 * @brief the functional part of the quicksort algorithm
 */
int partitionProb (int left, int right);

/**
 * @brief this method switches 2 people in an array
 */
void switchPerson(int i, int j);

/**
 * @brief adds a new person to the people file
 */
int personPush(person * newPerson);

/**
 * @brief the crna function
 */
float crna(float spreaderProb, float distance, float time);

/**
 * @brief parses the meetings file and stors the data
 */
int meetingsParser(char* filePath);

/**
 * @brief parses the peoples file path
 */
int peopleParser(char* filePath);

/**
 * @brief parses tha lines of the meetings file
 */
int meetingLineParser(char* line, int lineCounter);

/**
 * @brief finds a person based on his/her ID
 */
person* personFinder(int id);

/**
 * @brief parses the lines from the people file
 */
int personLineParser(char* line);

/**
 * @brief checks that the files are not empty
 */
int fileChecker(char* peoplePath, char* meetingPath);

/**
 * @brief preforms binary search using the persons id
 */
person* binarySearchId(int left, int right, int compare);

// ------------------------------ functions -----------------------------

/**
 * @brief checks that the files are not empty
 * @param peoplePath the path to the people file
 * @param meetingPath path to the meeting file
 * @return 0 if failed and 1 if not
 */
int fileChecker(char* peoplePath, char* meetingPath)
{
    FILE * peop;
    peop = fopen(peoplePath, "r");
    if (peop == NULL)
    {
        fprintf(stderr, INPUT_ERROR);
        return 0;
    }
    fseek(peop, 0, SEEK_END);
    unsigned long len = (unsigned long)ftell(peop);
    if (len <= 0)
    {
       peopleFlag = 1;
    }
    else
    {
        rewind(peop);
    }
    fclose(peop);
    // check the meetings file
    FILE * meet;
    meet = fopen(meetingPath, "r");
    if (meet == NULL)
    {
        fprintf(stderr, INPUT_ERROR);
        return 0;
    }
    fseek(meet, 0, SEEK_END);
    unsigned long len2 = (unsigned long)ftell(meet);
    if (len2 <= 0)
    {
        meetingFlag = 1;
    }
    else
    {
        rewind(meet);
    }
    fclose(meet);
    // bothe files are empty
    if(meetingFlag == 1 && peopleFlag == 1)
    {
        FILE * outputFile;
        outputFile = fopen(OUTPUT_FILE, "w");
        if (outputFile == NULL)
        {
            fprintf(stderr, OUTPUT_ERROR);
            return 0;
        }
        fclose(outputFile);
        return 0;
    }
    //if the meeting file is empty but the people file is not
    if(meetingFlag == 1 && peopleFlag == 0)
    {
        FILE * outputFile;
        outputFile = fopen(OUTPUT_FILE, "w");
        if (outputFile == NULL)
        {
            fprintf(stderr, OUTPUT_ERROR);
            return 0;
        }
        int counter = 0;
        while (counter <= gNumberOfPeople - 1)
        {
            fprintf(outputFile, "crna %f\n", people[gNumberOfPeople - 1 - counter].probability);
            fprintf(outputFile, CLEAN_MSG, people[gNumberOfPeople - 1 - counter].name,
                    people[gNumberOfPeople - 1 - counter].id);
            counter += 1;
        }
        fclose(outputFile);
        return 0;
    }
    return 1;
}

/**
 * @brief free's the people array
 */
void freePeople()
{
    free(people);
    free(people);
    people = NULL;
}

/**
 * @brief prints to a file the relevant message for every person
 */
int printing()
{
    FILE * outputFile;
    outputFile = fopen(OUTPUT_FILE, "w");
    if (outputFile == NULL)
    {
        fprintf(stderr, OUTPUT_ERROR);
        return 0;
    }
    int counter = 0;
    while(counter != gNumberOfPeople && people[gNumberOfPeople - 1 - counter].probability >=
          MEDICAL_SUPERVISION_THRESHOLD)
    {
        fprintf(outputFile, MEDICAL_SUPERVISION_THRESHOLD_MSG, people[gNumberOfPeople - 1 - counter]
        .name, people[gNumberOfPeople - 1 - counter].id);
        counter += 1;
    }
    while (counter != gNumberOfPeople && people[gNumberOfPeople - 1 - counter].probability >=
           REGULAR_QUARANTINE_THRESHOLD)
    {
        fprintf(outputFile, REGULAR_QUARANTINE_MSG, people[gNumberOfPeople - 1 - counter].name,
                people[gNumberOfPeople - 1 - counter].id);
        counter += 1;
    }
    while (counter <= gNumberOfPeople - 1)
    {
        fprintf(outputFile, CLEAN_MSG, people[gNumberOfPeople - 1 - counter].name,
                people[gNumberOfPeople - 1 - counter].id);
        counter += 1;
    }
    fclose(outputFile);
    return 1;
}

/**
 * @brief the functional part of the quicksort algorithm, sorts by id
 * @return the index if the pivot who is in its correct place
 */
int partitionId (int left, int right)
{
    unsigned long pivot = people[right].id;
    int i = (left - 1);
    for (int j = left; j <= right - 1; j++)
    {
        if (people[j].id < pivot)
        {
            i++;
            switchPerson(i, j);
        }
    }
    switchPerson(i + 1, right);
    return (i + 1);
}


/**
 * @brief the functional part of the quicksort algorithm, sorts by probability
 * @return the index if the pivot who is in its correct place
 */
int partitionProb (int left, int right)
{
    float pivot = people[right].probability;
    int i = (left - 1);
    for (int j = left; j <= right - 1; j++)
    {
        if (people[j].probability < pivot)
        {
            i++;
            switchPerson(i, j);
        }
    }
    switchPerson(i + 1, right);
    return (i + 1);
}

/**
 * @brief this method switches 2 people in an array
 */
void switchPerson(int i, int j)
{
    person t = people[i];
    people[i] = people[j];
    people[j] = t;
}

/**
 * @brief the quicksort algorithm- using partition- finds the pivots place then
 * recursively sorts the two halves of the array by the id
 */
void quickSortId(int left, int right)
{
    if (left < right)
    {
        int par = partitionId(left, right);
        quickSortId(left, par - 1);
        quickSortId(par + 1, right);
    }
}

/**
 * @brief the quicksort algorithm- using partition- finds the pivots place then
 * recursively sorts the two halves of the array by the probability
 */
void quickSortProb(int left, int right)
{
    if (left < right)
    {
        int par = partitionProb(left, right);
        quickSortProb(left, par - 1);
        quickSortProb(par + 1, right);
    }
}

/**
 * @brief parses the meetings file and stors the data
 * @param filePath the meetings file path
 * @return 0 if failed and 1 if succeeded
 */
int meetingsParser(char* filePath)
{
    FILE *fpf;
    char line[LINE_LENGTH];
    fpf = fopen(filePath, "r");
    // i have already checked if the file opens in the filechecker function
    int lineCounter = 0;
    while (fgets(line, LINE_LENGTH, fpf) != NULL)
    {
        if(meetingLineParser(line, lineCounter) == 0)
        {
            fclose(fpf);
            return 0;
        }
        lineCounter += 1;
    }
    fclose(fpf);
    return 1;
}

/**
 * @brief parses the peoples file path
 * @param filePath the peoples file path
 * @return 0 if failed and  if not
 */
int peopleParser(char* filePath)
{
    people = (person *)malloc(2 * sizeof(person));
    if(people == NULL)
    {
        freePeople();
        fprintf(stderr, STANDARD_LIB_ERR_MSG);
        return 0;
    }
    FILE *fpf;
    char line[LINE_LENGTH];
    fpf = fopen(filePath, "r");
    // i have already chcked if the file opens in the filechecker function
    int counter = 0;
    while (fgets(line, LINE_LENGTH, fpf) != NULL)
    {
        if (personLineParser(line) == 0)
        {
            fclose(fpf);
            return 0;
        }
        counter += 1;
    }
    fclose(fpf);
    return 1;
}

/**
 * @brief parses the lines of the meetings file
 * @param line the line from the file
 * @param lineCounter the number line
 * @return 0 if failed and 1 if not
 */
int meetingLineParser(char* line, int lineCounter)
{
    char * word = strtok(line, " ");
    int counter = 0;
    float distance;
    float time;
    person * infected;
    while( word != NULL )
    {
        if(counter == 0)
        {
            if(lineCounter == 0)
            {
                person* person1 = personFinder(atoi(word));
                gSpreader = *person1;
                person1->probability = 1;
                gSpreader.probability = 1;
            }
            else
            {
                if((int) gSpreader.id != atoi(word))
                {
                    gSpreader = *personFinder(atoi(word));
                }
            }
        }
        else if(counter == 1)
        {
            infected = personFinder(atoi(word));
        }
        else if(counter == 2)
        {
            distance = atof(word);
        }
        else if(counter == 3)
        {
            time = atof(word);
            infected->probability = crna(gSpreader.probability, distance,
                    time);
        }
        else
        {
            fprintf(stderr, STANDARD_LIB_ERR_MSG);
            return 0;
        }
        word = strtok(NULL, " ");
        counter += 1;
    }
    return 1;
}

/**
 * @brief the crna function
 * @param spreaderProb the probability that the spreader has corona
 * @param distance the distance between the people
 * @param time the time spent together
 * @return the final number
 */
float crna(float spreaderProb, float distance, float time)
{
    float denominator = distance * MAX_TIME;
    float numerator = time * MIN_DISTANCE;
    return spreaderProb * (numerator / denominator);
}

/**
 * @brief finds a person based on his/her ID
 * @param id the id
 * @return the person
 */
person* personFinder(int id)
{
    return binarySearchId(0, gNumberOfPeople - 1, id);
}

/**
 * @brief preforms binary search using the persons id
 * @param left the left index
 * @param right the right index
 * @param compare the id to compare
 * @return the person we are looking for
 */
person* binarySearchId(int left, int right, int compare)
{
    if (right >= left)
    {
        int mid = left + (right - left) / 2;
        if ((int) people[mid].id == compare)
        {
            return &people[mid];
        }
        if ((int) people[mid].id > compare)
        {
            return binarySearchId(left, mid - 1, compare);
        }
        return binarySearchId(mid + 1, right, compare);
    }
    return NULL;
}


/**
 * @brief adds a new person to the people file
 * @param newPerson the new person to add
 */
int personPush(person * newPerson)
{
    if(gNumberOfPeople == gPeopleCapacity)
    {
        person * temp = (person *) realloc(people, sizeof(person) * (4 + gNumberOfPeople));
        if(temp == NULL)
        {
            fprintf(stderr, STANDARD_LIB_ERR_MSG);
            return 0;
        }
        else
        {
            people = temp;
            gPeopleCapacity += 4;
        }
    }
    people[gNumberOfPeople] = *newPerson;
    gNumberOfPeople += 1;
    return 1;
}

/**
 * @brief parses the lines from the people file
 * @param line the line to parse
 * @return 0 if failed and 1 if not
 */
int personLineParser(char* line)
{
    person newPerson;
    newPerson.probability = -1.0f;
    char *word = strtok(line, " ");
    int counter = 0;
    while (word != NULL)
    {
        if (counter == 0)
        {
            strcpy(newPerson.name, word);
        }
        else if (counter == 1)
        {
            newPerson.id = atoi(word);
        }
        else if (counter == 2)
        {
            newPerson.age = atoi(word);
        }
        else
        {
            fprintf(stderr, STANDARD_LIB_ERR_MSG);
            return 0;
        }
        word = strtok(NULL, " ");
        counter += 1;
    }
    return personPush(&newPerson);
}

/**
 * @brief the main function
 * @param argc the number of values
 * @param argv the file paths that it receives
 * @return 1 if failed 0 if succeeded
 */
int main(int argc, char* argv[])
{
    if(argc == ARGV)
    {
        if(fileChecker(argv[1], argv[2]) == 0)
        {
            return 1;
        }
        if(peopleParser(argv[1]) == 0)
        {
            return 1;
        }
        else
        {
            quickSortId(0, gNumberOfPeople - 1);
            if(meetingsParser(argv[2]) == 0)
            {
                freePeople();
                return 1;
            }
            else
            {
                quickSortProb(0, gNumberOfPeople -1);
                if(printing() == 0)
                {
                    freePeople();
                    return 1;
                }
                freePeople();
            }
        }
    }
    else
    {
        fprintf(stderr, "%s, %s, %s, '.'\n", USAGE, argv[1], argv[2]);
        return 1;
    }
}
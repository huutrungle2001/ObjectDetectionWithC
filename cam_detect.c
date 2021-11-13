#include <stdio.h>
#include <string.h>
#include "bitmap.c"

typedef struct
{
    char Objects[255];
    int Hue, MaxDiff, MinSat, MinVal;
} Calibration;

Calibration *listCalibration;
int nbCalibration;

void printCalibration(Calibration cal)
{
    // Check read successful
    if (cal.Hue == -1)
    {
        return;
    }
    printf("%s: Hue: %d (Max. Diff %d), Min. SV: %d %d\n", cal.Objects, cal.Hue, cal.MaxDiff, cal.MinSat, cal.MinVal);
}

bool checkInput(int argc, char **argv)
{

    // something for debug
    printf("*****************************\nNumber of argc: %d\n", argc);
    for (int i = 0; i < argc; i++)
        printf("%s ", argv[i]);
    printf("\n***************************\n");

    if (argc >= 5 || argc <= 1)
    {
        printf("Incorrect input.\n");
        return false;
    }

    char *mode = argv[1];
    if (strlen(mode) > 1 || (mode[0] != 'c' && mode[0] != 's' && mode[0] != 'd'))
    {
        printf("Incorrect input.\n");
        return false;
    }

    // check calibration file with mode "s" or mode "s"
    if (mode[0] == 's' || mode[0] == 'd')
    {
        char *calibrationFile = argv[2];

        // Open file to read
        FILE *file = freopen(calibrationFile, "r", stdin);

        // Check valid file.
        if (file == NULL)
        {
            printf("Could not open calibration file.\n");
            return false;
        }

        fclose(file);
    }
    return true;
}

void readASingleCalibration(Calibration *cal)
{
    scanf("%s", cal->Objects);
    scanf("%d%d%d%d", &cal->Hue, &cal->MaxDiff, &cal->MinSat, &cal->MinVal);
}

Calibration *readCalibration(char *calibrationFile, int *nbCalibration)
{

    // Open file to read
    FILE *file = freopen(calibrationFile, "r", stdin);

    printf("Calibrated objects:\n");

    // Declare a list of Calibration variable to hold the Calibration data.
    Calibration *listCal = malloc(sizeof(Calibration));
    Calibration aCalibration;

    *nbCalibration = 0;
    aCalibration.Hue = -1;

    // Read until end of file.
    while (!feof(file))
    {
        // Read a new calibration
        readASingleCalibration(&aCalibration);

        // printCalibration(aCalibration);

        // if reading is not ok
        if (aCalibration.Hue == -1)
            break;

        // Move to the next calibration
        *(listCal + *nbCalibration) = aCalibration;
        (*nbCalibration)++;

        // // Make room for another calibration
        listCal = realloc(listCal, (*nbCalibration + 1) * sizeof(Calibration));

        // Reset the Calibration variable.
        aCalibration.Hue = -1;
    }
    return listCal;
}

void displayCalibration()
{
    for (int i = 0; i < nbCalibration; i++)
        printCalibration(*(listCalibration + i));
}

int main(int argc, char **argv)
{
    if (checkInput(argc, argv) == false)
        return 0;

    char *mode = argv[1];

    // Mode s: Show calibration
    if (strcmp(mode, "s") == 0 || strcmp(mode, "d") == 0)
    {
        // Get calibration file path from argument vector.
        char *calibrationFilepath = argv[2];
        // read calibrations into an array
        listCalibration = readCalibration(calibrationFilepath, &nbCalibration);

        printf("Number of calibration: %d\n", nbCalibration);

        if (strcmp(mode, "s") == 0)
        {
            displayCalibration();
            return 0;
        }
        else // mode == "d"
        {
            // do something here
        }
    } // mode == "c"
    else
    {
        // do something here
    }
}

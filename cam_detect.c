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
HSV center[50][50];

typedef struct
{
    int w, h;
    int **pixels;
    int **regions;
    int nbRegions;
} Bitmap01;

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
    // printf("*****************************\nNumber of argc: %d\n", argc);
    // for (int i = 0; i < argc; i++)
    //     printf("%s ", argv[i]);
    // printf("\n***************************\n");

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

HSV center[50][50];

#define MIN_SATURATION 50
#define MIN_VALUE 30

void CalibrateColorProfile(char *object_name, char *bitmap_file, char *calibration_file)
{
    Bmp bmp = read_bmp(bitmap_file);
    int midX = bmp.width / 2;
    int midY = bmp.height / 2;
    //(50,50) => 25,25
    int minHue = INT_MAX, maxHue = INT_MIN;

    for (int i = midY - 25, m = 0; i < midY + 25 && m < 50; i++, m++)
    {
        for (int j = midX - 25, n = 0; j < midX + 25 && n < 50; j++, n++)
        {
            center[m][n] = rgb2hsv(bmp.pixels[i][j]);

            // update saturation and value meet the thresholds
            if (center[m][n].saturation >= MIN_SATURATION && center[m][n].value >= MIN_VALUE)
            {
                minHue = (minHue > center[m][n].hue ? center[m][n].hue : minHue);
                maxHue = (maxHue < center[m][n].hue ? center[m][n].hue : maxHue);
            }
        }
    }

    printf("%d %d\n", minHue, maxHue);
    int middle_hue = hue_midpoint(minHue, maxHue);
    int max_hue_difference = hue_difference(minHue, maxHue);

    // for (int i = 0; i < 50; i++)
    // {
    //     for (int j = 0; j < 50; j++)
    //     {
    //         printf("(%d %d %d) ", center[i][j].hue, center[i][j].saturation, center[i][j].value);
    //     }
    //     printf("\n");
    // }
    // FILE *f = freopen(calibration_file, "a", stdout);

    printf("%s %d %d %d %d\n", object_name, middle_hue, max_hue_difference, MIN_SATURATION, MIN_VALUE);
}

int dx[] = {-1, 0, 1, 0};
int dy[] = {0, -1, 0, 1};

void dfs(int i, int j, Bitmap01 *bitmap01)
{
    bitmap01->regions[i][j] = bitmap01->nbRegions;
    for (int h = 0; h < 4; h++)
    {
        int x = i + dx[h], y = j + dy[h];
        if (x >= 0 && x < bitmap01->w && y >= 0 && y < bitmap01->h)
        {
            dfs(x, y, bitmap01);
        }
    }
}

void generate_regions(Bitmap01 *bitmap01)
{
    int **mark = bitmap01->regions;

    mark = malloc(bitmap01->h * sizeof(int *));
    if (mark == NULL)
    {
        fprintf(stderr, "out of memory\n");
        exit(0);
    }
    for (int i = 0; i < bitmap01->w; i++)
    {
        mark[i] = malloc(bitmap01->w * sizeof(int));
        if (mark[i] == NULL)
        {
            fprintf(stderr, "out of memory\n");
            exit(0);
        }
    }

    memset(bitmap01->regions, 0, sizeof(bitmap01->regions));

    for (int i = 0; i < bitmap01->h; i++)
        for (int j = 0; j < bitmap01->w; j++)
            if (bitmap01->pixels[i][j] == 1 && bitmap01->regions[i][j] == 0)
            {
                bitmap01->nbRegions++;
                dfs(i, j, bitmap01);
            }
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
        else // mode == "d" - detect object
        {
            // do something here
        }
    } // mode == "c" - calibration
    else
    {
        char *calibrationCode = argv[2];
        char *calibrationBitMap = argv[3];

        CalibrateColorProfile(calibrationCode, calibrationBitMap, "test.txt");
    }
}

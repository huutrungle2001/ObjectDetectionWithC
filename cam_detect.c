#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "bitmap.c"
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>

#define MY_INT_MAX 1000000000
#define MY_INT_MIN -1000000000

typedef struct
{
    char Objects[255];
    int Hue, MaxDiff, MinSat, MinVal;
} Calibration;

Calibration *listCalibration;
int nbCalibration;

typedef struct
{
    int min_x, max_x, min_y, max_y;
} BoundingBox;

typedef struct
{
    char calibration_code[255];
    int w, h;
    int **pixels;
    int **regions;
    int nbRegions;
    BoundingBox *boundingBoxes;

} Bitmap01;

int get_min(int a, int b)
{
    return (a < b) ? a : b;
}

int get_max(int a, int b)
{
    return (a > b) ? a : b;
}

void print_calibration(Calibration cal)
{
    // Check read successful
    if (cal.Hue == -1)
    {
        return;
    }
    printf("%s: Hue: %d (Max. Diff: %d), Min. SV: %d %d\n", cal.Objects, cal.Hue, cal.MaxDiff, cal.MinSat, cal.MinVal);
}

bool check_input(int argc, char **argv)
{

    // something for debug
    // printf("*****************************\nNumber of argc: %d\n", argc);
    // for (int i = 0; i < argc; i++)
    //     printf("%s ", argv[i]);
    // printf("\n***************************\n");

    if (argc >= 5 || argc <= 1)
    {
        printf("Incorrect input\n");
        return false;
    }

    char *mode = argv[1];
    if (strlen(mode) > 1 || (mode[0] != 'c' && mode[0] != 's' && mode[0] != 'd'))
    {
        printf("Incorrect input\n");
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

    // printf("%s\n", cal->Objects);

    scanf("%d%d%d%d", &cal->Hue, &cal->MaxDiff, &cal->MinSat, &cal->MinVal);
}

bool readASingleCalibrationwithFile(FILE *f, Calibration *cal)
{

    return (EOF != fscanf(f, "%s%d%d%d%d", cal->Objects, &cal->Hue, &cal->MaxDiff, &cal->MinSat, &cal->MinVal));
}

Calibration *read_calibration(char *calibrationFile, int *nbCalibration)
{

    // Open file to read
    FILE *file = freopen(calibrationFile, "r", stdin);

    // printf("Calibrated Objects:\n");

    // Declare a list of Calibration variable to hold the Calibration data.
    Calibration *listCal = malloc(10 * sizeof(Calibration));
    Calibration aCalibration;

    *nbCalibration = 0;
    aCalibration.Hue = -1;

    // Read until end of file.

    while (!feof(file))
    {
        // Read a new calibration
        readASingleCalibration(&aCalibration);

        print_calibration(aCalibration);

        // if reading is not ok
        if (aCalibration.Hue == -1)
            break;

        // Move to the next calibration
        *(listCal + *nbCalibration) = aCalibration;
        (*nbCalibration)++;

        // // Make room for another calibration
        // listCal = realloc(listCal, (*nbCalibration + 1) * sizeof(Calibration));

        // Reset the Calibration variable.
        aCalibration.Hue = -1;
    }

    fclose(file);

    return listCal;
}

Calibration *read_calibration_with_function(char *calibrationFile, int *nbCalibration)
{
    Calibration *listCal = malloc(10 * sizeof(Calibration));

    FILE *file = fopen(calibrationFile, "r");

    // printf("Calibrated Objects:\n");

    // Declare a list of Calibration variable to hold the Calibration data.
    Calibration aCalibration;

    *nbCalibration = 0;
    aCalibration.Hue = -1;

    while (!feof(file))
    {
        // Read a new calibration
        readASingleCalibrationwithFile(file, &aCalibration);

        // print_calibration(aCalibration);

        // if reading is not ok
        if (aCalibration.Hue == -1)
            break;

        // Move to the next calibration
        *(listCal + *nbCalibration) = aCalibration;
        (*nbCalibration)++;

        // // Make room for another calibration
        // listCal = realloc(listCal, (*nbCalibration + 1) * sizeof(Calibration));

        // Reset the Calibration variable.
        aCalibration.Hue = -1;
        strcpy(aCalibration.Objects, "");
    }

    fclose(file);

    return listCal;
}

void test_reading_file(char *FILENAME)
{

    FILE *fp;
    char data[255];
    fp = fopen(FILENAME, "r");
    if (fp == NULL)
    {
        printf("Could not open file test.c");
        return;
    }
    while (fgets(data, 255, fp) != NULL)

        printf("%s", data);
    fclose(fp);
    exit(0);
    // char *line_buf = NULL;
    // size_t line_buf_size = 0;
    // int line_count = 0;
    // size_t line_size;
    // FILE *fp = fopen(FILENAME, "r");
    // if (!fp)
    // {
    //     fprintf(stderr, "Error opening file '%s'\n", FILENAME);
    //     return;
    // }

    // /* Get the first line of the file. */
    // line_size = getline(&line_buf, &line_buf_size, fp);

    // /* Loop through until we are done with the file. */
    // while (feof(fp))
    // {
    //     /* Increment our line count */
    //     line_count++;

    //     /* Show the line details */
    //     printf("%s", line_buf);

    //     /* Get the next line */
    //     line_size = getline(&line_buf, &line_buf_size, fp);
    // }

    // /* Free the allocated line buffer */
    // free(line_buf);
    // line_buf = NULL;

    // /* Close the file now that we are done with it */
    // fclose(fp);

    // return;
}
Calibration *read_calibration_with_function_new(char *calibrationFile, int *nbCalibration)
{
    Calibration *listCal = malloc(10 * sizeof(Calibration));

    Calibration aCalibration;

    *nbCalibration = 0;
    aCalibration.Hue = -1;

    FILE *file = fopen(calibrationFile, "r");

    while (1)
    {
        // Read a new calibration
        bool ok = readASingleCalibrationwithFile(file, &aCalibration);

        if (!ok)
            break;

        // print_calibration(aCalibration);

        // if reading is not ok
        if (aCalibration.Hue == -1)
            break;

        // Move to the next calibration
        *(listCal + *nbCalibration) = aCalibration;
        (*nbCalibration)++;

        // // Make room for another calibration
        // listCal = realloc(listCal, (*nbCalibration + 1) * sizeof(Calibration));

        // Reset the Calibration variable.
        aCalibration.Hue = -1;
        strcpy(aCalibration.Objects, "");
    }

    fclose(file);

    return listCal;
}

void displayCalibration()
{
    for (int i = 0; i < nbCalibration; i++)
        print_calibration(*(listCalibration + i));
}

HSV center[50][50];

#define MIN_SATURATION 50
#define MIN_VALUE 30

void calibrate_color_profile(char *object_name, char *bitmap_file, char *calibration_file)
{
    Bmp bmp = read_bmp(bitmap_file);
    int midX = bmp.width / 2;
    int midY = bmp.height / 2;
    //(50,50) => 25,25
    int min_hue = MY_INT_MAX, max_hue = MY_INT_MIN;

    for (int i = midY - 25, m = 0; i < midY + 25 && m < 50; i++, m++)
    {
        for (int j = midX - 25, n = 0; j < midX + 25 && n < 50; j++, n++)
        {
            center[m][n] = rgb2hsv(bmp.pixels[i][j]);

            // update saturation and value that meet the thresholds
            if (center[m][n].saturation >= MIN_SATURATION && center[m][n].value >= MIN_VALUE)
            {
                min_hue = get_min(min_hue, center[m][n].hue);
                max_hue = get_max(max_hue, center[m][n].hue);
            }
        }
    }

    // printf("%d %d\n", minHue, maxHue);
    int middle_hue = hue_midpoint(min_hue, max_hue);
    int max_hue_difference = hue_difference(min_hue, max_hue) / 2;

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

int dx[] = {1, -1, 0, 0};
int dy[] = {0, 0, 1, -1};

void dfs(int i, int j, Bitmap01 *bitmap01)
{
    // printf("%d %d %d\n", bitmap01->nbRegions, i, j);
    bitmap01->regions[i][j] = bitmap01->nbRegions;
    for (int h = 0; h < 4; h++)
    {
        int x = i + dx[h], y = j + dy[h];
        if (x >= 0 && x < bitmap01->h && y >= 0 && y < bitmap01->w)
            if (bitmap01->regions[x][y] == 0 && bitmap01->pixels[i][j] == 1)
            {
                dfs(x, y, bitmap01);
            }
    }
}

void generate_regions(Bitmap01 *bitmap01)
{

    bitmap01->regions = malloc(bitmap01->h * sizeof(int *));
    if (bitmap01->regions == NULL)
    {
        fprintf(stderr, "out of memory 1\n");
        exit(0);
    }
    for (int i = 0; i < bitmap01->h; i++)
    {
        bitmap01->regions[i] = malloc(bitmap01->w * sizeof(int));
        if (bitmap01->regions[i] == NULL)
        {
            fprintf(stderr, "out of memory 2\n");
            exit(0);
        }
    }

    bitmap01->nbRegions = 0;
    for (int i = 0; i < bitmap01->h; i++)
        for (int j = 0; j < bitmap01->w; j++)
            bitmap01->regions[i][j] = 0;

    for (int i = 0; i < bitmap01->h; i++)
        for (int j = 0; j < bitmap01->w; j++)
        {
            // se loi neu lenh nay thuc thi
            // printf("%d %d %d\n", i, j, bitmap01->regions[i][j]);

            if (bitmap01->pixels[i][j] == 1 && bitmap01->regions[i][j] == 0)
            {
                // printf("Region %d %d %d:\n", bitmap01->nbRegions, i, j);
                bitmap01->nbRegions++;
                dfs(i, j, bitmap01);
            }
        }
}

bool large_enough(BoundingBox *boundingBox)
{
    return (boundingBox->max_x - boundingBox->min_x >= 19) && (boundingBox->max_y - boundingBox->min_y >= 19);
}

void bounding_boxes(Bitmap01 *bitmap01)
{
    bitmap01->boundingBoxes = malloc(bitmap01->nbRegions * sizeof(BoundingBox));

    // printf("Number of regions: %d\n", bitmap01->nbRegions);
    // printf("we are in bounding boxes 1\n");

    // initialize bounding boxes
    for (int i = 0; i < bitmap01->nbRegions; i++)
    {
        bitmap01->boundingBoxes[i].max_x = bitmap01->boundingBoxes[i].max_y = MY_INT_MIN;
        bitmap01->boundingBoxes[i].min_x = bitmap01->boundingBoxes[i].min_y = MY_INT_MAX;
    }

    // printf("we are in bounding boxes 2\n");
    // update bounding boxes
    for (int i = 0; i < bitmap01->h; i++)
        for (int j = 0; j < bitmap01->w; j++)
        {

            if (bitmap01->regions[i][j])
            {

                int region = bitmap01->regions[i][j];

                bitmap01->boundingBoxes[region - 1].max_x =
                    get_max(bitmap01->boundingBoxes[region - 1].max_x, j);

                bitmap01->boundingBoxes[region - 1].max_y =
                    get_max(bitmap01->boundingBoxes[region - 1].max_y, i);

                bitmap01->boundingBoxes[region - 1].min_x =
                    get_min(bitmap01->boundingBoxes[region - 1].min_x, j);

                bitmap01->boundingBoxes[region - 1].min_y =
                    get_min(bitmap01->boundingBoxes[region - 1].min_y, i);
            }
        }

    // printf("we are in bounding boxes 3\n");

    for (int i = 0; i < bitmap01->nbRegions; i++)
        if (large_enough(&bitmap01->boundingBoxes[i]))
        {
            int x = bitmap01->boundingBoxes[i].min_x;
            int y = bitmap01->boundingBoxes[i].min_y;
            int w = bitmap01->boundingBoxes[i].max_x - bitmap01->boundingBoxes[i].min_x;
            int h = bitmap01->boundingBoxes[i].max_y - bitmap01->boundingBoxes[i].min_y;
            char *object_name = bitmap01->calibration_code;

            if (x && y)
                printf("Detected %s: %d %d %d %d\n", object_name, x + 1, y + 1, w - 1, h - 1);
        }
}

void generate_blackwhite(Bmp *bmp, Bitmap01 *bitmap01, Calibration cal)
{
    // update bitmap01 from bmp and cal
    bitmap01->w = bmp->width;
    bitmap01->h = bmp->height;

    bitmap01->pixels = malloc(bitmap01->h * sizeof(int *));
    if (bitmap01->pixels == NULL)
    {
        fprintf(stderr, "out of memory 1\n");
        exit(0);
    }
    for (int i = 0; i < bitmap01->h; i++)
    {
        bitmap01->pixels[i] = malloc(bitmap01->w * sizeof(int));
        if (bitmap01->pixels[i] == NULL)
        {
            fprintf(stderr, "out of memory 2\n");
            exit(0);
        }
    }

    // bitmap01->pixels = malloc(bitmap01->w * bitmap01->h * sizeof(int));
    strcpy(bitmap01->calibration_code, cal.Objects);

    HSV hsv;
    for (int i = 0; i < bmp->height; i++)
    {
        for (int j = 0; j < bmp->width; j++)
        {
            hsv = rgb2hsv(bmp->pixels[i][j]);
            int diff = hue_difference(hsv.hue, cal.Hue);
            if (diff <= cal.MaxDiff)
            {
                bitmap01->pixels[i][j] = 1;
            }
            else
            {
                bitmap01->pixels[i][j] = 0;
            }
        }
    }
}

void write_threshold_image(Bmp *bmp, Bitmap01 *bitmap01)
{
    for (int i = 0; i < bmp->height; i++)
    {
        for (int j = 0; j < bmp->width; j++)
        {
            for (int k = 0; k < 3; k++)
                if (bitmap01->pixels[i][j] == 1)
                    bmp->pixels[i][j][k] = 255;
                else
                    bmp->pixels[i][j][k] = 0;
        }
    }
}

void draw_boxes(Bmp bmp, Bitmap01 bitmap01)
{
    for (int j = 0; j < bitmap01.nbRegions; j++)
    {
        if (large_enough(&bitmap01.boundingBoxes[j]))
        {
            // printf("we are drawing a box\n");
            BoundingBox box = bitmap01.boundingBoxes[j];

            draw_box(bmp, box.min_x, box.min_y, box.max_x - box.min_x, box.max_y - box.max_y);

            // draw_box(bmp, box.min_x, box.min_y, box.max_x - box.min_x, box.max_y - box.max_y);
        }
    }
}

void read_file_final(char *calibrationFilePath)
{
}
int main(int argc, char **argv)
{
    if (check_input(argc, argv) == false)
        return 0;

    char *mode = argv[1];

    // Mode s: Show calibration
    if (strcmp(mode, "s") == 0 || strcmp(mode, "d") == 0)
    {
        // Get calibration file path from argument vector.
        char *calibrationFilepath = argv[2];

        FILE *file = freopen(calibrationFilepath, "r", stdin);

        Calibration temp;
        temp.Hue = -1;

        listCalibration = malloc(10 * sizeof(Calibration));
        nbCalibration = 0;

        while (!feof(file))
        {

            scanf("%s", temp.Objects);

            if (temp.Objects[0] == ')')
                strcpy(temp.Objects, temp.Objects + 1);

            if (strcmp(temp.Objects, "") == 0)
                break;

            scanf("%d%d%d%d", &temp.Hue, &temp.MaxDiff, &temp.MinSat, &temp.MinVal);

            if (temp.Hue != -1 && temp.Objects[0] != '<')
            {

                listCalibration[nbCalibration].Hue = temp.Hue;
                listCalibration[nbCalibration].MaxDiff = temp.MaxDiff;
                listCalibration[nbCalibration].MinSat = temp.MinSat;
                listCalibration[nbCalibration].MinVal = temp.MinVal;
                strcpy(listCalibration[nbCalibration].Objects, temp.Objects);

                nbCalibration++;
                // printf("%s: Hue: %d (Max. Diff: %d), Min. SV: %d %d\n", temp.Objects, temp.Hue, temp.MaxDiff, temp.MinSat, temp.MinVal);
            }

            strcpy(temp.Objects, "");
            temp.Hue = -1;
        }

        // test_reading_file(calibrationFilepath);

        // exit(0);
        // read calibrations into an array
        // listCalibration = read_calibration(calibrationFilepath, &nbCalibration);

        // listCalibration = read_calibration_with_function_new(calibrationFilepath, &nbCalibration);

        // printf("Number of calibration: %d\n", nbCalibration);

        if (strcmp(mode, "s") == 0)
        {

            printf("Calibrated Objects:\n");

            displayCalibration();
            return 0;
        }
        else // mode == "d" - detect object
        {
            // displayCalibration();

            char *bitmap_file = argv[3];

            Bmp bmp = read_bmp(bitmap_file);

            Bitmap01 *bitmap01s = malloc(nbCalibration * sizeof(Bitmap01));

            Bmp bmp_with_boxes = copy_bmp(bmp);
            Bmp bmp_with_threshold = copy_bmp(bmp);

            // write_bmp(bmp_with_boxes, "zobox.bmp");
            // write_bmp(bmp, "zoinput.bmp");

            // printf("Size: %d %d\n", bmp.height, bmp.width);
            for (int i = 0; i < nbCalibration; i++)
            {

                generate_blackwhite(&bmp, &bitmap01s[i], listCalibration[i]);

                write_threshold_image(&bmp_with_threshold, &bitmap01s[i]);

                generate_regions(&bitmap01s[i]);

                bounding_boxes(&bitmap01s[i]);

                draw_boxes(bmp_with_boxes, bitmap01s[i]);
            }

            // write_bmp(bmp_with_threshold, "zthreshold.bmp");

            // write_bmp(bmp_with_boxes, "zbox.bmp");
        }
    } // mode == "c" - calibration
    else
    {
        char *calibrationCode = argv[2];
        char *calibrationBitMap = argv[3];

        calibrate_color_profile(calibrationCode, calibrationBitMap, "test.txt");
    }
}

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "bitmap.c"

typedef struct{
    char Objects[255];
    int Hue, MaxDiff, MinSat, MinVal;
} Calibration;

bool checkInput(int argc, char *mode, char *filename){
    if (argc > 3){
        printf("Incorrect input.\n");
        return false;
    }
 
    if (strlen(mode) > 1){
        printf("Incorrect input.\n");
        return false;
    }
 
    if (mode[0] != 'c' && mode[0] != 's' && mode[0] != 'd'){
        printf("Incorrect input.\n");
        return false;
    }

    // Check valid file.
    if (freopen(filename, "r", stdin) == NULL){
        printf("Could not open calibration file.\n");
        return false;
    }
 
    return true;
}

void readCalibration(Calibration *cal){
    scanf("%s", cal->Objects);
    scanf("%d%d%d%d", &cal->Hue, &cal->MaxDiff, &cal->MinSat, &cal->MinVal);
}

void printCalibration(Calibration *cal){
    // Check read successful
    if(cal->Hue == -1){
        return;
    }
    printf("%s: Hue: %d (Max. Diff %d), Min. SV: %d %d\n", cal->Objects, cal->Hue, cal->MaxDiff, cal->MinSat, cal->MinVal);
}

void ShowCalibration(char *filename){
    // Open file to read
    FILE *file = freopen(filename, "r", stdin);

    printf("Calibrated objects:\n");

    // Declare a list of Calibration variable to hold the Calibration data.
    Calibration *listCal;
    
    int calIndex = 0;
    listCal = malloc(sizeof(Calibration));

    // Read until end of file.
    while(!feof(file)){
        // Read calibration i
        readCalibration(listCal + calIndex);

        // Move to the next calibration
        calIndex++;

        // Make room for another calibration
        listCal = realloc(listCal, (calIndex + 1)*sizeof(Calibration));

        // Reset the Calibration variable.
        (listCal + calIndex)->Hue = -1;
    }

    for(int i = 0; i < calIndex; i++) {
        printCalibration(listCal + i);
    }
    
    fclose(file);
    free(file);
}

void Calibration(char *filename){
    Bmp bmp = read_bmp("images/coin.bmp");
    int midX = bmp.width/2;
    int midY = bmp.height/2;

    HSV center[50][50];
    for(int i = midY - 25, m = 0; i < midY + 25 && m < 50; i++, m++) {
        for(int j = midX - 25, n = 0; j < midX + 25 && n < 50; j++, n++) {
            center[m][n] = rgb2hsv(bmp.pixels[i][j]);
        }
    }

    for(int i = 0; i < 50; i++) {
        for(int j = 0; j < 50; j++) {
            printf("(%d %d %d) ", center[i][j].hue, center[i][j].saturation, center[i][j].value);
        }
        printf("\n");
    }
}

void Detection(){

}

int main(int argc, char **argv){
    // Check input
    checkInput(argc, mode, filename);   
    char *mode = argv[1];
    char *imagesFilepath = argv[argc - 1];

    // Mode s: Show calibration
    if(strcmp(mode, "s") == 0){
        // Get calibration file path from argument vector.
        char *calibrationFilepath = argv[2];
        ShowCalibration(calibrationFilepath);
    }else if(strcmp(mode, "d") == 0){
        Calibration();
    }else if(strcmp(mode, "c") == 0){
        Detection();
    }
}



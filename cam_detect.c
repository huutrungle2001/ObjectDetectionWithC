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

void ShowCalibration(int argc, char *mode, char *filename){
    // Check input
    checkInput(argc, mode, filename);    
    
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

int main(int argc, char **argv){
    char *mode = argv[1];
    char *imagesFilepath = argv[argc - 1];

    // Mode s: Show calibration
    if(strcmp(mode, "s") == 0){
        // Get calibration file path from argument vector.
        char *calibrationFilepath = argv[2];
        ShowCalibration(argc, mode, calibrationFilepath);
    }else if(strcmp(mode, "d") == 0){

    }else if(strcmp(mode, "c") == 0){

    }
    
    // Bmp bmp = read_bmp("images/blueblob.bmp");
    // for(int i = 0; i < bmp.height; i++){
    //     for(int j = 0; j < bmp.width; j++){
    //         for(int k = 0; k < 3; k++){
    //             printf("%d,", bmp.pixels[i][j][k]);
    //         }
    //         printf(" ");
    //     }
    //     printf("\n");
    // }
}



#include <stdio.h>
#include <string.h>
#include "bitmap.c"

typedef struct{
    char Objects[255];
    int Hue, MaxDiff, MinSat, MinVal;
} Calibration;

void printCalibration(Calibration cal){
    printf("%s: Hue: %d (Max. Diff %d), Min. SV: %d %d\n", cal.Objects, cal.Hue, cal.MaxDiff, cal.MinSat, cal.MinVal);
}

void ShowCalibration(int argc, char* filename){
    // Check argument count.
        if(argc > 3){
            printf("Incorrect input.\n");
            return;
        }
        
        // Open file to read
        FILE *file = fopen(filename, "r");

        // Check valid file.
        if(file == NULL){
            printf("Could not open calibration file.\n");
            return;
        }

        printf("Calibrated objects:\n");

        // Declare a Calibration variable to hold the Calibration data.
        Calibration cal;

        // Read and print until end of file.
        while(fread(&cal, sizeof(Calibration), 1, file) != NULL){
            printf("%s: Hue: %d (Max. Diff %d), Min. SV: %d %d\n", cal.Objects, cal.Hue, cal.MaxDiff, cal.MinSat, cal.MinVal);
        }
        
        fclose(file);
}

int main(int argc, char **argv){
    char *mode = argv[1];
    char *imagesFilepath = argv[argc - 1];

    // Mode s: Show calibration
    if(strcmp(mode, "s") == 0){
        // Get calibration file path from argument vector.
        char *calibrationFilepath = argv[2];

        ShowCalibration(argc, calibrationFilepath);
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



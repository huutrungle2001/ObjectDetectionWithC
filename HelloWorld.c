#include <stdio.h>
#include "bitmap.c"

int main(){
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
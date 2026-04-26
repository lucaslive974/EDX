#include "helpers.h"
#include <math.h>

typedef unsigned int uint;
typedef int KERNEL[3][3];

void copy_image(int height, int width, RGBTRIPLE src[height][width], RGBTRIPLE dst[height][width]);

void sum_RGBT(uint* red, uint* green, uint* blue, int* avg_cnt, RGBTRIPLE value);

double sobel_filter(int gX, int gY);

void calculate_filter(int* arr, int line, int col, RGBTRIPLE value);

static inline int min(int a, int b);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            RGBTRIPLE value = image[i][j];
            BYTE gray_scale =  round((value.rgbtRed + value.rgbtGreen + value.rgbtBlue) / 3.0);
            RGBTRIPLE newValue = { gray_scale, gray_scale, gray_scale };
            image[i][j] = newValue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for(int i = 0; i < height; i++) {
        int left = 0, right = width - 1;
        while(left < right) {
            RGBTRIPLE tmp = image[i][right];
            image[i][right] = image[i][left];
            image[i][left] = tmp;

            left++;
            right--;
        }
    }
    return;
}

void sum_RGBT(uint* red, uint* green, uint* blue, int* avg_cnt, RGBTRIPLE value) {
    (*red) += value.rgbtRed;
    (*green) += value.rgbtGreen;
    (*blue) += value.rgbtBlue;
    (*avg_cnt)++;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp[height][width];
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            unsigned int red = 0, green = 0, blue = 0;
            int cnt = 0;

            int upper = i - 1, lower = i + 1;
            int left  = j - 1, right = j + 1;

            //Check upper row;
            if(upper >= 0) {
                if(left >= 0) sum_RGBT(&red, &green, &blue, &cnt, image[upper][left]);
                if(right < width) sum_RGBT(&red, &green, &blue, &cnt, image[upper][right]);
                sum_RGBT(&red, &green, &blue, &cnt, image[upper][j]);
            }

            //Left and Right on middle
            if(left >= 0) sum_RGBT(&red, &green, &blue, &cnt, image[i][left]);
            if(right < width) sum_RGBT(&red, &green, &blue, &cnt, image[i][right]);
            sum_RGBT(&red, &green, &blue, &cnt, image[i][j]);


            //Check lower row
            if(lower < height) {
                if(left >= 0) sum_RGBT(&red, &green, &blue, &cnt, image[lower][left]);
                if(right < width) sum_RGBT(&red, &green, &blue, &cnt, image[lower][right]);
                sum_RGBT(&red, &green, &blue, &cnt, image[lower][j]);
            }

            tmp[i][j].rgbtRed = round(red / (float) cnt);
            tmp[i][j].rgbtGreen = round(green / (float) cnt);
            tmp[i][j].rgbtBlue = round(blue / (float) cnt);
        }
    }

    copy_image(height, width, tmp, image);

    return;
}

double sobel_filter(int gX, int gY)  {
    return round(sqrt(pow(gX, 2) + pow(gY, 2)));
}


void calculate_filter(int* arr, int line, int col, RGBTRIPLE value) {
    static KERNEL kernelX = {
       { -1,  0,  1 },
       { -2,  0,  2 },
       { -1,  0,  1 }
    };

    static KERNEL kernelY = {
        { -1, -2, -1 },
        {  0,  0,  0 },
        {  1,  2,  1 }
    };
    //RED GX GY
    arr[0] += value.rgbtRed * kernelX[line][col];
    arr[1] += value.rgbtRed * kernelY[line][col];

    //GREEN GX GY
    arr[2] += value.rgbtGreen * kernelX[line][col];
    arr[3] += value.rgbtGreen * kernelY[line][col];

    //BLUE GX GY
    arr[4] += value.rgbtBlue * kernelX[line][col];
    arr[5] += value.rgbtBlue * kernelY[line][col];
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp[height][width];
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            //  RED    GREEN    BLUE
            // GX GY   GX GY    GX GY
            int arrXY[6] = { 0, 0, 0, 0, 0, 0 };

            int upper = i - 1, lower = i + 1;
            int left  = j - 1, right = j + 1;

            //Check upper row;
            if(upper >= 0) {
                if(left >= 0) calculate_filter(arrXY, 0, 0, image[upper][left]);
                if(right < width) calculate_filter(arrXY, 0, 2, image[upper][right]);
                calculate_filter(arrXY, 0, 1, image[upper][j]);
            }

            //Check left and right
            if(left >= 0) calculate_filter(arrXY, 1, 0, image[i][left]);
            if(right < width) calculate_filter(arrXY, 1, 2, image[i][right]);

            //Check lower row
            if(lower < height) {
                if(left >= 0) calculate_filter(arrXY, 2, 0, image[lower][left]);
                if(right < width) calculate_filter(arrXY, 2, 2, image[lower][right]);
                calculate_filter(arrXY, 2, 1, image[lower][j]);
            }

            tmp[i][j].rgbtRed =   min(sobel_filter(arrXY[0], arrXY[1]), 255);
            tmp[i][j].rgbtGreen = min(sobel_filter(arrXY[2], arrXY[3]), 255);
            tmp[i][j].rgbtBlue =  min(sobel_filter(arrXY[4], arrXY[5]), 255);
        }
    }

    copy_image(height, width, tmp, image);

    return;
}

void copy_image(int height, int width, RGBTRIPLE src[height][width], RGBTRIPLE dst[height][width]) {
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            dst[i][j] = src[i][j];
        }
    }
}

static inline int min(int a, int b) {
    return (a < b) ? a : b;
}

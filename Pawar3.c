/* Assignment 3, Pawar, Tinyc */
#include "PPMTools.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//int ReadPPM(const char *FileName, IMAGE *Image);
/*int ReadPPMHeader(FILE *fp, IMAGE *Image);
int ReadPPMPixel(FILE *fp, IMAGE *Image);
int WritePPM(const FILE *fp, const IMAGE *Image);
int WritePPMHeader(FILE *fp, const IMAGE *Image);
int WritePPMPixel(FILE *fp, const IMAGE *Image); */

//creating the ppm


/*int CreatPPM(IMAGE *Image);


int PutRPixel(const IMAGE *Image, int i, int j, int R);
int PutGPixel(const IMAGE *Image, int i, int j, int G);
int PutBPixel(const IMAGE *Image, int i, int j, int B);
int nearest_int(double x);
int max_int(int a, int b);
int min_int(int a, int b); */

//declare the nearest value 

int closest_value(double output_value)
{
    if (fabs(fabs(output_value) - fabs(floor(output_value))) > 0.5)
    {
        return ceil(output_value); //returning the output value of ceil
    }

    return floor(output_value);//returning the output value of floor
}

int mask(int k, int s, int x, int y)
{
    double temporary_variable, output_value;//here we are declaringthe temporary variable and the variable to get the output as output_value
    temporary_variable = (double)(x * x + y * y) / (double)(s * s);//we are storing the value in the temporary variable
    output_value = (double)k * (2.0 - temporary_variable) * exp(-temporary_variable / 2.0);//we are performing the operations and the value is stored in the output_value 
    return(closest_value(output_value));//we are retuning the nearest value of the output_value generated
}
int getValue(int* buffer, int x, int y, int color, int Column_values1)//initializing x,y,color and column_values1 to get the output
{
    return buffer[y * Column_values1 * 3 + x * 3 + color];//returning the buffer value
}
/*// Generate Mask
//Horizontal mask
 h_mask = (int **)malloc(mask_size * sizeof(int *));
 for (i = 0; i < mask_size; i++)
 h_mask[i] = (int *)malloc(mask_size * sizeof(int)); */

//vertical mask

 /* v_mask = (int **)malloc(mask_size * sizeof(int *));
 for (i = 0; i < mask_size; i++)
 v_mask[i] = (int *)malloc(mask_size * sizeof(int)); */

void generate_mask(int filter[][100], int Size_Of_Mask, int k, int s)//generating the mask value
{
    printf("LoG mask values generating \n");

    for (int i = -Size_Of_Mask; i <= Size_Of_Mask; i++)
    {
        for (int j = -Size_Of_Mask; j <= Size_Of_Mask; j++)
        {
            filter[i + Size_Of_Mask][j + Size_Of_Mask] = mask(k, s, i, j);
            printf("%d ", filter[i + Size_Of_Mask][j + Size_Of_Mask]);
        }
        printf("\n");
    }

    printf("LoG mask generated \n");//here we are printing the values of the LoG mask generated 
}
/*
// Put Pixel
 for (i = 0; i < image->Height; i++)
 {
 for (j = 0; j < image->Width; j++)
 {
 temp[i][j] = h_filter[i][j];
 }
 } */
 
 //int ReadPPMPixel(FILE *fp, IMAGE *Image)

 
void applyFilter(unsigned char* image, int* buffer, int filter[][100], int Column_values1, int Row_values1, int width) {
    printf("filter applied\n");
    for (int First_Image1 = 0; First_Image1 < Column_values1; First_Image1++) {
        for (int Second_Image1 = 0; Second_Image1 < Row_values1; Second_Image1++) {

            int red = 0, green = 0, blue = 0;//first initializing the the red,green and blue color values to 0

            for (int i = 0; i < width; i++) {
                for (int j = 0; j < width; j++) {

                    int pixelx = (First_Image1 - width / 2 + i);
                    int pixely = (Second_Image1 - width / 2 + j);
                    int colorr1, colorb1, colorg1;
                    if (pixelx < 0 || pixelx >= Row_values1 || pixely < 0 || pixely >= Column_values1) {
                        colorr1 = 255;//color of red has 8bits so the values must be from 0-255
                        colorb1 = 255;//color of blue hass 8bits so the values must be from 0-255
                        colorg1 = 255;//color of green hass 8bits so the values must be from 0-255
                    }
                    else {
                        colorr1 = (int)GetRPixel(image, Column_values1, pixelx, pixely);
                        colorb1 = (int)GetBPixel(image, Column_values1, pixelx, pixely);
                        colorg1 = (int)GetGPixel(image, Column_values1, pixelx, pixely);
                    }

                    red += (colorr1 * filter[i][j]); //assigning the pixel values of red
                    blue += (colorb1 * filter[i][j]); //assigning the pixel values of blue
                    green += (colorg1 * filter[i][j]);//assigning the pixel values of green
                }
            }

            buffer[First_Image1 * Column_values1 * 3 + Second_Image1 * 3] = red; //buffer for red surface detection
            buffer[First_Image1 * Column_values1 * 3 + Second_Image1 * 3 + 1] = blue; //buffer for blue surface detection
            buffer[First_Image1 * Column_values1 * 3 + Second_Image1 * 3 + 2] = green; //buffer for green surface detection
        }
    }
    printf("filter applied\n"); //generating the applied filter
}

/*red = (int)(GetRPixel(image, cols, m, n)); 
blue = (int)(GetBPixel(image, cols, m, n)); 
green = (int)(GetGPixel(image, cols, m, n)); */


/*for (i = 0; i < height; i++) {
 for (j = 0; j < width; j++) {
 if (mask[i][j] > threshold) {
 Image->Image[i][j].R = 0;
 Image->Image[i][j].G = 0;
 Image->Image[i][j].B = 0;
*/
//Applying threshold value
void applyThresholding(unsigned char* image1, int threshold, int* buffer, int Column_values1, int Row_values1) //we are adding the threshold value
{
    printf("applying the threshold value\n");
    for (int k = 0; k < 3; k++) {
        for (int i = 0; i < Column_values1; i++) {
            for (int j = 0; j < Row_values1; j++) {
                if ((i + 1 < Column_values1) && (getValue(buffer, i, j, k, Column_values1) < 0 && getValue(buffer, i + 1, j, k, Column_values1) > 0) || (getValue(buffer, i, j, k, Column_values1) > 0 && getValue(buffer, i + 1, j, k, Column_values1) < 0)) {
                    if (fabs((double)(getValue(buffer, i, j, k, Column_values1) + getValue(buffer, i + 1, j, k, Column_values1))) >= threshold)
                    {
                        PutRPixel(image1, Column_values1, j, i, 0); //plotting the Red pixel at i,j
                        PutGPixel(image1, Column_values1, j, i, 0); //plotting the green pixel at i,j
                        PutBPixel(image1, Column_values1, j, i, 0); //plotting the blue pixel at i,j
                    }
                }
                else if ((i + 2 < Column_values1) && (getValue(buffer, i, j, k, Column_values1) < 0 && getValue(buffer, i + 1, j, k, Column_values1) == 0 && getValue(buffer, i + 2, j, k, Column_values1) > 0) || (getValue(buffer, i, j, k, Column_values1) > 0 && getValue(buffer, i + 1, j, k, Column_values1) == 0 && getValue(buffer, i + 2, j, k, Column_values1) < 0)) {
                    if (fabs((double)(getValue(buffer, i, j, k, Column_values1) + getValue(buffer, i + 2, j, k, Column_values1))) >= threshold)
                    {
                        PutRPixel(image1, Column_values1, j, i, 0);
                        PutGPixel(image1, Column_values1, j, i, 0);
                        PutBPixel(image1, Column_values1, j, i, 0);
                    }
                }
            }
        }

        for (int i = 0; i < Column_values1; i++) {
            for (int j = 0; j < Row_values1; j++) {
                if ((j + 1 < Row_values1) && (getValue(buffer, i, j, k, Column_values1) < 0 && getValue(buffer, i, j + 1, k, Column_values1) > 0) || (getValue(buffer, i, j, k, Column_values1) > 0 && getValue(buffer, i, j + 1, k, Column_values1) < 0)) {
                    if (fabs((double)(getValue(buffer, i, j, k, Column_values1) + getValue(buffer, i, j + 1, k, Column_values1))) >= threshold)
                    {
                        PutRPixel(image1, Column_values1, j, i, 0);
                        PutGPixel(image1, Column_values1, j, i, 0);
                        PutBPixel(image1, Column_values1, j, i, 0);
                    }
                }
                else if ((j + 2 < Row_values1) && (getValue(buffer, i, j, k, Column_values1) < 0 && getValue(buffer, i, j + 1, k, Column_values1) == 0 && getValue(buffer, i, j + 2, k, Column_values1) > 0) || (getValue(buffer, i, j, k, Column_values1) > 0 && getValue(buffer, i, j + 1, k, Column_values1) == 0 && getValue(buffer, i, j + 2, k, Column_values1) < 0)) {
                    if (fabs((double)(getValue(buffer, i, j, k, Column_values1) + getValue(buffer, i, j + 2, k, Column_values1))) >= threshold)
                    {
                        PutRPixel(image1, Column_values1, j, i, 0);
                        PutGPixel(image1, Column_values1, j, i, 0);
                        PutBPixel(image1, Column_values1, j, i, 0);
                    }
                }
            }
        }
    }
    printf("applied threshold value\n"); //value of the threshold
}
/*// Convolution
 for (i = 0; i < kernel_height; i++) {
 for (j = 0; j < kernel_width; j++) {

  // declaring
 
 int y_pos = y + (kernel_height / 2) - i;
 int x_pos = x + (kernel_width / 2) - j;*/

void main()
{
    unsigned char* image, * image1;
    char filename[100];
    int Column_values1, Row_values1, i, j, k, s, threshold, filter[100][100], * buffer;
    unsigned int Size_Of_Mask;
    char red, blue, green;
    printf("Enter the name of the file : \n"); //entering the name of the file uploaded
    scanf("%s", filename);
    printf("Enter the size of mask : \n"); //we need to enter the size of the mask
    scanf("%d", &Size_Of_Mask);
    printf("Enter the value of k : \n"); //we need to add the value of k
    scanf("%d", &k);
    printf("Enter the value of s : \n"); //we need to add the value of s
    scanf("%d", &s);
    printf("Enter the threshold : \n"); // we need to enter the value og threshold
    scanf("%d", &threshold);
    generate_mask(filter, Size_Of_Mask, k, s);
    image = ReadPPM(filename, &Column_values1, &Row_values1);
    image1 = CreatePPM(Column_values1, Row_values1);
    buffer = (int*)malloc(Column_values1 * Row_values1 * 3 * sizeof(int));
    for (i = 0; i < Row_values1; i++)
    {
        for (j = 0; j < Column_values1; j++)
        {
            PutRPixel(image1, Column_values1, j, i, 255);
            PutGPixel(image1, Column_values1, j, i, 255);
            PutBPixel(image1, Column_values1, j, i, 255);
        }
    }
/*// Horizontal
 for ( y = 0; y < height; y++) {
 for ( x = 1; x < width - 1; x++) {
 if (abs(GetRPixel(output_image, x, y) - GetRPixel(output_image, x - 1, y)) > 
threshold ||
 abs(GetRPixel(output_image, x, y) - GetRPixel(output_image, x + 1, y)) > 
threshold) {
 PutRPixel(output_image, x, y, 0);
 PutGPixel(output_image, x, y, 0);
 PutBPixel(output_image, x, y, 0);
 }
 }
 } */

 /* Vertical Image
 // Horizontal
 for ( y = 0; y < height; y++) {
 for ( x = 1; x < width - 1; x++) {
 if (abs(GetRPixel(output_image, x, y) - GetRPixel(output_image, x - 1, y)) > 
threshold ||
 abs(GetRPixel(output_image, x, y) - GetRPixel(output_image, x + 1, y)) > 
threshold) {
 PutRPixel(output_image, x, y, 0);
 PutGPixel(output_image, x, y, 0);
 PutBPixel(output_image, x, y, 0);
 }
 }
 } */
    applyFilter(image, buffer, filter, Column_values1, Row_values1, 2 * Size_Of_Mask + 1); //applying the filter 
    applyThresholding(image1, threshold, buffer, Column_values1, Row_values1);// applying the thresholding values
    WritePPM("out.ppm", image1, Row_values1, Column_values1); //output of the ppm 
}
/* Assignment 2 , Pawar, Tiny C */
#include <stdio.h>
#include <stdlib.h>
typedef unsigned char byte;
void ReadPPM(FILE *fp, byte *red, byte *green, byte *blue, int *w1, int *h1);
void WritePPM(FILE *fp, byte *red, byte *green, byte *blue, int w1, int h1);
void CreatePPM(FILE *fp, byte *red, byte *green, byte *blue, int w1, int h1);
void MeanFilter(byte *red, byte *green, byte *blue, int w1, int h1);
void MedianFilter(byte *red, byte *green, byte *blue, int w1, int h1);
int main(void)
{
FILE *fp;
byte *red, *green, *blue;
int w1, h1;
fp = fopen("sample1.ppm", "rb");
if (fp == NULL)
{
 printf("File open error!\n");
 exit(1);
}
ReadPPM(fp, red, green, blue, &w1, &h1);
fclose(fp);
fp = fopen("mean.ppm", "wb");
if (fp == NULL)
{
 printf("File open error!\n");
 exit(1);
}
CreatePPM(fp, red, green, blue, w1, h1);
MeanFilter(red, green, blue, w1, h1);
WritePPM(fp, red, green, blue, w1, h1);
fclose(fp);
fp = fopen("median.ppm", "wb");
if (fp == NULL)
{
 printf("File open error!\n");
 exit(1);
}
CreatePPM(fp, red, green, blue, w1, h1);
MedianFilter(red, green, blue, w1, h1);
WritePPM(fp, red, green, blue, w1, h1);
fclose(fp);
return 0;
}
void ReadPPM(FILE *fp, byte *red, byte *green, byte *blue, int *w1, int *h1)
{
char buf[BUFSIZ];
int max_color;
fscanf(fp, "%s %d %d %d\n", buf, w1, h1, &max_color);
red = (byte *)calloc(*w1 * *h1, sizeof(byte));
green = (byte *)calloc(*w1 * *h1, sizeof(byte));
blue = (byte *)calloc(*w1 * *h1, sizeof(byte));
fread(red, sizeof(byte), *w1 * *h1, fp);
fread(green, sizeof(byte), *w1 * *h1, fp);
fread(blue, sizeof(byte), *w1 * *h1, fp);
}
void WritePPM(FILE *fp, byte *red, byte *green, byte *blue, int w1, int h1)
{
int i, j;
int max_color = 255;
fprintf(fp, "P6 %d %d %d\n", w1, h1, max_color);
for (i = 0; i < h1; i++)
{
 for (j = 0; j < w1; j++)
 {
  fputc(red[i * w1 + j], fp);
  fputc(green[i * w1 + j], fp);
  fputc(blue[i * w1 + j], fp);
 }
}
}
void CreatePPM(FILE *fp, byte *red, byte *green, byte *blue, int w1, int h1)
{
int i, j;
for (i = 0; i < h1; i++)
{
 for (j = 0; j < w1; j++)
 {
  red[i * w1 + j] = 255;
  green[i * w1 + j] = 255;
  blue[i * w1 + j] = 255;
 }
}
}
void MeanFilter(byte *red, byte *green, byte *blue, int w1, int h1)
{
int i, j, m, n, sumr, sumg, sumb;
for (i = 1; i < h1 - 1; i++)
{
 for (j = 1; j < w1 - 1; j++)
 {
  sumr = sumg = sumb = 0;
  for (m = -1; m <= 1; m++)
  {
   for (n = -1; n <= 1; n++)
   {
    sumr += red[(i + m) * w1 + (j + n)];
    sumg += green[(i + m) * w1 + (j + n)];
    sumb += blue[(i + m) * w1 + (j + n)];
   }
  }
  red[i * w1 + j] = (byte)(sumr / 9);
  green[i * w1 + j] = (byte)(sumg / 9);
  blue[i * w1 + j] = (byte)(sumb / 9);
 }
}
}
void MedianFilter(byte *red, byte *green, byte *blue, int w1, int h1)
{
int i, j, m, n, medianr, mediang, medianb, k;
byte R[9], G[9], B[9];
for (i = 1; i < h1 - 1; i++)
{
 for (j = 1; j < w1 - 1; j++)
 {
  k = 0;
  for (m = -1; m <= 1; m++)
  {
   for (n = -1; n <= 1; n++)
   {
    R[k] = red[(i + m) * w1 + (j + n)];
    G[k] = green[(i + m) * w1 + (j + n)];
    B[k] = blue[(i + m) * w1 + (j + n)];
    k++;
   }
  }
  for (m = 0; m < 9 - 1; m++)
  {
   for (n = m + 1; n < 9; n++)
   {
    if (R[m] > R[n])
    {
     byte temp = R[m];
     R[m] = R[n];
     R[n] = temp;
    }
    if (G[m] > G[n])
    {
     byte temp = G[m];
     G[m] = G[n];
     G[n] = temp;
    }
    if (B[m] > B[n])
    {
     byte temp = B[m];
     B[m] = B[n];
     B[n] = temp;
    }
   }
  }
  medianr = R[4];
  mediang = G[4];
  medianb = B[4];
  red[i * w1 + j] = (byte)medianr;
  green[i * w1 + j] = (byte)mediang;
  blue[i * w1 + j] = (byte)medianb;
 }
}
}
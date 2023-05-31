# Tiny c pawar assignment 3
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX 255
#define MAX_BUF_SIZE MAX
typedef struct
{
  unsigned char R;
  unsigned char G;
  unsigned char B;
} COLOR;
typedef struct
{
  int Width;
  int Height;
  int maxColorValue;
  COLOR **Image;
} IMAGE;
int ReadPPM(const char *FileName, IMAGE *Image);
int ReadPPMHeader(FILE *fp, IMAGE *Image);
int ReadPPMPixel(FILE *fp, IMAGE *Image);
int WritePPM(const FILE *fp, const IMAGE *Image);
int WritePPMHeader(FILE *fp, const IMAGE *Image);
int WritePPMPixel(FILE *fp, const IMAGE *Image);

int CreatPPM(IMAGE *Image);
int PutRPixel(const IMAGE *Image, int i, int j, int R);
int PutGPixel(const IMAGE *Image, int i, int j, int G);
int PutBPixel(const IMAGE *Image, int i, int j, int B);

int nearest_int(double x);
int max_int(int a, int b);
int min_int(int a, int b);
int main(int argc, char *argv[])
{
  IMAGE *image, *edge;
  image = (IMAGE *)malloc(sizeof(IMAGE));
  edge = (IMAGE *)malloc(sizeof(IMAGE));
  int mask_size, k, s, threshold, i, j, x, y, count1;
  char *FileName;
  char *h_file = "LoG_h.ppm";
  char *v_file = "LoG_v.ppm";
  char *out_file = "LoG.ppm";
  int **h_mask, **v_mask, **h_filter, **v_filter;
  int **tempvar;
  FileName = argv[1];
  mask_size = atoi(argv[2]);
  s = atoi(argv[3]);
  k = atoi(argv[4]);
  threshold = atoi(argv[5]);
// Reading the image
  if (!ReadPPM(FileName, image))
  {
    printf("Image Read Fail\n");
    return 0;
  }
  CreatPPM(edge);

  // We need to generate the mask
  h_mask = (int **)malloc(mask_size * sizeof(int *));
  for (i = 0; i < mask_size; i++)
    h_mask[i] = (int *)malloc(mask_size * sizeof(int));

  v_mask = (int **)malloc(mask_size * sizeof(int *));
  for (i = 0; i < mask_size; i++)
    v_mask[i] = (int *)malloc(mask_size * sizeof(int));

  tempvar = (int **)malloc(image->Height * sizeof(int *));
  for (i = 0; i < image->Height; i++)
    tempvar[i] = (int *)malloc(image->Width * sizeof(int));
  for (i = 0; i < image->Height; i++)
  {
    for (j = 0; j < image->Width; j++)
    {
      tempvar[i][j] = 0;
    }
  }

  int count1 = 0;
  for (i = -(mask_size / 2); i <= (mask_size / 2); i++)
  {
    for (j = -(mask_size / 2); j <= (mask_size / 2); j++)
    {
      h_mask[i + (mask_size / 2)][j + (mask_size / 2)] = mask(k, s, i, j);
      v_mask[j + (mask_size / 2)][i + (mask_size / 2)] = mask(k, s, i, j);
      // printf("%d ", h_mask[i + (mask_size / 2)][j + (mask_size / 2)]);
      // printf("%d ", v_mask[j + (mask_size / 2)][i + (mask_size / 2)]);
      count1++;
      if (count1 % 3 == 0)
      {
         printf("\n");
      }
    }
  }
  h_filter = (int **)malloc(image->Height * sizeof(int *));
  for (i = 0; i < image->Height; i++)
    h_filter[i] = (int *)malloc(image->Width * sizeof(int));

  for (i = 0; i < image->Height; i++)
  {
    for (j = 0; j < image->Width; j++)
    {
      h_filter[i][j] = 0;
    }
  }

  for (i = 0; i < image->Height; i++)
  {
    for (j = 0; j < image->Width; j++)
    {
      for (x = -(mask_size / 2); x <= (mask_size / 2); x++)
      {
        for (y = -(mask_size / 2); y <= (mask_size / 2); y++)
        {
          if (((i + x) >= 0) && ((i + x) < image->Height) && ((j + y) >= 0) && ((j + y) < image->Width))
          {
            h_filter[i][j] += image->Image[i + x][j + y].R * h_mask[x + (mask_size / 2)][y + (mask_size / 2)];
          }
        }
      }
    }
  }

  v_filter = (int **)malloc(image->Height * sizeof(int *));
  for (i = 0; i < image->Height; i++)
    v_filter[i] = (int *)malloc(image->Width * sizeof(int));

  for (i = 0; i < image->Height; i++)
  {
    for (j = 0; j < image->Width; j++)
    {
      v_filter[i][j] = 0;
    }
  }

  for (i = 0; i < image->Height; i++)
  {
    for (j = 0; j < image->Width; j++)
    {
      for (x = -(mask_size / 2); x <= (mask_size / 2); x++)
      {
        for (y = -(mask_size / 2); y <= (mask_size / 2); y++)
        {
          if (((i + x) >= 0) && ((i + x) < image->Height) && ((j + y) >= 0) && ((j + y) < image->Width))
          {
            v_filter[i][j] += image->Image[i + x][j + y].R * v_mask[x + (mask_size / 2)][y + (mask_size / 2)];
          }
        }
      }
    }
  }

  // We need to put the pixel
  for (i = 0; i < image->Height; i++)
  {
    for (j = 0; j < image->Width; j++)
    {
      tempvar[i][j] = h_filter[i][j];
    }
  }

  for (i = 0; i < image->Height; i++)
  {
    for (j = 0; j < image->Width; j++)
    {
      tempvar[i][j] += v_filter[i][j];
    }
  }

  for (i = 0; i < image->Height; i++)
  {
    for (j = 0; j < image->Width; j++)
    {
      if (tempvar[i][j] > threshold)
      {
        edge->Image[i][j].R = 0;
        edge->Image[i][j].G = 0;
        edge->Image[i][j].B = 0;
      }
      else
      {
        edge->Image[i][j].R = 255;
        edge->Image[i][j].G = 255;
        edge->Image[i][j].B = 255;
      }
    }
  }

  if (!WritePPM(h_file, edge))
  {
    printf("Image Write Fail\n");
    return 0;
  }

  // V Filter
  for (i = 0; i < image->Height; i++)
  {
    for (j = 0; j < image->Width; j++)
    {
      tempvar[i][j] = v_filter[i][j];
    }
  }

  for (i = 0; i < image->Height; i++)
  {
    for (j = 0; j < image->Width; j++)
    {
      tempvar[i][j] += h_filter[i][j];
    }
  }

  for (i = 0; i < image->Height; i++)
  {
    for (j = 0; j < image->Width; j++)
    {
      if (tempvar[i][j] > threshold)
      {
        edge->Image[i][j].R = 0;
        edge->Image[i][j].G = 0;
        edge->Image[i][j].B = 0;
      }
      else
      {
        edge->Image[i][j].R = 255;
        edge->Image[i][j].G = 255;
        edge->Image[i][j].B = 255;
      }
    }
  }

  if (!WritePPM(v_file, edge))
  {
    printf("Image Write Fail\n");
    return 0;
  }

  //Here is the output
  for (i = 0; i < image->Height; i++)
  {
    for (j = 0; j < image->Width; j++)
    {
      tempvar[i][j] = min_int(h_filter[i][j], v_filter[i][j]);
      tempvar[i][j] = max_int(h_filter[i][j], v_filter[i][j]);
    }
  }

  for (i = 0; i < image->Height; i++)
  {
    for (j = 0; j < image->Width; j++)
    {
      if (tempvar[i][j] > threshold)
      {
        edge->Image[i][j].R = 0;
        edge->Image[i][j].G = 0;
        edge->Image[i][j].B = 0;
      }
      else
      {
        edge->Image[i][j].R = 255;
        edge->Image[i][j].G = 255;
        edge->Image[i][j].B = 255;
      }
    }
  }

  if (!WritePPM(out_file, edge))
  {
    printf("Image Write Fail\n");
    return 0;
  }

  // free
  free(image);
  free(edge);
  free(tempvar);
  free(h_mask);
  free(v_mask);
  free(h_filter);
  free(v_filter);

  return 0;
}

int ReadPPM(const char *FileName, IMAGE *Image)
{
  FILE *fp;

  if ((fp = fopen(FileName, "rb")) == NULL)
  {
    printf("%s File open fail\n", FileName);
    return 0;
  }

  if (!ReadPPMHeader(fp, Image))
  {
    printf("%s File header read fail\n", FileName);
    return 0;
  }

  if (!ReadPPMPixel(fp, Image))
  {
    printf("%s File pixel read fail\n", FileName);
    return 0;
  }

  fclose(fp);
  return 1;
}

int ReadPPMHeader(FILE *fp, IMAGE *Image)
{
  char buf[MAX];
  int flag = 0;
  if ((fgets(buf, MAX, fp)) == NULL)
  {
    printf("PPM Header read fail\n");
    return 0;
  }

  if (strncmp(buf, "P6", 2) != 0)
  {
    printf("PPM File is not binary PPM\n");
    return 0;
  }

  while (1)
  {
    if ((fgets(buf, MAX, fp)) == NULL)
    {
      printf("PPM Header read fail\n");
      return 0;
    }

    if (buf[0] == '#')
    {
      continue;
    }

    if (flag == 0 && sscanf(buf, "%d %d", &Image->Width, &Image->Height) == 2)
    {
      flag++;
      continue;
    }

    if (flag == 1 && sscanf(buf, "%d", &Image->maxColorValue) == 1)
    {
      break;
    }
  }

  if (Image->maxColorValue != MAX)
  {
    printf("Color scale is not %d\n", MAX);
    return 0;
  }

  return 1;
}

int ReadPPMPixel(FILE *fp, IMAGE *Image)
{
  int i, j;

  if ((Image->Image = (COLOR **)malloc(Image->Height * sizeof(COLOR *))) == NULL)
  {
    printf("Memory allocation fail (1)\n");
    return 0;
  }

  for (i = 0; i < Image->Height; i++)
  {
    if ((Image->Image[i] = (COLOR *)malloc(Image->Width * sizeof(COLOR))) == NULL)
    {
      printf("Memory allocation fail (2)\n");
      return 0;
    }
  }

  for (i = 0; i < Image->Height; i++)
  {
    for (j = 0; j < Image->Width; j++)
    {
      if (((Image->Image[i][j].R) = fgetc(fp)) == EOF)
      {
        printf("PPM Pixel read fail\n");
        return 0;
      }
      if (((Image->Image[i][j].G) = fgetc(fp)) == EOF)
      {
        printf("PPM Pixel read fail\n");
        return 0;
      }
      if (((Image->Image[i][j].B) = fgetc(fp)) == EOF)
      {
        printf("PPM Pixel read fail\n");
        return 0;
      }
    }
  }

  return 1;
}

int ReadPPM(FILE *fp, IMAGE *Image)
{
  int i, max;
  char buf[MAX_BUF_SIZE];

  if ((fgets(buf, MAX_BUF_SIZE, fp)) == NULL)
  {
    printf("PPM File Header read fail\n");
    return 0;
  }

  if (buf[0] != 'P' || buf[1] != '6')
  {
    printf("PPM File Header is not correct\n");
    return 0;
  }

  if ((fgets(buf, MAX_BUF_SIZE, fp)) == NULL)
  {
    printf("PPM File Header read fail\n");
    return 0;
  }

  for (i = 0; i < (int)strlen(buf); i++)
  {
    if ((buf[i] < '0') || (buf[i] > '9'))
    {
      if (buf[i] == '#')
      {
        if ((fgets(buf, MAX_BUF_SIZE, fp)) == NULL)
        {
          printf("PPM File Header read fail\n");
          return 0;
        }
        continue;
      }
      else
      {
        printf("PPM File Header is not correct\n");
        return 0;
      }
    }
  }

  if (sscanf(buf, "%d %d", &(Image->Width), &(Image->Height)) != 2)
  {
    printf("PPM File Header read fail\n");
    return 0;
  }

  if ((fgets(buf, MAX_BUF_SIZE, fp)) == NULL)
  {
    printf("PPM File Header read fail\n");
    return 0;
  }

  for (i = 0; i < (int)strlen(buf); i++)
  {
    if ((buf[i] < '0') || (buf[i] > '9'))
    {
      if (buf[i] == '#')
      {
        if ((fgets(buf, MAX_BUF_SIZE, fp)) == NULL)
        {
          printf("PPM File Header  fail to read\n");
          return 0;
        }
        continue;
      }
      else
      {
        printf("PPM File Header incorrect\n");
        return 0;
      }
    }
  }

  if (sscanf(buf, "%d", &max) != 1)
  {
    printf("PPM File Header fail to read\n");
    return 0;
  }

  if (max != 255)
  {
    printf("PPM File Header is incorrect\n");
    return 0;
  }

  if ((ReadPPMPixel(fp, Image)) == 0)
  {
    printf("PPM File Header read fail\n");
    return 0;
  }

  return 1;
}

int WritePPMPixel(FILE *fp, const IMAGE *Image)
{
  int i, j;

  for (i = 0; i < Image->Height; i++)
  {
    for (j = 0; j < Image->Width; j++)
    {
      if ((fputc(Image->Image[i][j].R, fp)) == EOF)
      {
        printf("PPM Pixel write failed\n");
        return 0;
      }
      if ((fputc(Image->Image[i][j].G, fp)) == EOF)
      {
        printf("PPM Pixel write failed\n");
        return 0;
      }
      if ((fputc(Image->Image[i][j].B, fp)) == EOF)
      {
        printf("PPM Pixel write failed\n");
        return 0;
      }
    }
  }

  return 1;
}

int WritePPM(const FILE *fp, const IMAGE *Image)
{
  int i;
  char buf[MAX_BUF_SIZE];

  sprintf(buf, "P6 %d %d 255\n", Image->Width, Image->Height);

  if ((fputs(buf, fp)) == EOF)
  {
    printf("PPM File Header write failed\n");
    return 0;
  }

  if ((WritePPMPixel(fp, Image)) == 0)
  {
    printf("PPM Pixel write failed\n");
    return 0;
  }

  return 1;
}

int main(int argc, char **argv)
{
  FILE *fp;
  IMAGE *Image, *output_image, *image;
  char *infile, *outfile;
  int i, j, k, s, threshold, mask_size;

  if (argc != 2)
  {
    printf("Usage: %s <input image filename>\n", argv[0]);
    return 0;
  }

  infile = argv[1];

  if ((fp = fopen(infile, "rb")) == NULL)
  {
    printf("File open fail\n");
    return 0;
  }

  Image = (IMAGE *)malloc(sizeof(IMAGE));

  if ((ReadPPM(fp, Image)) == 0)
  {
    printf("Read PPM failed\n");
    return 0;
  }

  fclose(fp);

  int mask_height = Image->Height;
  int mask_width = Image->Width;
  int mask[mask_height][mask_width];

  printf("Type in a mask size: ");
  scanf("%d", &mask_size);

  printf("Type in a s value for mask function: ");
  scanf("%d", &s);

  printf("Type in a k value for mask function: ");
  scanf("%d", &k);

  printf("Type in a threshold value for edge detection: ");
  scanf("%d", &threshold);

  int kernel_size = (2 * mask_size + 1);
  int kernel[kernel_size][kernel_size];

  int sum = 0;

  for (int i = 0; i < kernel_size; i++) {
    for (int j = 0; j < kernel_size; j++) {
      kernel[i][j] = mask(k, s, i - mask_size, j - mask_size);
      sum = sum + kernel[i][j];
    }
  }

  for (i = 0; i < kernel_size; i++) {
    for (j = 0; j < kernel_size; j++) {
      kernel[i][j] = kernel[i][j] / sum;
    }
  }

  int kernel_height = kernel_size;
  int kernel_width = kernel_size;

  int height = Image->Height;
  int width = Image->Width;

  int x_padding = (kernel_size - 1) / 2;
  int y_padding = (kernel_size - 1) / 2;

  int padded_height = (height + 2 * y_padding);
  int padded_width = (width + 2 * x_padding);

  int padded_array[padded_height][padded_width];

  for (i = 0; i < kernel_height; i++) {
    for (j = 0; j < kernel_width; j++) {
      padded_array[i][j] = 0;
    }
  }

  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      padded_array[i + y_padding][j + x_padding] = Image->Image[i][j].R;
    }
  }

  for (i = 0; i < kernel_height; i++) {
    for (j = 0; j < kernel_width; j++) {
      padded_array[i][j + x_padding] = 0;
    }
  }

  for (i = 0; i < kernel_height; i++) {
    for (j = 0; j < kernel_width; j++) {
      padded_array[i + padded_height - kernel_height][j + x_padding] = 0;
    }
  }

  for (i = 0; i < kernel_height; i++) {
    for (j = 0; j < kernel_width; j++) {
      padded_array[i + y_padding][j] = 0;
    }
  }

  for (i = 0; i < kernel_height; i++) {
    for (j = 0; j < kernel_width; j++) {
      padded_array[i + y_padding][j + padded_width - kernel_width] = 0;
    }
  }

  for (i = y_padding; i < height + y_padding; i++) {
    for (j = x_padding; j < width + x_padding; j++) {
      sum = 0;
      for (k = 0; k < kernel_height; k++) {
        for (int l = 0; l < kernel_width; l++) {
          sum = sum + kernel[k][l] * padded_array[i - y_padding + k][j - x_padding + l];
        }
      }
      mask[i - y_padding][j - x_padding] = sum;
    }
  }

  int x_padding_2 = (kernel_size - 1);
  int y_padding_2 = (kernel_size - 1);

  int padded_height_2 = (height + 2 * y_padding_2);
  int padded_width_2 = (width + 2 * x_padding_2);

  int sum_2 = 0;

  int padded_array_2[padded_height_2][padded_width_2];

  for (i = 0; i < kernel_height; i++) {
    for (j = 0; j < kernel_width; j++) {
      padded_array_2[i][j] = 0;
    }
  }

  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      padded_array_2[i + y_padding_2][j + x_padding_2] = mask[i][j];
    }
  }

  for (i = 0; i < kernel_height; i++) {
    for (j = 0; j < kernel_width; j++) {
      padded_array_2[i][j + x_padding_2] = 0;
    }
  }

  for (i = 0; i < kernel_height; i++) {
    for (j = 0; j < kernel_width; j++) {
      padded_array_2[i + padded_height_2 - kernel_height][j + x_padding_2] = 0;
    }
  }

  for (i = 0; i < kernel_height; i++) {
    for (j = 0; j < kernel_width; j++) {
      padded_array_2[i + y_padding_2][j] = 0;
    }
  }

  for (i = 0; i < kernel_height; i++) {
    for (j = 0; j < kernel_width; j++) {
      padded_array_2[i + y_padding_2][j + padded_width_2 - kernel_width] = 0;
    }
  }

  for (i = y_padding_2; i < height + y_padding_2; i++) {
    for (j = x_padding_2; j < width + x_padding_2; j++) {
      sum_2 = 0;
      for (k = 0; k < kernel_height; k++) {
        for (int l = 0; l < kernel_width; l++) {
          sum_2 = sum_2 + kernel[k][l] * padded_array_2[i - y_padding_2 + k][j - x_padding_2 + l];
        }
      }
      mask[i - y_padding_2][j - x_padding_2] = sum_2;
    }
  }

  for (i = 0; i < kernel_height; i++) {
    for (j = 0; j < kernel_width; j++) {
      mask[i][j] = 0;
    }
  }

  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      if (mask[i][j] > threshold) {
        Image->Image[i][j].R = 0;
        Image->Image[i][j].G = 0;
        Image->Image[i][j].B = 0;
      }
    }
  }

  int x_padding_3 = (kernel_size - 1);
  int y_padding_3 = (kernel_size - 1);

  int sum_3 = 0;

  int padded_array_3[padded_height_2][padded_width_2];

  for (i = 0; i < kernel_height; i++) {
    for (j = 0; j < kernel_width; j++) {
      padded_array_3[i][j] = 0;
    }
  }

  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      padded_array_3[i + y_padding_3][j + x_padding_3] = mask[i][j];
    }
  }

  for (i = 0; i < kernel_height; i++) {
    for (j = 0; j < kernel_width; j++) {

        double tempvar, ans;
        tempvar = (double)(i * i + j * j) / (double)(s * s);
        ans = (double)k * (2.0 - tempvar) * exp(-tempvar / 2.0);
        kernel[i][j] = nearest_int(ans);

    }
  }

  for (i = 0; i < kernel_height; i++) {
    for (j = 0; j < kernel_width; j++) {
      printf("%3d ", kernel[i][j]); 
    }
    printf("\n");
  }
  printf("\n");
 #convolution
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {

      int sum_r = 0;
      int sum_g = 0;
      int sum_b = 0;
      
      // Convolution
      for (i = 0; i < kernel_height; i++) {
        for (j = 0; j < kernel_width; j++) {
          // declaring
          int y_pos = y + (kernel_height / 2) - i;
          int x_pos = x + (kernel_width / 2) - j;

          // Edge detection
          if (y_pos < 0 || y_pos >= height || x_pos < 0 || x_pos >= width) {
            continue;
          }

          sum_r += GetRPixel(image, x_pos, y_pos) * kernel[i][j];
          sum_g += GetGPixel(image, x_pos, y_pos) * kernel[i][j];
          sum_b += GetBPixel(image, x_pos, y_pos) * kernel[i][j];

        }
      }

      // Save the result of convolution
      PutRPixel(output_image, x, y, sum_r);
      PutGPixel(output_image, x, y, sum_g);
      PutBPixel(output_image, x, y, sum_b);
    }
  }
  ///global declaration
  int x,y;
  for ( y = 0; y < height; y++) {
    for ( x = 1; x < width - 1; x++) {
      if (abs(GetRPixel(output_image, x, y) - GetRPixel(output_image, x - 1, y)) > threshold ||
          abs(GetRPixel(output_image, x, y) - GetRPixel(output_image, x + 1, y)) > threshold) {
        PutRPixel(output_image, x, y, 0);
        PutGPixel(output_image, x, y, 0);
        PutBPixel(output_image, x, y, 0);
      }
    }
  }
  for (y = 1; y < height - 1; y++) {
    for (x = 0; x < width; x++) {
      if (abs(GetRPixel(output_image, x, y) - GetRPixel(output_image, x, y - 1)) > threshold ||
          abs(GetRPixel(output_image, x, y) - GetRPixel(output_image, x, y + 1)) > threshold) {
        PutRPixel(output_image, x, y, 0);
        PutGPixel(output_image, x, y, 0);
        PutBPixel(output_image, x, y, 0);
      }
    }
  }
  WritePPM(argv[6], output_image);
  FreeImage(image);
  FreeImage(output_image);

  return 0;
}
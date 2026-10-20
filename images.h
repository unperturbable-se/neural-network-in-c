#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

double* load_normalized_image(const char *filepath) 
{
    int width=28, height=28, channels;
    int total_pixels = width * height;
    unsigned char *raw_pixels = stbi_load(filepath, &width, &height, &channels, 1);
    double *normalized_features = (double*)malloc(sizeof(double) * total_pixels);
    for (int i = 0; i < total_pixels; i++) normalized_features[i] = (double)raw_pixels[i] / 255.0;
    stbi_image_free(raw_pixels);
    return normalized_features;
}

int* shuffle(size_t n) //get random indeces(sort image in random order to get unbiased prediction)
{
    int* array=(int*)malloc(sizeof(int)*n);
    for(int i=0;i<n;i++)array[i]=i;
    if (n > 1) 
    {
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
    return array;
}


void testPrint(char* filepath) 
{
    double* features= load_normalized_image(filepath);
    char* darkness=" ..**:;$B#";
    for (int i=0;i<28;i++)
        {
            for(int j=0;j<28;j++) 
                {
                    int val=ceil(features[i*28+j]*10);
                    val=(val<0)?0:(val>10)?10:val;
                    printf("%c",darkness[val]);
                    printf("%c",darkness[val]);
                    printf("%c",darkness[val]);
                    printf("%c",darkness[val]);
                }
            printf("\n");
        }
}

typedef struct image
{
  char* location;
  int value;
} image;

image* populateImages(char* dataset)
{
  // example: dataset="train"
  image* images;
  char cur_loc[100];
  char command[100];
  char buffer[100];
  images=(image*)malloc(sizeof(image)*60000);
  int index=0;
  for(char folder='0';folder<='9';folder++)
  {
    snprintf(cur_loc,100,"mnist_png/%s/%c",dataset,folder);
    snprintf(command,100,"ls %s | grep .png",cur_loc);
    FILE* fp=popen(command,"r");
    while(fgets(buffer,100,fp))
    {
        buffer[strcspn(buffer, "\n")] = '\0';
        asprintf(&images[index].location,"%s/%s",cur_loc,buffer);
        images[index].value=folder-'0';
        index++;
    }
    pclose(fp);
  }
  return images;
}
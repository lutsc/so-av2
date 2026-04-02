#include "pgm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

int read_pgm(const char* path, PGM* img){
  // Open file
  FILE* fd = fopen(path, "rb");
  if(fd == NULL){
    perror("Couldn't open file.");
    return -1;
  }

  // Ensure it is P5 and ignores comments
  char magic[3];
  if(fscanf(fd, "%2s", magic) != 1 || strcmp(magic, "P5") != 0){
    fprintf(stderr, "PGM format must be P5.\n");
    fclose(fd);
    return -1;
  }

  // Get values of w, h and maxv
  if (fscanf(fd, "%"PRIu32" %"PRIu32" %"PRIu8"", &img->w, &img->h, &img->maxv) != 3) {
    fprintf(stderr, "Invalid PGM header.\n");
    fclose(fd);
    return -1;
  }
  
  // Skip single whitespace character after maxv
  fgetc(fd);

  // Allocate memory for image data
  img->data = (unsigned char*)malloc(img->w * img->h);
  if(img->data == NULL){
    perror("Couldn't allocate memory for image data.");
    fclose(fd);
    return -1;
  }

  // Read image data and check if the correct amount of data was read
  size_t read = fread(img->data, 1, img->w * img->h, fd);
  if(read != (img->w * img->h)){
    fprintf(stderr, "Error reading image data.\n");
    free(img->data);
    fclose(fd);
    return -1;
  }

  fclose(fd);

  return 0;
}

int write_pgm(const char* path, const PGM* img)
{
  FILE * fd = fopen(path, "wb");

  fprintf(fd, "P5 ");
  fprintf(fd, "%"PRIu32" ", img->w);
  fprintf(fd, "%"PRIu32"\n", img->h);
  fprintf(fd, "%"PRIu8"\n", img->maxv);

  uint8_t temp;
  for(size_t i = 0; i < img->h; i++)
  {
    for(size_t j = 0; j < img->w; j++)
    {
      temp = img->data[j + ((img->w)*i)];
      fwrite(&temp, 1, 1, fd);
    }
  }

  fclose(fd);
  return 0;
}

PGM pgm_negative(PGM img)
{
  PGM img_negative;
  img_negative.w = img.w;
  img_negative.h = img.h;
  img_negative.maxv = img.maxv;
  img_negative.data = (unsigned char *)malloc(img.w*img.h);

  for(size_t i = 0; i < (img.w*img.h); i++)
  {
    img_negative.data[i] = (img.maxv - img.data[i]);
  }
  return img_negative;
}

PGM pgm_slice(PGM img, uint8_t x, uint8_t y)
{
  PGM img_slice;
  img_slice.w = img.w;
  img_slice.h = img.h;
  img_slice.maxv = img.maxv;
  img_slice.data = (unsigned char *)malloc(img.w*img.h);

  for(size_t i = 0; i < (img.w*img.h); i++)
  {
    if(img.data[i] >= x && img_slice.data[i] <= y)
    {
      img_slice.data[i] = img.data[i];
    }
    else
    {
      img_slice.data[i] = 0;
    }
  }
  return img_slice;
}


#include "library.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  if (fscanf(fd, "%d %d %d", &img->w, &img->h, &img->maxv) != 3) {
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

  const char * magic_number = "P5 ";
  char width[32];
  char height[32];
  char maxv[32];

  sprintf(width, "%d ", img->w);
  sprintf(height, "%d\n", img->h);
  sprintf(maxv, "%d\n", img->maxv);

  fwrite(magic_number, sizeof(char), strlen(width), fd);
  fwrite(width, sizeof(char), strlen(width), fd);
  fwrite(height, sizeof(char), strlen(width), fd);
  fwrite(maxv, sizeof(char), strlen(width), fd);

  uint8_t temp;
  for(size_t i = 0; i < img->h; i++)
  {
    for(size_t j = 0; j < img->w; j++)
    {
      temp = img->data[j + ((img->w-1)*i)];
      fwrite(&temp, sizeof(char), strlen(width), fd);
    }
  }

  fclose(fd);
  return 0;
}

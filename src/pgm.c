#include "pgm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <ctype.h>

void ignoreComments(FILE *fd){
  int ch;
  while((ch = fgetc(fd)) != EOF && isspace(ch)); // Skips whitespace
  if(ch == '#'){ // If is a comment
    while((ch = fgetc(fd)) != EOF && ch != '\n'); // Skips until end of line
    ignoreComments(fd); // Check for more comments
  }else{
    ungetc(ch, fd); // Returns ch if its not a comment
  }
} 

int read_pgm(const char* path, PGM* img){
  // Open file
  FILE* fd = fopen(path, "rb");
  if(fd == NULL){
    perror("Couldn't open file.");
    return -1;
  }

  // Ignore comments and read header values
  ignoreComments(fd);
  fscanf(fd, "%s", img->type);
  if(strcmp(img->type, "P5") != 0){
    fprintf(stderr, "PGM format must be P5.\n");
    fclose(fd);
    return -1;
  }

  ignoreComments(fd);
  if(fscanf(fd, "%"PRIu32"", &img->w) != 1){
    fprintf(stderr, "Invalid PGM Width.\n");
    fclose(fd);
    return -1;
  }

  ignoreComments(fd);
  if(fscanf(fd, "%"PRIu32"", &img->h) != 1){
    fprintf(stderr, "Invalid PGM Height.\n");
    fclose(fd);
    return -1;
  }

  ignoreComments(fd);
  if(fscanf(fd, "%"PRIu8"", &img->maxv) != 1){
    fprintf(stderr, "Invalid PGM Max Value.\n");
    fclose(fd);
    return -1;
  }
  
  // Skip single whitespace character after maxv
  fgetc(fd);

  // Allocate memory for image data
  size_t img_size = (size_t)img->w * (size_t)img->h;
  img->data = (unsigned char*)malloc(img_size);
  if(img->data == NULL){
    perror("Couldn't allocate memory for image data.");
    fclose(fd);
    return -1;
  }

  // Read image data and check if the correct amount of data was read
  size_t read = fread(img->data, 1, img_size, fd);
  if(read != (img_size)){
    fprintf(stderr, "Error reading image data.\n");
    free(img->data);
    fclose(fd);
    return -1;
  }

  fclose(fd);
  return 0;
}

int write_pgm(const char* path, const PGM* img){
  // Open file
  FILE* fd = fopen(path, "wb");
  if(fd == NULL){
    perror("Couldn't open file.");
    return -1;
  }

  // Write values of w, h and maxv
  fprintf(fd, "P5\n%"PRIu32" %"PRIu32"\n%"PRIu8"\n", img->w, img->h, img->maxv);

  // Write image data
  size_t img_size = (size_t)img->w * (size_t)img->h;
  size_t written = fwrite(img->data, 1, img_size, fd);
  if(written != img_size){
    fprintf(stderr, "Error writing image data.\n");
    fclose(fd);
    return -1;
  }

  fclose(fd);
  return 0;
}

PGM pgm_negative(PGM img){
  PGM img_negative;
  img_negative.w = img.w;
  img_negative.h = img.h;
  img_negative.maxv = img.maxv;
  size_t img_size = (size_t)img.w * (size_t)img.h;
  img_negative.data = (unsigned char*)malloc(img_size);

  for(size_t i = 0; i < img_size; i++){
    img_negative.data[i] = (img.maxv - img.data[i]);
  }
  return img_negative;
}

PGM pgm_slice(PGM img, uint8_t x, uint8_t y){
  PGM img_slice;
  img_slice.w = img.w;
  img_slice.h = img.h;
  img_slice.maxv = img.maxv;
  size_t img_size = (size_t)img.w * (size_t)img.h;
  img_slice.data = (unsigned char*)malloc(img_size);

  for(size_t i = 0; i < img_size; i++){
    if(img.data[i] <= x || img.data[i] >= y){
      img_slice.data[i] = 255;
    }else{
      img_slice.data[i] = img.data[i];;
    }
  }
  return img_slice;
}


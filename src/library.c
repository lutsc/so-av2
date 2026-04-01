#include "library.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// typedef struct {
//   uint32_t w, h; 
//   uint8_t maxv; // maxv = 255 
//   unsigned char* data; // w*h bytes (grayscale)
// } PGM; 


int read_pgm(const char* path, PGM* img)
{
  FILE * fd = fopen(path, "rb");
  if(fd == NULL)
  {
    perror("Couldn't open file");
    return -1;
  }

  char buffer[256];
  int ch;
  size_t i = 0;
  int arg = 0;

  while((ch = fgetc(fd)) != EOF) //Get magic number
  {
    if(ch == ' ' || ch == '\n' || ch == '\t')
    {
      buffer[i] = '\0';
      i = 0;
      switch(arg)
      {
        case 0:
          arg++;
          break;

        case 1:
          // printf("%d\n", atoi(buffer));
          img->w = atoi(buffer);
          arg++;
          break;

        case 2:
          // printf("%d\n", atoi(buffer));
          img->h = atoi(buffer);
          arg++;
          break;

        case 3:
          // printf("%d", atoi(buffer));
          img->maxv = atoi(buffer);
          arg++;
          break;
      }
    }
    if(arg > 3)
    {
      break;
    }
      buffer[i] = ch;
      i++;
  }
  img->data = (unsigned char *)malloc((img->w * img->h));

  while((ch = fgetc(fd)) != EOF)
  {
    img->data[i] = ch;
    i++;
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

#include <stdio.h>
#include <string.h>

#define H_INT 768
#define W_INT 1366

#define H "768 "
#define W "1366 "
#define MAXV "255\n"

int main()
{
  FILE * FD = fopen("output.pgm", "wb");

  const char * magic_number = "P5 ";
  const char * width = W;
  const char * height = H;
  const char * maxv = MAXV;

  fwrite(magic_number, sizeof(char), strlen(magic_number),FD);
  fwrite(width, sizeof(char), strlen(width),FD);
  fwrite(height, sizeof(char), strlen(height),FD);
  fwrite(maxv, sizeof(char), strlen(maxv),FD);

  int temp;
  for(int i = 0; i < H_INT; i++)
  {
    for(int j = 0; j < W_INT; j++)
    {
      temp = (i+j)%255;
      fwrite(&temp, sizeof(int), 1, FD);
    }
  }
  fclose(FD);

}

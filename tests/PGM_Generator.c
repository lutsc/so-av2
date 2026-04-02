#include <stdio.h>
#include <string.h>

#define W 1366
#define H 768
#define MAXV 255

int main()
{
  FILE * fd = fopen("output.pgm", "wb");

  fprintf(fd, "P5 ");
  fprintf(fd, "%d ", W);
  fprintf(fd, "%d\n", H);
  fprintf(fd, "%d\n", MAXV);

  int temp;
  for(int i = 0; i < H; i++)
  {
    for(int j = 0; j < W; j++)
    {
      temp = (i+j)%255;
      fwrite(&temp, sizeof(char), 1, fd);
    }
  }
  fclose(fd);

}

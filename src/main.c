#include "library.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
  PGM pgm;
  read_pgm("input.pgm", &pgm);
  write_pgm("output.pgm", &pgm);

  return 0;
}

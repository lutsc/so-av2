#include "pgm.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
  PGM pgm;
  read_pgm("input.pgm", &pgm);

  PGM neg = pgm_negative(pgm);
  PGM slice = pgm_slice(pgm, 100, 100);

  write_pgm("output_slice.pgm", &slice);
  write_pgm("output_neg.pgm", &neg);

  return 0;
}

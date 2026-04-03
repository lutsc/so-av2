#include "pgm.h"
#include "arguments.h"
#include <stdio.h>
#include <stdlib.h>

struct argp argp = {options, parse_opt, args_doc, doc};
int main(int argc, char* argv[])
{
  struct arguments arguments;
  arguments.silent = 0;
  arguments.verbose = 0;
  arguments.output_file = "-";

  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  PGM pgm;
  read_pgm("input.pgm", &pgm);

  PGM neg = pgm_negative(pgm);
  PGM slice = pgm_slice(pgm, 100, 110);

  write_pgm("output_slice.pgm", &slice);
  write_pgm("output_neg.pgm", &neg);

  return 0;
}

#include "pgm.h"
#include "arguments.h"
#include <stdio.h>
#include <stdlib.h>


struct argp argp = {options, parse_opt, args_doc, doc};
bool valid = 1; 

int main(int argc, char* argv[])
{
  struct arguments arguments;
  arguments.silent = 0;
  arguments.verbose = 0;
  arguments.range = "100-110";
  arguments.output_file = "output.pgm";

  argp_parse(&argp, argc, argv, 0, 0, &arguments);
  OP_Mode mode = parse_mode(arguments.args[0]);
  uint8_t t1, t2;

  const char * input = arguments.args[1];

  PGM pgm;
  read_pgm(input, &pgm);

  PGM neg = pgm_negative(pgm);

  if(mode == MODE_SLICE)
  {
    bool slice_result = parse_range(arguments.range, &t1, &t2);
    if(!slice_result)
    {
      PGM slice = pgm_slice(pgm, t1, t2);
      write_pgm(arguments.output_file, &slice);
    }
    else{
      exit(1);
    }
  }
  else if(mode == MODE_NEG)
  {
    write_pgm(arguments.output_file, &neg);
  }

  return 0;
}

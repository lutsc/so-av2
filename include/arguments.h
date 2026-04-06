#ifndef ARGUMENTS_H
#define ARGUMENTS_H 1

#ifndef bool
#define bool _Bool
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#include <argp.h>
#include "pgm.h"

#define argp_program_version "so-av2 1.0"
static char doc[] = "Avaliação de sistemas operacionais, implementação de filtors para arquivos PGM usando threads, sincronização e IPC";

static char args_doc[] = "COMMAND [ARG1] [ARG2]";  
static struct argp_option options[] =
{
  {"verbose", 'v', 0, 0, "Produce verbose output"},
  {"quiet", 'q', 0, 0, "Don't produce any output"},
  {"silent", 's', 0, OPTION_ALIAS},
  {"range", 'r', "RANGE", 0, "Specify the range for the slice operation (t1-t2)"},
  {"output", 'o', "FILE", 0, "Output to FILE instead of modifying input"},
  // {0},
};

OP_Mode parse_mode(char * mode);
int parse_range(char * range, uint8_t* t1, uint8_t* t2);

struct arguments
{
  char *args[2]; //args[0] = MODE; args[1] = INPUT_FILE
  bool silent, verbose;
  char *range;
  char *output_file;
};

error_t parse_opt (int key, char *arg, struct argp_state *state);
#endif

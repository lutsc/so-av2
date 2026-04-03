#ifndef ARGUMENTS_H
#define ARGUMENTS_H 1

#ifndef bool
#define bool _Bool
#endif

#include <argp.h>

#define argp_program_version "so-av2 1.0"
static char doc[] = "Avaliação de sistemas operacionais, implementação de filtors para arquivos PGM usando threads, sincronização e IPC";

static char args_doc[] = "COMMAND [ARG1] [OPTIONS] [ARG2]";  
static struct argp_option options[] =
{
  {"verbose", 'v', 0, 0, "Produce verbose output"},
  {"quiet", 'q', 0, 0, "Don't produce any output"},
  {"silent", 's', 0, OPTION_ALIAS},
  {"output", 'o', "FILE", 0, "Output to FILE instead of modifying input"},
  {0},
};

struct arguments
{
  char *args[2];
  bool silent, verbose;
  char *output_file;
};

static error_t parse_opt (int key, char *arg, struct argp_state *state);
#endif

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

// Worker arguments
static char doc_worker[] = "Avaliação de sistemas operacionais, implementação de filtros para arquivos PGM usando threads, sincronização e IPC (arquivo worker)";
static char args_doc_woker[] = "COMMAND [ARG1] [ARG2]"; //TODO: Update doc worker text

static struct argp_option options_worker[] =
{
  {"verbose", 'v', 0, 0, "Produce verbose output"},
  {"quiet", 'q', 0, 0, "Don't produce any output"},
  {"silent", 's', 0, OPTION_ALIAS},
  {"range", 'r', "RANGE", 0, "Specify the range for the slice operation (T1xT2)"},
  {"threads", 't', "THREADS", 0, "Specify the max number of threads to run"},
  {"fifo", 'f', "FIFO_PATH", 0, "Specify the fifo path used, if none is specified use /tmp/imgpath"},
  {"output", 'o', "FILE", 0, "Output to FILE instead of modifying input"},
  {0},
};


struct arguments_worker //TODO:Update the arguments_worker structure
{
  OP_Mode mode; // args[0] = MODE;
  bool silent, verbose;
  uint32_t threads; // Specifies the thread pool size
  uint8_t t1, t2; 
  char *fifo;
  char *output_file; // output.pgm by default
};

error_t parse_opt_worker (int key, char *arg, struct argp_state *state);

// Takes a string and extracts the mode of operation, returns OP_Mode on success and -1 on error
OP_Mode parse_mode(char * mode);

// Takes a string and extracts t1 and t2 from it, returns 0 on success and -1 on error
int32_t parse_range(char * range, uint8_t* t1, uint8_t* t2);

void init_args_worker(struct arguments_worker * args);

// Sender arguments
static char doc_sender[] = "Avaliação de sistemas operacionais, implementação de filtros para arquivos PGM usando threads, sincronização e IPC (arquivo sender)";
static char args_doc_sender[] = "COMMAND [ARG1] [ARG2]"; //TODO: Update doc sender text
static struct argp_option options_sender[] =
{
  {"verbose", 'v', 0, 0, "Produce verbose output"},
  {"quiet", 'q', 0, 0, "Don't produce any output"},
  {"silent", 's', 0, OPTION_ALIAS},
  {"fifo", 'f', "FIFO_PATH", 0, "Specify the fifo path used, if none is specified use /tmp/imgpath"},
  {0},
};

struct arguments_sender
{
  char *input_file; //args[0] = input_file
  char *fifo;
  bool silent, verbose;
};

error_t parse_opt_sender (int key, char *arg, struct argp_state *state);
void init_args_sender(struct arguments_sender * args);

#endif

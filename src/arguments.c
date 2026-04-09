#include "arguments.h"
#include <argp.h>
#include <string.h>
#include <stdlib.h>

int32_t parse_range(char * range, uint8_t* t1, uint8_t* t2)
{
  size_t break_index = 0;
  char t1_buffer[16] = {0};
  char t2_buffer[16] = {0};
  for(size_t i = 0; i < strlen(range); i++)
  {
    if(range[i] == 'x' || range[i] == '-' || range[i] == ' ' || range[i] == ',')
    {
      break_index = i;
      break;
    }
  }
  strncpy(t1_buffer, range, break_index);
  strncpy(t2_buffer, &range[break_index+1], strlen(range)-break_index);

  *t1 = atoi(t1_buffer);
  *t2 = atoi(t2_buffer);

  if (*t1 > *t2)
  {
    // perror("t2 must be greater than or equal to t1");
    return -1;
  }
  else if(*t1 < 0 || *t2 < 0)
  {
    // perror("Values must be positive integers");
    return -1;
  }
  return 0;
}

error_t parse_opt_worker (int key, char *arg, struct argp_state* state)
{
  struct arguments_worker *arguments = state->input;

  switch(key)
  {
    case 'q': case 's':
      arguments->silent = 1;
      break;
    case 'v':
      arguments->verbose = 1;
      break;
    case 'o':
      arguments->output_file = arg;
      break;
    case 'r':
      {
        int32_t result = parse_range(arg, &arguments->t1, &arguments->t2);
        if(result == -1) // Gets t1 and t2 from argument
        {
          argp_usage(state);
        }
      }
      break;
    case 't':
      arguments->threads = atoi(arg);
      break;

    case ARGP_KEY_ARG:
      if (state->arg_num >= 1) //Too many arguments
      {
        argp_usage(state);
      }
      arguments->mode = parse_mode(arg); // Gets mode from positional argument
      if(arguments->mode == -1)
      {
        argp_usage(state);
      }
      break;

    case ARGP_KEY_END:
      if (state->arg_num < 1) //Not enough arguments
      {
        argp_usage(state);
      }
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

OP_Mode parse_mode(char * mode)
{
  if(!strcmp(mode, "negative"))
  {
    return MODE_NEG;
  }
  if(!strcmp(mode, "slice"))
  {
    return MODE_SLICE;
  }
  return -1;
}

void init_args_worker(struct arguments_worker * args)
{
  args->silent = 0;
  args->verbose = 0;
  args->t1 = 100;
  args->t2 = 140;
  args->fifo = FIFO_PATH;
  args->output_file = "output.pgm";
  args->threads = 4;
}

error_t parse_opt_sender (int key, char *arg, struct argp_state* state)
{
  struct arguments_sender *arguments = state->input;

  switch(key) 
  {
    case 'q': case 's':
      arguments->silent = 1;
      break;
    case 'v':
      arguments->verbose = 1;
      break;
    case 'f':
      arguments->fifo = arg;
      break;
    case ARGP_KEY_ARG:
      if(state->arg_num >= 1)
      {
        argp_usage(state);
      }
      arguments->input_file = arg;
      break;
    case ARGP_KEY_END:
      if (state->arg_num < 1) //Not enough arguments
      {
        argp_usage(state);
      }
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

void init_args_sender(struct arguments_sender * args)
{
  args->fifo = FIFO_PATH;
  args->silent = 0;
  args->verbose = 0;
}







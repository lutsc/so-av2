#include "arguments.h"
#include <argp.h>
#include <string.h>
#include <stdlib.h>

error_t parse_opt (int key, char *arg, struct argp_state* state)
{
  struct arguments *arguments = state->input;

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
      arguments->range = arg;
      break;

    case ARGP_KEY_ARG:
      if (state->arg_num >= 2) //Too many arguments
      {
        argp_usage(state);
      }
      arguments->args[state->arg_num] = arg;
      break;

    case ARGP_KEY_END:
      if (state->arg_num < 2) //Not enough arguments
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

int parse_range(char * range, uint8_t* t1, uint8_t* t2)
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



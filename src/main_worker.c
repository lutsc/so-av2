#include "pgm.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

// Task Queue (circular) + synch
#define QMAX 128
Task queue_buf[QMAX];
int q_head = 0, q_tail = 0, q_count = 0;

pthread_mutex_t q_lock = PTHREAD_MUTEX_INITIALIZER;
// sem_t sem_items; // quantity of available task
// sem_t sem_space; // free space on the queue

pthread_mutex_t done_lock = PTHREAD_MUTEX_INITIALIZER;
// sem_t sem_done; // signal when tasks are done
// int remaining_tasks = 0;

// Shared data for processing
PGM g_in, g_out;
OP_Mode g_mode; // MODE_NEG or MODE_SLICE
int g_t1, g_t2;
int g_nthreads = 4;

int main_worker(int argc, char** argv) {

  // argv: img_worker <fifo_path> <output.pgm> <negative|slice> [t1 t2] [nthreads]

  // parse_args_or_exit(); //TODO:
  const char* fifo = argv[1];
  const char* outpth = argv[2];
  const char* mode = argv[3];

  if (mode == "negative") {
    g_mode = MODE_NEG;
    g_nthreads = (argc >= 5) ? atoi(argv[4]) : 4;
  } else if (mode == "slice") {
    g_mode = MODE_SLICE;
    g_t1 = atoi(argv[4]);
    g_t2 = atoi(argv[5]);
    g_nthreads = (argc >= 7) ? atoi(argv[6]) : 4;
  } else {
    // exit_error("Invalid mode. Use 'negative' or 'slice'."); //TODO:
  }

  // 1) Ensures FIFO exists and opens for reading (blocks until sender opens for writing)

  // 2) Reads header + pixels from FIFO
  
  // 3) Creates thread pool and task queue (doesn't need to be a thread pool)
  
  // 4) Waits for all tasks to finish
  
  // 5) Writes output image
  
  // 6) Frees resources
  
  // 7) End

  return 0;
}

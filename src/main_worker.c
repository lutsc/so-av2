#include "pgm.h"
#include "arguments.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/stat.h>

// Task Queue (circular) + synch
#define QMAX 128  
Task queue_buf[QMAX];

uint32_t q_count = 0;

sem_t sem;

// Shared data for processing
PGM g_in, g_out;
OP_Mode g_mode; // MODE_NEG or MODE_SLICE
uint8_t g_t1, g_t2;
uint32_t g_nthreads = 4;

// Filters
void apply_negative_block(size_t rs, size_t re){
  size_t i;
  for(size_t row = rs; row <= re; row++){
    for(size_t column = 0; column < g_in.w; column++){
      i = (g_in.w * row) + column;
      g_out.data[i] = (g_in.maxv - g_in.data[i]);
    }
  }
}

void apply_slice_block(size_t rs, size_t re, uint8_t t1, uint8_t t2){
  size_t i;
  for(size_t row = rs; row <= re; row++){
    for(size_t column = 0; column < g_in.w; column++){
      i = (g_in.w * row) + column;
      if(g_in.data[i] >= t1 && g_in.data[i] <= t2){
        g_out.data[i] = g_in.data[i];
      }else{
        g_out.data[i] = 0;
      }
    }
  }
}


// Thread
void* worker_thread(void* arg){
  while(1){

    sem_wait(&sem);
    Task task = queue_buf[q_count];
    q_count++;
    sem_post(&sem);

    uint32_t rs = task.row_start, re = task.row_end;
    if(g_mode == MODE_NEG)
    {
      apply_negative_block(rs, re);
    }
    else if(g_mode == MODE_SLICE)
    {
      apply_slice_block(rs,re, g_t1, g_t2);
    }
    if(q_count >= QMAX)
    {
      pthread_exit(NULL); 
    }
  } 
}

struct argp argp = {options_worker, parse_opt_worker, args_doc_woker, doc_worker};
int main(int argc, char** argv) {

  struct arguments_worker args;
  init_args_worker(&args);
  argp_parse(&argp, argc, argv, 0, 0, &args);

  g_mode = args.mode;
  g_t1 = args.t1;
  g_t2 = args.t2;
  g_nthreads = args.threads;


  // 1) Ensures FIFO exists and opens for reading (blocks until sender opens for writing)
  if(mkfifo(args.fifo, 0666) == -1 && errno != EEXIST){
    perror("Error creating FIFO.");
    return 1;
  }

  FILE* fd = fopen(args.fifo, "rb");
  if(fd == NULL) { 
    fprintf(stderr, "Error opening FIFO for reading.");
    return 1;
  }

  // 2) Reads header + pixels from FIFO
  Header header;
  fread(&header, sizeof(Header), 1, fd);
  g_in.w = header.w;
  g_in.h = header.h;
  g_in.maxv = header.maxv;

  size_t img_size = ((size_t)g_in.w * (size_t)g_in.h);
  g_in.data = (uint8_t*)malloc(img_size);
  if(g_in.data == NULL){
    fprintf(stderr, "Couldn't allocate memory for image data.\n");
    fclose(fd);
    return 1;
  }
  fread(g_in.data, 1, img_size, fd);
  
  // 3) Allocates space for the output image
  g_out.w = g_in.w;
  g_out.h = g_in.h;
  g_out.maxv = g_in.maxv;
  g_out.data = (uint8_t *)malloc(img_size);
  if(g_out.data == NULL){
    fprintf(stderr, "Couldn't allocate memory for output image data.\n");
    return -1;
  }

  // 4) Define what the tasks intervals should be
  int32_t Q_Rows;
  Q_Rows = header.h / QMAX;

  for (size_t i = 0; i < QMAX; i++)
  {
    queue_buf[i].row_start = i*(Q_Rows);
    queue_buf[i].row_end = i*(Q_Rows)+ (Q_Rows)-1;
  }
  queue_buf[QMAX-1].row_end = header.h-1;

  // 5) Creates thread pool and task queue 
  sem_init(&sem, 0, g_nthreads);
  pthread_t threads[g_nthreads];
  for(int i = 0; i < g_nthreads; i++){
    pthread_create(&threads[i], NULL, worker_thread, NULL);
  }

  // 6) Waits for all tasks to finish
  for(int i = 0; i < g_nthreads; i++){
    pthread_join(threads[i], NULL);
  }
  write_pgm(args.output_file, &g_out);
  
  // 7) Frees resources
  sem_destroy(&sem);
  free(g_in.data);
  free(g_out.data);
  fflush(fd);
  fclose(fd);

  return 0;
}

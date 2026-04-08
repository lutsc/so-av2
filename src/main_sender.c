#include "pgm.h"
#include <stdio.h>
// #include <pthread.h>
// #include <unistd.h>
#include <stdlib.h>
// #include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

// Basic structure for Sender
int main(int argc, char** argv){

  // argv: img_sender <fifo_path> <input.pgm>
  // Sender only sends image; worker does the filtering through its CLI

  // parse_args_or_exit(); //TODO:
  
  const char* fifo = argv[1];
  const char* inpath = argv[2];
  
  // 1) Ensure FIFO exists (mkfifo if necessary)
  if(mkfifo(fifo, 0666) == -1 && errno != EEXIST){
    perror("Error creating FIFO.");
    return 1;
  }
  
  // 2) Read PGM image (P5) from disk
  PGM pgm;
  read_pgm(inpath, &pgm);

  // 3) Prepare header (mode/t1/t2 are ignoned by the worker, send only image metadata) 
  Header header;
  header.w = pgm.w;
  header.h = pgm.h;
  header.maxv = pgm.maxv;
  
  // 4) Open FIFO for writing (blocked until worker opens for reading)
  FILE* fd = fopen(fifo, "wb");
  if(fd == NULL){
    perror("Error opening FIFO for writing.");
    free(pgm.data);
    return 1;
  }

  // 5) Send header + pixels
  if(fwrite(&header, sizeof(Header), 1, fd) != 1){
    fprintf(stderr, "Error writing header to FIFO.\n");
    fclose(fd);
    free(pgm.data);
    return 1;
  }
  
  size_t img_size = ((size_t)pgm.w * (size_t)pgm.h);
  if(fwrite(pgm.data, 1, img_size, fd) != img_size){
    fprintf(stderr, "Error writing image data to FIFO.\n");
    fclose(fd);
    free(pgm.data);
    return 1;
  }

  // 6) Close FIFO and free up memory 
  free(pgm.data);
  fflush(fd);
  fclose(fd);

  return 0;
}

#include "pgm.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

// Basic structure for Sender
int main_sender(int argc, char** argv) {

  // argv: img_sender <fifo_path> <input.pgm>
  // Sender only sends image; worker does the filtering through its CLI

  // parse_args_or_exit(); //TODO:
  
  const char* fifo = argv[1];
  const char* inpath = argv[2];
  
  // 1) Ensure FIFO exists (mkfifo if necessary)
  mkfifo(fifo, 0666);
  
  // 2) Read PGM image (P5) from disk  
  PGM pgm;
  read_pgm(inpath, &pgm);

  // 3) Prepare header (mode/t1/t2 are ignoned by the worker;
  // send only image metadata) 
  Header header;
  header.w = pgm.w;
  header.h = pgm.h;
  header.maxv = pgm.maxv;
  
  // 4) Open FIFO for writing (blocked until worker opens for reading)
  int fd = open(fifo, O_WRONLY);
  if(fd == -1) {
    perror("Error opening FIFO for writing.");
    free(pgm.data);
    return 1;
  }

  // 5) Send header + pixels
  size_t img_size = (size_t)pgm.w * (size_t)pgm.h;
  write(fd, &header, sizeof(Header));
  write(fd, pgm.data, img_size);
 
  // 6) Close FIFO and free up memory 
  close(fd);
  free(pgm.data);

  return 0;
}

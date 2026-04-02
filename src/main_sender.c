#include "pgm.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

// Basic structure for Sender
int main_sender(int argc, char** argv) {

  // argv: img_sender <fifo_path> <input.pgm>
  // Sender only sends image; worker does the filtering through its CLI

  // parse_args_or_exit(); //TODO:
  
  const char* fifo = argv[1];
  const char* inpath = argv[2];
  
  // 1) Ensure FIFO exists (mkfifo if necessary)
  
  // 2) Read PGM image (P5) from disk  
  
  // 3) Prepare header (mode/t1/t2 are ignoned by the worker;
  // send only image metadata) 
  
  // 4) Open FIFO for writing (blocked until worker opens for reading)
 
  // 5) Send header + pixels
 
  // 6) Close FIFO and free up memory 
 
  // 7) End
 
  return 0;
}

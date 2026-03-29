#ifndef LIBRARY_H
#define LIBRARY_H

/* ===== Constants ===== */ 
#define MODE_NEG 0
#define MODE_SLICE 1 

typedef struct {
  int w, h, maxv; // maxv = 255 
  unsigned char* data; // w*h bytes (grayscale)
} PGM; 

typedef struct {
  int w, h, maxv; // image metadata
  int mode; // 0=NEGATIVE, 1=SLICE 
  int t1, t2; // valid if mode=SLICE 
} Header;

typedef struct { 
  int row_start; 
  int row_end; 
} Task ;

/* ===== I/O of PGM ===== */ 

int read_pgm(const char* path, PGM* img); 

int write_pgm(const char* path, const PGM* img); 

/* ===== FIFO ===== */ 

const char* FIFO_PATH = "/tmp/imgpipe";

void test_print_function();

#endif

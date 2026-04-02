#ifndef PGM_H
#define PGM_H 1

#include <stdint.h>

/* ===== Constants ===== */ 
// #define MODE_NEG 0
// #define MODE_SLICE 1 

typedef enum OP_Mode{
  MODE_NEG, MODE_SLICE
} OP_Mode;

typedef struct {
  uint32_t w, h; 
  uint8_t maxv; // maxv = 255 
  uint8_t* data; // w*h bytes (grayscale)
} PGM; 

typedef struct {
  uint32_t w, h;
  uint8_t maxv; // image metadata
  OP_Mode mode ; // 0=NEGATIVE, 1=SLICE 
  int32_t t1, t2; // valid if mode=SLICE 
} Header;

typedef struct { 
  uint32_t row_start; 
  uint32_t row_end; 
} Task ;

/* ===== I/O of PGM ===== */ 

int read_pgm(const char* path, PGM* img); 

int write_pgm(const char* path, const PGM* img); 

PGM pgm_negative(PGM img);

PGM pgm_slice(PGM img, uint8_t x, uint8_t y);

/* ===== FIFO ===== */ 

#define FIFO_PATH = "/tmp/imgpipe";

#endif

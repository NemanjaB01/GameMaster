#include "stdio.h"
// #include "memory.h"
#include "stdlib.h"
#include "assert.h"

#define ARRAY_SIZE0 5
#define ARRAY_SIZE1 20
#define ARRAY_SIZE2 40
#define ARRAY_SIZE3 100
#define ARRAY_SIZE4 500

#define SOME_CONSTANT 0x9999999
#define ANOTHER_CONSTANT 0x666

int main() {
  int* data[5];
  int i = 0;

  data[0] = (int*) malloc(ARRAY_SIZE0 * sizeof(int));
  assert(data[0] != 0);
  data[1] = (int*) malloc(ARRAY_SIZE1 * sizeof(int));
  data[2] = (int*) malloc(ARRAY_SIZE2 * sizeof(int));
  data[3] = (int*) malloc(ARRAY_SIZE3 * sizeof(int));
  data[4] = (int*) malloc(ARRAY_SIZE4 * sizeof(int));

  for (i = 0; i < ARRAY_SIZE2; ++i)
  {
    data[2][i] = SOME_CONSTANT;
  }
  for (i = 0; i < ARRAY_SIZE4; ++i)
  {
    data[4][i] = ANOTHER_CONSTANT;
  }

  free(data[0]);
  free(data[1]);
  free(data[3]);

  for (i = 0; i < ARRAY_SIZE2; ++i)
  {
    if (data[2][i] != SOME_CONSTANT)
    {
      assert(0);
    }
  }
  for (i = 0; i < ARRAY_SIZE4; ++i)
  {
    if (data[4][i] != ANOTHER_CONSTANT)
    {
      assert(0);
    }
  }
  
  data[0] = (int*)malloc(25 * sizeof(int));

  free(data[0]);
  free(data[2]);
  free(data[4]);

  return 0;
}
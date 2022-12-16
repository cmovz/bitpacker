# bitpacker

You can use this code to pack and unpack bits as needed.

Example usage:
```c
#include "bitpacker.h"
#include <stdio.h>
#include <string.h>

#define MSG "Hello World!\n"
#define MSG_SIZE (sizeof MSG)
#define CHAR_TYPE 0
#define NUMBER_TYPE 1
#define SMALL_NUMBER_TYPE 2

int main(int argc, char *argv[])
{
  int numbers[2] = {0, 42};
  char msg[MSG_SIZE];
  unsigned char packed[30 + 1]; // must always have 1 extra byte

  memcpy(msg, MSG, MSG_SIZE);

  struct bitpacker_ctx pctx;
  bitpacker_ctx_init(&pctx, packed);

  // pack the msg
  for (int i = 0; i < MSG_SIZE; ++i) {
    unsigned char type = CHAR_TYPE;
    bitpacker_update(&pctx, &type, 2);
    bitpacker_update(&pctx, msg + i, 7);
  }

  // pack the numbers
  for (int i = 0; i < 2; ++i) {
    unsigned char type = NUMBER_TYPE;
    bitpacker_update(&pctx, &type, 2);
    bitpacker_update(&pctx, numbers + i, 32);
  }

  // pack small numbers
  for (int i = 0; i < 8; ++i) {
    unsigned char type = SMALL_NUMBER_TYPE;
    bitpacker_update(&pctx, &type, 2);
    bitpacker_update(&pctx, &i, 3);
  }

  printf("packed size = %zu\n", pctx.size);

  struct bitunpacker_ctx uctx;
  bitunpacker_ctx_init(&uctx, packed);

  for (int i = 0; i < pctx.size;) {
    unsigned char type;
    bitunpacker_update(&uctx, &type, 2);
    i += 2;

    if (type == CHAR_TYPE) {
      char c;
      bitunpacker_update(&uctx, &c, 7);
      i += 7;
      putchar(c);
    }
    else if (type == NUMBER_TYPE) {
      int n;
      bitunpacker_update(&uctx, &n, 32);
      i += 32;
      printf("number = %d\n", n);
    }
    else if (type == SMALL_NUMBER_TYPE) {
      unsigned char n;
      bitunpacker_update(&uctx, &n, 3);
      i += 3;
      printf("small number = %u\n", n);
    }
  }

  return 0;
}
```
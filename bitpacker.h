/*
  MIT License

  Copyright (c) 2022 Douglas Maieski

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
#ifndef BITPACKER_H
#define BITPACKER_H

#include <stddef.h>

struct bitpacker_ctx {
  unsigned char *ptr;
  size_t size;
  int byte_capacity;
};

struct bitunpacker_ctx {
  const unsigned char *ptr;
  int byte_pos;
};

// init ctx to pack bits
void bitpacker_ctx_init(struct bitpacker_ctx *ctx, void *dest);

// add bit_len bits to dest in ctx
void bitpacker_update(
  struct bitpacker_ctx *ctx,
  const void *bits,
  size_t bit_len
);

// init ctx to unpack bits
void bitunpacker_ctx_init(
  struct bitunpacker_ctx *ctx,
  const void *src
);

// unpack bit_len bits from ctx to dest
void bitunpacker_update(
  struct bitunpacker_ctx *ctx,
  void *bits,
  size_t bit_len
);

#endif
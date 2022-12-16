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
#include "bitpacker.h"

static const unsigned int mask_table[9] = {
  0x0, 0x1, 0x3, 0x7, 0xf, 0x1f, 0x3f, 0x7f, 0xff
};

static const unsigned int shift_table[9] = {
  8, 7, 6, 5, 4, 3, 2, 1, 0
};

void bitpacker_ctx_init(struct bitpacker_ctx *ctx, void *dest)
{
  ctx->ptr = dest;
  ctx->size = 0;
  ctx->byte_capacity = 8;
  *ctx->ptr = 0;
}

void bitpacker_update(
  struct bitpacker_ctx *ctx,
  const void *bits,
  size_t bit_len
)
{
  ctx->size += bit_len;

  const unsigned char *src = bits;
  int src_available = bit_len > 8 ? 8 : bit_len;
  int src_pos = 0;

  while (bit_len) {
    int n = ctx->byte_capacity < src_available 
      ? ctx->byte_capacity : src_available;
    
    *ctx->ptr |= (mask_table[n] & (*src >> src_pos)) 
      << shift_table[ctx->byte_capacity];
    
    bit_len -= n;

    src_available -= n;
    if (src_available == 0) {
      src_available = bit_len > 8 ? 8 : bit_len;
      src_pos = 0;
      ++src;
    }
    else
      src_pos += n;
    
    ctx->byte_capacity -= n;
    if (ctx->byte_capacity == 0) {
      ctx->byte_capacity = 8;
      ++ctx->ptr;
      *ctx->ptr = 0;
    }
  }
}

void bitunpacker_ctx_init(
  struct bitunpacker_ctx *ctx,
  const void *src
)
{
  ctx->ptr = src;
  ctx->byte_pos = 0;
}

void bitunpacker_update(
  struct bitunpacker_ctx *ctx,
  void *bits,
  size_t bit_len
)
{
  unsigned char *dest = bits;
  int src_available = 8 - ctx->byte_pos;
  int dest_capacity = 8;

  if (bit_len > 0)
    *dest = 0;

  while (bit_len) {
    int n = src_available < bit_len ? src_available : bit_len;
    if (dest_capacity < n)
      n = dest_capacity;

   *dest |= (mask_table[n] & (*ctx->ptr >> ctx->byte_pos))
      << shift_table[dest_capacity];
    
    bit_len -= n;

    src_available -= n;
    if (src_available == 0) {
      src_available = 8;
      ctx->byte_pos = 0;
      ++ctx->ptr;
    }
    else
      ctx->byte_pos += n;
    
    dest_capacity -= n;
    if (dest_capacity == 0 && bit_len > 0) {
      dest_capacity = 8;
      ++dest;
      *dest = 0;
    }
  }
}
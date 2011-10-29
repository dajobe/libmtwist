/* -*- Mode: c; c-basic-offset: 2 -*-
 *
 * mtwist_internal.c - Mersenne Twister internals
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 * 
 */

#ifndef MTWIST_INTERNAL_H
#define MTWIST_INTERNAL_H 1

#define MTWIST_N             624
#define MTWIST_M             397

#define MT_STATIC_SEED 5489UL

/* Mersenne Twister library state */
struct mtwist_s {
  /* MT buffer holding N 32 bit unsigned integers */
  uint32_t state[MTWIST_N];

  /* Pointer into above - next long to use */
  uint32_t* next;

  /* Number of remaining integers in state before an update is needed */
  unsigned int remaining;

  /* 1 if a seed was given */
  unsigned int seeded : 1;

  /* 1 to always return a static system seed (MT_STATIC_SEED) */
  unsigned int static_system_seed : 1;
};


#endif

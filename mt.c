/* -*- Mode: c; c-basic-offset: 2 -*-
 *
 * mt.c - Mersenne Twister functions
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


#ifdef MTWIST_CONFIG
#include <mtwist_config.h>
#endif

#include <stdio.h>

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_STDINT_H
/* To get UINT32_C */
#define __STDC_CONSTANT_MACROS 1
#include <stdint.h>
#else
/* pessimistic - use an unsigned long */
typedef unsigned long uint32_t;
#define UINT32_C(v) (v ## UL)
#endif


#include <mtwist.h>

#include <mtwist_internal.h>

/*
 * Mersenne Twister (MT19937) algorithm
 * http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
 *
 * http://en.wikipedia.org/wiki/Mersenne_twister
 *
 */

#define MTWIST_UPPER_MASK    UINT32_C(0x80000000)
#define MTWIST_LOWER_MASK    UINT32_C(0x7FFFFFFF)
#define MTWIST_FULL_MASK     UINT32_C(0xFFFFFFFF)

#define MTWIST_MATRIX_A      UINT32_C(0x9908B0DF)

#define MTWIST_MIXBITS(u, v) ( ( (u) & MTWIST_UPPER_MASK) | ( (v) & MTWIST_LOWER_MASK) )
#define MTWIST_TWIST(u, v)  ( (MTWIST_MIXBITS(u, v) >> 1) ^ ( (v) & UINT32_C(1) ? MTWIST_MATRIX_A : UINT32_C(0)) )


/**
 * mtwist_new:
 *
 * Construct a Mersenne Twister object
 *
 * Return value: new MT object or NULL on failure
 */
mtwist*
mtwist_new(void)
{
  mtwist* mt;
  
  mt = (mtwist*)calloc(1, sizeof(*mt));
  if(!mt)
    return NULL;
  
  mt->remaining = 0;
  mt->next = NULL;
  mt->seeded = 0;

  return mt;
}


/**
 * mtwist_free:
 * @mt: mt object
 *
 * Destroy a Mersenne Twister object
 */
void
mtwist_free(mtwist* mt) 
{
  if(mt)
    free(mt);
}


/**
 * mtwist_init:
 * @mt: mt object
 * @seed: seed (lower 32 bits used)
 *
 * Initialise a Mersenne Twister with an unsigned 32 bit int seed
 */
void
mtwist_init(mtwist* mt, unsigned long seed)
{
  int i;

  if(!mt)
    return;
  
  mt->state[0] = (uint32_t)(seed & MTWIST_FULL_MASK);
  for(i = 1; i < MTWIST_N; i++) {
    mt->state[i] = (UINT32_C(1812433253) * (mt->state[i - 1] ^ (mt->state[i - 1] >> 30)) + i); 
    mt->state[i] &= MTWIST_FULL_MASK;
  }

  mt->remaining = 0;
  mt->next = NULL;

  mt->seeded = 1;
}


static void
mtwist_update_state(mtwist* mt)
{
  int count;
  uint32_t *p = mt->state;

  for(count = (MTWIST_N - MTWIST_M + 1); --count; p++)
    *p = p[MTWIST_M] ^ MTWIST_TWIST(p[0], p[1]);

  for(count = MTWIST_M; --count; p++)
    *p = p[MTWIST_M - MTWIST_N] ^ MTWIST_TWIST(p[0], p[1]);

  *p = p[MTWIST_M - MTWIST_N] ^ MTWIST_TWIST(p[0], mt->state[0]);

  mt->remaining = MTWIST_N;
  mt->next = mt->state;
}


/**
 * mtwist_u32rand:
 * @mt: mt object
 *
 * Get a random unsigned 32 bit integer from the random number generator
 *
 * Return value: unsigned long with 32 valid bits
 */
unsigned long
mtwist_u32rand(mtwist* mt)
{
  uint32_t r;

  if(!mt)
    return 0UL;

  if(!mt->seeded)
    mtwist_init(mt, mtwist_seed_from_system(mt));

  if(!mt->remaining)
    mtwist_update_state(mt);
  
  r = *mt->next++;
  mt->remaining--;

  /* Tempering */
  r ^= (r >> 11);
  r ^= (r << 7) & UINT32_C(0x9D2C5680);
  r ^= (r << 15) & UINT32_C(0xEFC60000);
  r ^= (r >> 18);

  r &= MTWIST_FULL_MASK;

  return (unsigned long)r;
}


/**
 * mtwist_drand:
 * @mt: mt object
 *
 * Get a random double from the random number generator
 *
 * Return value: random double in the range 0.0 inclusive to 1.0 exclusive; [0.0, 1.0) */
double
mtwist_drand(mtwist* mt)
{
  unsigned long r;
  double d;

  if(!mt)
    return 0.0;

  r = mtwist_u32rand(mt);

  d = r / 4294967296.0; /* 2^32 */

  return d;
}

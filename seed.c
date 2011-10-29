/* -*- Mode: c; c-basic-offset: 2 -*-
 *
 * seed.c - Seeding functions
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

#ifdef HAVE_TIME_H
#include <time.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_STDINT_H
#include <stdint.h>
#else
/* pessimistic - use an unsigned long */
typedef unsigned long uint32_t;
#endif

#include <mtwist.h>

#include <mtwist_internal.h>


/**
 * mtwist_seed_from_system:
 * @mt: mt object
 *
 * Get a 32 bit unsigned integer random seed based on system sources
 *
 * Return value: seed with only lower 32 bits valid
 */
unsigned long
mtwist_seed_from_system(mtwist* mt)
{
  /* SOURCE 1: processor clock ticks since process started */
  uint32_t a = (uint32_t)clock();
  /* SOURCE 2: unix time in seconds since epoch */
  uint32_t b = (uint32_t)time(NULL);
  uint32_t c;
#ifdef HAVE_UNISTD_H
  /* SOURCE 3: process ID (on unix) */
  c = getpid();
#else
  c = 0;
#endif
  
  /* Mix seed sources using public domain code from
   * http://www.burtleburtle.net/bob/c/lookup3.c
   */

#define rot(x,k) (((x)<<(k)) | ((x)>>(32-(k))))
  
  /* inlined mix(a, b, c) macro */
  a -= c;  a ^= rot(c, 4);  c += b;
  b -= a;  b ^= rot(a, 6);  a += c;
  c -= b;  c ^= rot(b, 8);  b += a;
  a -= c;  a ^= rot(c,16);  c += b;
  b -= a;  b ^= rot(a,19);  /* a += c; */ /* CLANG: not needed because of below */
  c -= b;  c ^= rot(b, 4);  /* b += a; */ /* CLANG: last calculation not needed */

  if(mt->static_system_seed)
    c = MT_STATIC_SEED;

  return (unsigned long)c;
}

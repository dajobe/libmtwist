/* -*- Mode: c; c-basic-offset: 2 -*-
 *
 * test.c - Mersenne Twister tests
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

#include <mtwist.h>


int main(int argc, char *argv[]);


#define N_TESTS 1000

#define TEST_SEED 54321U

/* test set 1: results 0..19 */
#define TEST_SET_1_START 0
#define TEST_SET_1_END 19
#define TEST_SET_1_NRESULTS (TEST_SET_1_END - TEST_SET_1_START + 1)


#define ULONG_C(v) (v ## UL)

static unsigned long const test_seed_54321_results_1[TEST_SET_1_NRESULTS] = {
  ULONG_C(3915467345), ULONG_C(2189234826), ULONG_C(2679307290), ULONG_C( 787501152),
  ULONG_C(3400771556), ULONG_C(3473638550), ULONG_C(1845911630), ULONG_C(4027756818),
  ULONG_C(2332222920), ULONG_C( 127158527), ULONG_C(1775789767), ULONG_C(3371479562),
  ULONG_C( 367824108), ULONG_C( 703848432), ULONG_C(3339822589), ULONG_C(1863375487),
  ULONG_C(2100022882), ULONG_C(2466459787), ULONG_C( 217027622), ULONG_C( 932105407)
};
#define TEST_SET_1_RESULTS test_seed_54321_results_1

/* test set 2: results 622..629 */
#define TEST_SET_2_START 622
#define TEST_SET_2_END 629
#define TEST_SET_2_NRESULTS (TEST_SET_2_END - TEST_SET_2_START + 1)
static unsigned long const test_seed_54321_results_2[TEST_SET_2_NRESULTS] = {
  ULONG_C(2109020469), ULONG_C( 264978304), ULONG_C(3951898066), ULONG_C(3322908472),
  ULONG_C(2243665931), ULONG_C(3379990241), ULONG_C(1427746768), ULONG_C(3217532946)
};
#define TEST_SET_2_RESULTS test_seed_54321_results_2

/* test set 3: results 990..999 */
#define TEST_SET_3_START 990
#define TEST_SET_3_END 999
#define TEST_SET_3_NRESULTS (TEST_SET_3_END - TEST_SET_3_START + 1)
static unsigned long const test_seed_54321_results_3[TEST_SET_3_NRESULTS] = {
  ULONG_C(4262956485), ULONG_C(2083563531), ULONG_C(1724557607), ULONG_C(4100776152), ULONG_C(4050777500),
  ULONG_C(3146323433), ULONG_C(2882918002), ULONG_C(3891093309), ULONG_C(1534503088), ULONG_C(1821071197)
};
#define TEST_SET_3_RESULTS test_seed_54321_results_3



int
main(int argc, char *argv[])
{
  const char *program = argv[0];
  int failures = 0;
  int test;
  mtwist* mt = NULL;

  if(argc != 1)
    return 1;

  mt = mtwist_new();
  if(!mt) {
    fprintf(stderr, "%s: mtwist_new() failed\n", program);
    failures++;
    goto tidy;
  }
  
  mtwist_init(mt, TEST_SEED);

  for(test = 0; test < N_TESTS; test++) {
    int check = 0;
    unsigned long expected_v;
    unsigned long v;
    
    v = mtwist_u32rand(mt);

    if(test >= TEST_SET_1_START && test <= TEST_SET_1_END) {
      check = 1;
      expected_v = TEST_SET_1_RESULTS[test - TEST_SET_1_START];
    } else if(test >= TEST_SET_2_START && test <= TEST_SET_2_END) {
      check = 1;
      expected_v = TEST_SET_2_RESULTS[test - TEST_SET_2_START];
    } else if(test >= TEST_SET_3_START && test <= TEST_SET_3_END) {
      check = 1;
      expected_v = TEST_SET_3_RESULTS[test - TEST_SET_3_START];
    }

    if(check && v != expected_v) {
      fprintf(stderr,
              "%s: Test %3d returned value: %lu expected %lu\n",
              program, test, v, expected_v);
      failures++;
    } else {
#if defined(DEBUG) && DEBUG > 1
      if(check)
        fprintf(stderr,
                "%s: Test %3d returned expected value: %lu OK\n",
                program, test, v);
      else
        fprintf(stderr, "%s: Test %3d returned value: %lu OK\n",
                program, test, v);
#endif
    }
  }

  fprintf(stdout, "%s: Returned %d failures\n", program, failures);

  tidy:
  if(mt)
    mtwist_free(mt);

  return failures;
}

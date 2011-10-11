/* { dg-do run } */
/* { dg-prune-output ".*warning: 'sseregparm' attribute ignored.*" } */
/* { dg-require-effective-target fma } */
/* { dg-options "-O3 -mfpmath=sse -mfma" } */

/* Test that the compiler properly optimizes floating point multiply
   and add instructions into FMA3 instructions.  */

#define TYPE double

#include "l_fma_1.h"

#include "fma_run_double_results_1.h"

#include "fma-check.h"
#include "l_fma_main.h"

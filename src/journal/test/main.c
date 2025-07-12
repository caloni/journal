#include "harness.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void version(test_batch_runner *runner) {
}

int main(void) {
  int retval;
  test_batch_runner *runner = test_batch_runner_new();

  version(runner);

  test_print_summary(runner);
  retval = test_ok(runner) ? 0 : 1;
  free(runner);

  return retval;
}

#include <cstdlib>
#include "include/assembly.hpp"
#include "include/signal.hpp"
#include <cstdio>

static unsigned int gv = 1;
int stack_offset = 0;

int FORCE_NOINLINE helper(const unsigned char* m) {
  ENFORCE_NON_LEAF_FUNC_VAR(m[0]);
  MOD_STACK_DAT(m, stack_offset);
  return gv;
}

int main(int argc, char* argv[])
{
  // get the offset of RA on stack
  stack_offset = 4 * (argv[1][0] - '0');

  unsigned char m[] = FUNC_MACHINE_CODE;
  printf("dummy print: m = %p\n", m);
  begin_catch_exception(m, SEGV_ACCERR);
  begin_catch_exception(m+4, 0, 0, SIGFPE);
  begin_catch_exception(m+4, 0, 0, SIGILL);
  int rv = helper(m);
  end_catch_exception();
  end_catch_exception();
  end_catch_exception();
  exit(rv);
}

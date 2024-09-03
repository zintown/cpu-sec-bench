#include <cstdlib>
#include "include/global_var.hpp"
#include <string>
#include <iostream>

volatile arch_int_t offset;
int fake_init_val; // init in configure.json and is -2
FORCE_NOINLINE void helper(void *label) {
  COMPILER_BARRIER;
  GET_RAA_SP_OFFSET(offset);
  #ifdef TRACE_RUN
    GET_SP_BASE(ra_addr);
    WRITE_TRACE("RA address: 0x", ra_addr);
    WRITE_TRACE("RA before modified: 0x", *(long long*)ra_addr);
  #endif
  if(gvar() < 0){
    MOD_STACK_DAT(label, offset);
  }
  WRITE_TRACE("RA address: 0x", ra_addr);
  WRITE_TRACE("RA after modified: 0x", *(long long*)ra_addr);
  /* HiFive Unmatched, GCC 11.2.0
   * Make sure offset is modified as otherwise
   * the stack is not expanded similarily with
   * the -within-analysis test.
   */
  offset = rand();
}

FORCE_NOINLINE void* helper2(void* curr_return_addr, int sel) {
  if(sel == 1){
    GET_RA_ADDR(curr_return_addr);
  }
  return curr_return_addr;
}

int main(int argc, char* argv[])
{
  INIT_TRACE_FILE;
  int sel;
  // get the offset of RA on stack
  std::string cmd_offset = argv[1];
  offset = 4 * stoll(cmd_offset);
  fake_init_val = stol(std::string(argv[2]));
  sel = stol(std::string(argv[3]));

  void *ret_label = (void*)&main;
  GET_LABEL_ADDRESS(ret_label,TARGET_LABEL);
  if(offset == -1) { GOTO_SAVED_LABEL(ret_label);}   // impossible to happen
  gvar_init(fake_init_val);
  // call a function but illegally return
  ret_label = helper2(ret_label,sel);
TARGET_LABEL(argc)
  gvar_incr();
  helper(ret_label); // if successful, it will return to gvar_incr()
  COMPILER_BARRIER;

  exit(gvar());// if failed, it will return to -1
}

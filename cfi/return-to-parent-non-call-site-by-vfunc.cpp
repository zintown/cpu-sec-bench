#include <cstdlib>
#include "include/cfi.hpp"
#include <string>

int main(int argc, char* argv[]){

  INIT_TRACE_FILE;
  gvar_init(1);
  // get the cfi_offset of RA on stack
  if(argc == 2){
    std::string cmd_offset = argv[1];
    cfi_offset = 4 * stoll(cmd_offset);
  }else{
    std::string cmd_offset = argv[1];
    std::string extra_offset = argv[2];
    cfi_offset = 4*(stoll(cmd_offset) + stoll(extra_offset));
  }
  void *ret_label = (void*)&main;
  GET_LABEL_ADDRESS(ret_label,TARGET_LABEL);
  // call a function but illegally return
  Ret_From_Helper* obj = new Ret_From_Helper();
  if(cfi_offset == -4) { GOTO_SAVED_LABEL(ret_label);}   // impossible to happen

  obj->virtual_func(ret_label);
  gvar_decr();
  COMPILER_BARRIER;
  // the elligal return site
TARGET_LABEL(argc)
  gvar_decr();
  delete obj;
  exit(gvar());
}

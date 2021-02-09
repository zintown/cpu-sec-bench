#include "include/gcc_builtin.hpp"
#include "include/mss.hpp"

charBuffer *gbuffer;

int FORCE_NOINLINE helper(bool option) {
  charBuffer buffer;
  if(option)
    return check(buffer.data, 8,  1, 'c');
  else
    gbuffer = &buffer;
  return 0;
}

int main() {
  int rv0 = helper(false);
  update_by_pointer(gbuffer->data, 0, 8,  1, 'c');
  int rv1 = helper(true);
  return rv0 + rv1;
}

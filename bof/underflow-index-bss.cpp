#include "include/bof.hpp"

charBuffer buffer;

int main()
{
  char_buffer_init(&buffer);
  update_index(buffer, -9, -1);
  return check(buffer.underflow);
}


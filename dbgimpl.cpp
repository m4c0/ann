module;
#include <stdarg.h>
#include <stdio.h>

module dbg:dbgimpl;

namespace dbg {
void print(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
}
} // namespace dbg

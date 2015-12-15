

#include "DebugPrinter.h"

#include "MOOS/libMOOS/Utils/MOOSUtilityFunctions.h"

#include <stdarg.h>

DebugPrinter::DebugPrinter(void)
{
  setLevel(LVL_OFF);
}

DebugPrinter::DebugPrinter(const DebugLevel level)
{
  setLevel(level);
}

DebugPrinter::~DebugPrinter(void)
{

}

void
DebugPrinter::dprintf(const DebugLevel level, const char *format, ...)
{
  if (level <= this->level && level != LVL_OFF)
  {
    const unsigned int MAX_TRACE_STR = 2048;
    char buf[MAX_TRACE_STR * 2];

    va_list args;
    va_start(args, format);

    vsnprintf(buf, sizeof(buf), format, args);

    va_end(args);

    MOOSTrace(buf);
  }
}

void
DebugPrinter::setLevel(const DebugLevel level) throw(int)
{
  if (level > LVL_MAX_VERB)
    throw level;
  this->level = level;
}

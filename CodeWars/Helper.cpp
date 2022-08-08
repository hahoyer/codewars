#include "Helper.h"
#include "windows.h"

void OutputDebug(const std::string& text)
{
  if(IsDebuggerPresent())
    OutputDebugStringA(text.c_str());
  else
    printf("%s ", text.c_str());
}

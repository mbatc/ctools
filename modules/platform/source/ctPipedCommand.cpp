#include "ctPipedCommand.h"

ctVector<uint8_t> ctPipedCommand::Run(const ctString &command, const ctVector<ctString> &args)
{
  char buffer[1024] = { 0 };

  ctString cmdLine = command;
  for (const ctString &v : args)
    cmdLine += " \"" + v + "\"";

  ctString result = "";
  FILE* hPipe = _popen(cmdLine.c_str(), "r");
  if (!hPipe)
    return {};
  
  try
  {
    while (fgets(buffer, sizeof(buffer), hPipe) != NULL)
      result += buffer;
  }
  catch (...) {
    // Something went wrong
  }
  _pclose(hPipe);

  return ctVector<uint8_t>((uint8_t*)result.c_str(), result.length() + 1);
}

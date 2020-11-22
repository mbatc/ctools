#ifndef atPipedCommand_h__
#define atPipedCommand_h__

#include "ctString.h"

class ctPipedCommand
{
public:
  static ctVector<uint8_t> Run(const ctString &command, const ctVector<ctString> &args);
};

#endif // atPipedCommand_h__

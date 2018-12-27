#include "stream.h"
#include "utils.h"

std::string toCppArray(const STREAM & iStream)
{
  if (iStream.buffer == NULL || iStream.length == 0)
  {
    return "{}";
  }

  std::string code;
  for(size_t i=0; i<iStream.length; i++)
  {
    if (code != "")
      code.append(", ");

    const char * hexString = toHex(iStream.buffer[i], "0x%02X");
    code.append(hexString);
  }

  code.insert(0, "{");
  code.append("}");

  return code;
}

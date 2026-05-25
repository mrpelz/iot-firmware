#include "./main.h"

namespace IotNode::Utils::Link
{
  ::std::string printMacAddress(unsigned char input[6])
  {
    char result[18];

    snprintf(
        result,
        sizeof(result),
        "%02x:%02x:%02x:%02x:%02x:%02x",
        input[0],
        input[1],
        input[2],
        input[3],
        input[4],
        input[5]);

    String output(result);
    output.toUpperCase();

    return output.c_str();
  }
}

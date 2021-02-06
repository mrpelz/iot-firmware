#include "./tools.h"

String printMacAddress(uint8_t input[6]) {
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
    input[5]
  );

  String output(result);
  output.toUpperCase();

  return output;
}

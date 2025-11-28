#include <iostream>

#include "version.h"

int main(int argc, char* argv[]) {
  std::cout << APP_NAME << " v" << APP_VERSION << std::endl;
  std::cout << APP_DESCRIPTION << std::endl;
  return 0;
}
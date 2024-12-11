#include <iostream>
#include <fstream>
#include <regex>

int main(int argc, char** argv) {
  if (argc != 2) {
    return 0;
  }

  std::ifstream fileStream(argv[1]);
  if (fileStream.is_open()) {
    std::regex mulPattern(R"(mul\((\d{1,3}),(\d{1,3})\))");
    std::regex dnPattern(R"(don't\(\))");
    std::regex doPattern(R"(do\(\))");

    int sum = 0;

    int num1, num2;
    std::string buffer;
    bool canMultiply = true;
    char character;

    while(fileStream.get(character)) {
      buffer += character;

      std::smatch regexMatch;
      if (std::regex_search(buffer, regexMatch, dnPattern)) {
        canMultiply = false;
        buffer.clear();
        continue;
      }

      if (std::regex_search(buffer, regexMatch, doPattern)) {
        canMultiply = true;
        buffer.clear();
        continue;
      }

      if (!canMultiply) continue;

      if (std::regex_search(buffer, regexMatch, mulPattern)) {
        if (regexMatch.size() == 3) {
          num1 = std::stoi(regexMatch.str(1));
          num2 = std::stoi(regexMatch.str(2));
          sum += num1 * num2;
        }

        buffer.clear();
      }
    }

    std::cout << sum << std::endl;
  }

  return 0;
}

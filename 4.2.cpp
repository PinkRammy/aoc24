#include <deque>
#include <iostream>
#include <fstream>
#include <string>

int countMasLines(const std::deque<std::string>& lines) {
  int result = 0;

  size_t lineSize = lines[0].size();
  size_t diagonalSize = lineSize - 2;
  std::string diagonal1;
  std::string diagonal2;

  for (size_t i = 0; i < diagonalSize; i++) {
    diagonal1.push_back(lines[0][i]);
    diagonal1.push_back(lines[1][i + 1]);
    diagonal1.push_back(lines[2][i + 2]);

    diagonal2.push_back(lines[0][i + 2]);
    diagonal2.push_back(lines[1][i + 1]);
    diagonal2.push_back(lines[2][i]);

    if (
      (diagonal1 == "MAS" || diagonal1 == "SAM") &&
      (diagonal2 == "MAS" || diagonal2 == "SAM")
    ) {
      result += 1;
    }

    diagonal1.clear();
    diagonal2.clear();
  }

  return result;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    return 0;
  }

  std::ifstream fileStream(argv[1]);
  if (fileStream.is_open()) {
    int sum = 0;
    int line = 0;
    std::deque<std::string> buffer;

    std::string fileStreamLine;
    while(std::getline(fileStream, fileStreamLine)) {
      if (fileStreamLine.empty()) {
        continue;
      }

      // build the vertical window
      if (line < 2) {
        buffer.push_back(fileStreamLine);
        line++;
        continue;
      }

      // window built
      if (line == 2) {
        buffer.push_back(fileStreamLine);
      } else {
        buffer.pop_front();
        buffer.push_back(fileStreamLine);
      }

      // search the window
      sum += countMasLines(buffer);
      line++;
    }

    std::cout << sum << std::endl;
  }

  return 0;
}

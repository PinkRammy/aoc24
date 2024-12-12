#include <deque>
#include <iostream>
#include <fstream>

int countXmasLine(const std::string& line) {
  int result = 0;
  std::string buffer;

  for (size_t i = 0; i < line.size() - 3; i++) {
    buffer.push_back(line[i]);
    buffer.push_back(line[i + 1]);
    buffer.push_back(line[i + 2]);
    buffer.push_back(line[i + 3]);

    if (buffer == "XMAS" || buffer == "SAMX") {
      result++;
    }

    buffer.clear();
  }

  return result;
}

int countXmasLines(const std::deque<std::string>& lines) {
  int result = 0;

  size_t lineSize = lines[0].size();
  size_t diagonalSize = lineSize - 3;
  std::string buffer;

  // search vertically
  for (size_t i = 0; i < lineSize; i++) {
    buffer.push_back(lines[0][i]);
    buffer.push_back(lines[1][i]);
    buffer.push_back(lines[2][i]);
    buffer.push_back(lines[3][i]);
    if (buffer == "XMAS" || buffer == "SAMX") {
      result += 1;
    }

    buffer.clear();
  }

  // search diagonally forward
  for (size_t i = 0; i < diagonalSize; i++) {
    buffer.push_back(lines[0][i]);
    buffer.push_back(lines[1][i + 1]);
    buffer.push_back(lines[2][i + 2]);
    buffer.push_back(lines[3][i + 3]);
    if (buffer == "XMAS" || buffer == "SAMX") {
      result += 1;
    }

    buffer.clear();
  }

  // search diagonally backward
  for (size_t i = lineSize - 1; i >= 3; i--) {
    buffer.push_back(lines[0][i]);
    buffer.push_back(lines[1][i - 1]);
    buffer.push_back(lines[2][i - 2]);
    buffer.push_back(lines[3][i - 3]);
    if (buffer == "XMAS" || buffer == "SAMX") {
      result += 1;
    }

    buffer.clear();
  }

  return result;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    return 0;
  }

  std::ifstream fileStream(argv[1]);
  if (fileStream.is_open()) {
    int line = 0;
    int sum = 0;
    std::deque<std::string> buffer;

    std::string fileStreamLine;
    while(std::getline(fileStream, fileStreamLine)) {
      if (fileStreamLine.empty()) {
        continue;
      }

      // search the line
      sum += countXmasLine(fileStreamLine);

      // build the vertical window
      if (line < 3) {
        buffer.push_back(fileStreamLine);
        line++;
        continue;
      }

      // window built
      if (line == 3) {
        buffer.push_back(fileStreamLine);
      } else {
        buffer.pop_front();
        buffer.push_back(fileStreamLine);
      }
      
      // search the window
      sum += countXmasLines(buffer);
      line++;
    }

    std::cout << sum << std::endl;
  }

  return 0;
}

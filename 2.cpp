#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

bool isSafe(std::vector<int> levels) {
  int lastLevel = -1;
  int lastTrend = 0;
  int currentTrend = 0;
  int diff, absDiff = 0;

  for (auto it = levels.begin(); it != levels.end(); it++) {
    if (lastLevel < 0) {
      lastLevel = *it;
      continue;
    }

    if (lastTrend == 0) {
      diff = *it - lastLevel;
      absDiff = std::abs(diff);
      if (absDiff < 1 || absDiff > 3) {
        return false;
      }

      lastTrend = diff < 0 ? -1 : 1;
      lastLevel = *it;
      continue;
    }

    diff = *it - lastLevel;
    absDiff = std::abs(diff);
    if (absDiff < 1 || absDiff > 3) {
      return false;
    }

    currentTrend = diff < 0 ? -1 : 1;
    if (currentTrend != lastTrend) {
      return false;
    }

    lastLevel = *it;
  }

  return true;
}

int main(int argc, char** argv) {
  if (argc != 2) return 0;

  std::ifstream fileStream(argv[1]);
  if (fileStream.is_open()) {
    int num, sum = 0;

    std::string fileStreamLine;
    while (std::getline(fileStream, fileStreamLine)) {
      std::istringstream stringStream(fileStreamLine);
      std::vector<int> levels;

      while (stringStream >> num) {
        levels.push_back(num);
      }

      if (isSafe(levels)) {
        sum += 1;
      }
    }

    std::cout << sum << std::endl;
  }

  return 0;
}


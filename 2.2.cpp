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

std::string toString(std::vector<int>& levels) {
  std::string result = "";

  for (size_t i = 0; i < levels.size(); i++) {
    if (i == levels.size() - 1) {
      result += std::to_string(levels[i]);
      continue;
    }

    result += std::to_string(levels[i]);
    result += " ";
  }

  return result;
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
        std::cout << fileStreamLine << " is safe" << std::endl;
        sum += 1;
        continue;
      }

      std::cout << fileStreamLine << " is unsafe" << std::endl;

      int tolerations = 1;
      for (size_t i = 0; i < levels.size(); i++) {
        std::vector<int> maybeSafe = levels;
        maybeSafe.erase(maybeSafe.begin() + i);

        std::cout << "\tchecking " << toString(maybeSafe) << ": "; 

        if (isSafe(maybeSafe)) {
          std::cout << "safe" << std::endl;
          sum += 1;
          break;
        } else {
          std::cout << "unsafe" << std::endl;
        }
      }
    }

    std::cout << sum << std::endl;
  }

  return 0;
}


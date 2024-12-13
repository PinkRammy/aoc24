#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

std::unordered_map<int, std::vector<int>> pageRules;

bool hasCorrectOrder(const std::vector<int>& numbers) {
  int lastChecked = 0;

  for (const int& n : numbers) {
    // can't check the first one on its own
    if (lastChecked == 0) {
      lastChecked = n;
      continue;
    }

    // initialize the correct position flag
    bool correctPosition = false;

    // check the page rules
    const std::vector<int>& pages = pageRules[lastChecked];
    for(const int& p : pages) {
      if (p == n) {
        correctPosition = true;
        break;
      }
    }

    // check if the flag changed
    if (!correctPosition) {
      return false;
    }

    lastChecked = n;
  }

  return true;
}

int getMiddlePage(const std::string& pages) {
  std::string number;
  std::vector<int> numbers;

  // extract the numbers
  for (size_t i = 0; i < pages.size(); i++) {
    if (pages[i] == ',') {
      numbers.push_back(std::stoi(number));
      number.clear();
      continue;
    }

    number.push_back(pages[i]);

    if (i == pages.size() - 1) {
      numbers.push_back(std::stoi(number));
    }
  }

  // check the ordering
  if (!hasCorrectOrder(numbers)) {
    return 0;
  }

  // return middle page
  int middleIndex = numbers.size() / 2;
  return numbers[middleIndex];
}

int main(int argc, char** argv) {
  if (argc != 2) {
    return 0;
  }

  std::ifstream fileStream(argv[1]);
  if (fileStream.is_open()) {
    int sum = 0;
    bool writePageRules = true;
    int page1, page2;
    char pageDelimiter;

    std::string fileStreamLine;
    while(std::getline(fileStream, fileStreamLine)) {
      if (fileStreamLine.empty()) {
        writePageRules = false;
        continue;
      }

      if (writePageRules) {
        std::stringstream stringStream(fileStreamLine);
        stringStream >> page1 >> pageDelimiter >> page2;
        
        if (stringStream && pageDelimiter == '|') {
          pageRules[page1].push_back(page2);
        }

        continue;
      }

      sum += getMiddlePage(fileStreamLine);
    }

    std::cout << sum << std::endl;
  }

  return 0;
}

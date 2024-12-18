#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main(int argc, char **argv) {
  if (argc != 2) return 0;

  std::ifstream fileStream(argv[1]);
  if (fileStream.is_open()) {
    std::vector<int> list1;
    std::vector<int> list2;

    int id1, id2 = 0;
    std::string fileStreamLine;
    while (std::getline(fileStream, fileStreamLine)) {
      if (std::sscanf(fileStreamLine.c_str(), "%d %d", &id1, &id2) == 2) {
        list1.push_back(id1);
        list2.push_back(id2);
      }
    }

    std::sort(list1.begin(), list1.end());
    std::sort(list2.begin(), list2.end());

    assert (list1.size() == list2.size());

    int count, sum = 0;
    for (size_t i = 0; i < list1.size(); i++) {
      count = 0;

      id1 = list1[i];
      for (size_t j = 0; j < list2.size(); j++) {
        if (id1 == list2[j]) {
          count++;
        }
      }

      sum += id1 * count;
    }

    std::cout << sum << std::endl;
  }

  return 0;
}

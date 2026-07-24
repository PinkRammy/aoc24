#include <iostream>
#include <fstream>
#include <string>
#include <vector>

void debugDiskMap(const std::vector<int> &diskmap) {
  for (size_t i = 0; i < diskmap.size(); i++) {
    if (diskmap[i] < 0) {
      std::cout << ".";
    } else {
      std::cout << diskmap[i];
    }
  }
  std::cout << std::endl;
}

int64_t computeChecksum(const std::vector<int> &diskmap) {
  int64_t checksum = 0;
  for (size_t i = 0; i < diskmap.size(); i++) {
    if (diskmap[i] < 0) {
      continue;
    }
    checksum += i * diskmap[i];
  }
  return checksum;
}

int main(int argc, char **argv) {
  if (argc != 2) return 0;

  std::ifstream fileStream(argv[1]);
  if (fileStream.is_open()) {
    std::vector<int> diskmap;
    int64_t checksum = 0;

    // decode the diskmap
    std::string fileStreamLine;
    while(std::getline(fileStream, fileStreamLine)) {
      for (size_t i = 0; i < fileStreamLine.size(); i++) {
        int block = std::stoi(std::string(1, fileStreamLine[i]));
        if (i % 2 == 0) {
          for(int j = 0; j < block; j++) {
            diskmap.push_back(i / 2);
          }
        } else {
          for (int j = 0; j < block; j++) {
            diskmap.push_back(-1);
          }
        }
      }
    }

    // debug
    // debugDiskMap(diskmap);

    // defragment
    for (size_t i = 0; i < diskmap.size(); i++) {
      if (diskmap[i] < 0) {
        for (size_t j = diskmap.size() - 1; j > i; j--) {
          if (diskmap[j] >= 0) {
            diskmap[i] = diskmap[j];
            diskmap[j] = -1;
            break;
          }
        }
      }
    }

    // debug
    // debugDiskMap(diskmap);

    // compute checksum
    checksum = computeChecksum(diskmap);
    std::cout << checksum << std::endl;
  }

  return 0;
}
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <unordered_map>
#include <vector>

struct Point {
  int x;
  int y;

  Point(int x, int y): x(x), y(y) { }
};

struct Antenna {
  char frequency;
  Point coords;

  Antenna(): frequency(0), coords({0, 0}) { }
  Antenna(char freq, Point coords): frequency(freq), coords(coords) { }
  Antenna(char freq, int x, int y): frequency(freq), coords({x, y}) { } 
};

std::unordered_map<char, std::vector<Antenna>> antennasByFrequency;

int maxX = 0;
int maxY = 0;

bool isWithinBounds(int x, int y) {
  return x < 0 || y < 0 || x >= maxX || y >= maxY;
}

int distanceBetween(Point a, Point b) {
  int dx = a.x - b.x;
  int dy = a.y - b.y;
  return std::sqrt(dx * dx + dy * dy);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    return 0;
  }

  std::ifstream fileStream(argv[1]);
  if (fileStream.is_open()) {
    int sum = 0;

    // get antennas location
    std::string fileStreamLine;
    while(std::getline(fileStream, fileStreamLine)) {
      if (fileStreamLine.empty()) {
        continue;
      }

      if (maxX == 0) {
        maxX = fileStreamLine.size();
      }

      char c;
      for (size_t i = 0; i < fileStreamLine.size(); i++) {
        c = fileStreamLine[i];
        if (c == '.' || !std::isalnum(c)) {
          continue;
        }

        Point coords = { (int)i, maxY };
        Antenna a { c, coords };

        antennasByFrequency[c].push_back(a);
      }

      maxY++;
    }

    // get antinodes
    Antenna lastAntenna;
    for (const auto& item : antennasByFrequency) {
      for (const Antenna& antenna : item.second) {
        if (lastAntenna.frequency == 0) {
          lastAntenna = antenna;
          continue;
        }

        if (lastAntenna.frequency != antenna.frequency) {
          continue;
        }

        int distance = distanceBetween(antenna.coords, lastAntenna.coords);
        std::cout << "Distance between Antenna " << lastAntenna.frequency << " (x: " << lastAntenna.coords.x << ", y: " << lastAntenna.coords.y << ") and Antenna " << antenna.frequency << " (x: " << antenna.coords.x << ", y: " << antenna.coords.y << "): " << distance << std::endl;
      }
    }

    std::cout << sum << std::endl;
  }

  return 0;
}

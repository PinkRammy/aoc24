#include <iostream>
#include <fstream>
#include <cmath>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

struct Point {
  int x;
  int y;

  Point(int x, int y): x(x), y(y) { }
};
struct PointComparator {
  bool operator()(const Point& a, const Point& b) const {
    return std::tie(a.x, a.y) < std::tie(b.x, b.y);
  }
};

int maxX = 0;
int maxY = 0;

bool isOutOfBounds(int x, int y) {
  return x < 0 || y < 0 || x >= maxX || y >= maxY;
}

bool isOutOfBounds(const Point& p) {
  return isOutOfBounds(p.x, p.y);
}

struct Antenna {
  char frequency;
  Point coords;

  Antenna(): frequency(0), coords({0, 0}) { }
  Antenna(char freq, Point coords): frequency(freq), coords(coords) { }
  Antenna(char freq, int x, int y): frequency(freq), coords({x, y}) { }

  std::string toString() const {
    return std::string(1, frequency) +
      " (x: " + std::to_string(coords.x) +
      ", y: " + std::to_string(coords.y) +
      ")";
  }
};

struct AntennaPair {
  const Antenna* antenna1;
  const Antenna* antenna2;

  std::pair<const Point, const Point> getAntinodes() const {
    int dx = antenna2->coords.x - antenna1->coords.x;
    int dy = antenna2->coords.y - antenna1->coords.y;
    
    Point antinode1(antenna1->coords.x - dx, antenna1->coords.y - dy);
    Point antinode2(antenna2->coords.x + dx, antenna2->coords.y + dy);
    return {antinode1, antinode2};
  }

  std::set<Point, PointComparator> getAllAntinodes() const {
    int dx = antenna2->coords.x - antenna1->coords.x;
    int dy = antenna2->coords.y - antenna1->coords.y;
    std::set<Point, PointComparator> result;

    int a1 = 1;
    while(true) {
      Point antinode(antenna1->coords.x - dx * a1, antenna1->coords.y - dy * a1);
      if (isOutOfBounds(antinode)) {
        break;
      }

      result.insert(antinode);
      a1++;
    }

    int a2 = 1;
    while(true) {
      Point antinode(antenna2->coords.x + dx * a2, antenna2->coords.y + dy * a2);
      if (isOutOfBounds(antinode)) {
        break;
      }

      result.insert(antinode);
      a2++;
    }

    return result;
  }
};
struct AntennaPairComparator {
  bool operator()(const AntennaPair& a, const AntennaPair& b) const {
    return std::tie(
      a.antenna1->frequency,
      a.antenna1->coords.x, a.antenna1->coords.y,
      a.antenna2->coords.x, a.antenna2->coords.y) <
           std::tie(
      b.antenna1->frequency,
      b.antenna1->coords.x, b.antenna1->coords.y,
      b.antenna2->coords.x, b.antenna2->coords.y);
  }
};

std::unordered_map<char, std::vector<Antenna>> antennasByFrequency;

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

    // get antenna pairs
    std::set<AntennaPair, AntennaPairComparator> antennaPairs;
    for (const auto& item : antennasByFrequency) {
      for (size_t i = 0; i < item.second.size(); i++) {
        for (size_t j = i + 1; j < item.second.size(); j++) {
          AntennaPair pair { &item.second[i], &item.second[j] };
          antennaPairs.insert(pair);
        }
      }
    }

    // get antinodes
    std::set<Point, PointComparator> antinodes;
    for (const auto& antennaPair : antennaPairs) {
      std::set<Point, PointComparator> pairAntinodes = antennaPair.getAllAntinodes();
      antinodes.insert(pairAntinodes.begin(), pairAntinodes.end());
    }

    // get all antinodes
    std::set<Point, PointComparator> allAntinodes;
    allAntinodes.insert(antinodes.begin(), antinodes.end());
    for (const auto& item : antennasByFrequency) {
      for (const auto& antenna : item.second) {
        allAntinodes.insert(antenna.coords);
      }
    }

    sum = allAntinodes.size();
    std::cout << sum << std::endl;
  }

  return 0;
}

#include <cstdio>
#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

enum class MovingDirection { Up, Down, Left, Right };

struct Point {
  int x;
  int y;

  Point(int x = 0, int y = 0): x(x), y(y) { }

  Point operator+(const Point& other) const {
    return Point(x + other.x, y + other.y);
  }

  bool operator<(const Point& other) const {
    return x == other.x ? y < other.y : x < other.x;
  }

  bool operator==(const Point& other) const {
    return x == other.x && y == other.y;
  }
};

// https://en.cppreference.com/w/cpp/utility/hash
struct PointHash {
  std::size_t operator()(const Point& p) const noexcept {
    std::size_t xHash = std::hash<int>{}(p.x);
    std::size_t yHash = std::hash<int>{}(p.y);
    return xHash ^ (yHash << 1);
  }
};

int maxX, maxY;
bool outOfBounds;
MovingDirection movingDirection = MovingDirection::Up;

std::vector<std::vector<int>> obstacles;
std::unordered_map<Point, int, PointHash> visited;
std::set<Point> guardRoute;
std::set<Point> loopObstaclePositions;
Point guardPosition;

bool isOutOfBounds(const Point& p) {
  return p.x < 0 || p.y < 0 || p.x > maxX || p.y > maxY;
}

Point getMovingDirectionDelta() {
  Point result { 0, 0 };
  switch(movingDirection) {
    case MovingDirection::Up:
      result.y = -1;
      break;
    case MovingDirection::Down:
      result.y = 1;
      break;
    case MovingDirection::Left:
      result.x = -1;
      break;
    case MovingDirection::Right:
      result.x = 1;
      break;
  }

  return result;
}

MovingDirection turnRight() {
  switch(movingDirection) {
    case MovingDirection::Up:
      return MovingDirection::Right;
    case MovingDirection::Down:
      return MovingDirection::Left;
    case MovingDirection::Left:
      return MovingDirection::Up;
    case MovingDirection::Right:
      return MovingDirection::Down;
  }

  return MovingDirection::Up;
}

Point moveForward(const Point& p, const Point& o) {
  Point movingDirectionDelta = getMovingDirectionDelta();
  Point targetPosition = p + movingDirectionDelta;

  if (isOutOfBounds(targetPosition)) {
    outOfBounds = true;
    return targetPosition;
  }

  if (targetPosition == o || obstacles[targetPosition.y][targetPosition.x] == 1) {
    movingDirection = turnRight();
    targetPosition = moveForward(p, o);
  }

  return targetPosition;
}

std::set<Point> getGuardRoute() {
  Point position { guardPosition.x, guardPosition.y };
  Point noObstacle { -1, -1 };

  std::set<Point> result;
  while(!outOfBounds) {
    position = moveForward(position, noObstacle);
    if (outOfBounds) {
      break;
    }

    result.insert(position);
  }

  return result;
}

bool willLoop(const Point& newObstaclePosition) {
  outOfBounds = false;
  visited.clear();
  movingDirection = MovingDirection::Up;

  Point position { guardPosition.x, guardPosition.y };
  while(!outOfBounds) {
    position = moveForward(position, newObstaclePosition);
    if (outOfBounds) {
      break;
    }
    
    visited[position]++;

    if (visited[position] > 3) {
      return true;
    }
  }

  return false;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    return 0;
  }

  // build the obstacles
  std::ifstream fileStream(argv[1]);
  if (fileStream.is_open()) {
    int sum = 0;

    std::string fileStreamLine;
    while(std::getline(fileStream, fileStreamLine)) {
      if (fileStreamLine.empty()) {
        continue;
      }

      maxX = fileStreamLine.size();
      
      std::vector<int> row;
      char c;
      for(size_t i = 0; i < fileStreamLine.size(); i++) {
        c = fileStreamLine[i];

        if (c == '^') {
          guardPosition.x = i;
          guardPosition.y = maxY;
          row.push_back(0);
        }

        row.push_back(c == '#' ? 1 : 0);
      }

      obstacles.push_back(row);
      maxY++;
    }

    // adjust max values for bounds check
    maxX -= 1; maxY -= 1;

    // get the guard route
    guardRoute = getGuardRoute();

    // add the obstacles on the route
    for (const Point& p : guardRoute) {
      if (willLoop(p)) {
        loopObstaclePositions.insert(p);
      }
    }

    sum = loopObstaclePositions.size();
    std::cout << sum << std::endl;
  }

  return 0;
}

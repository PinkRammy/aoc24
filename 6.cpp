#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <vector>

enum class MovingDirection { Up, Down, Left, Right };

struct Point {
  int x;
  int y;

  Point(int x = 0, int y = 0): x(x), y(y) {}

  Point operator+(const Point& other) const {
    return Point(x + other.x, y + other.y);
  }

  bool operator<(const Point& other) const {
    return x == other.x ? y < other.y : x < other.x;
  }
};

int maxX, maxY;
bool outOfBounds = false;
MovingDirection movingDirection = MovingDirection::Up;

std::vector<std::vector<int>> obstacles;
std::set<Point> visited;
Point guardPosition;

bool isOutOfBounds(const Point& point) {
  return point.x < 0 || point.y < 0 || point.x > maxX || point.y > maxY;
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
      break;
    case MovingDirection::Down:
      return MovingDirection::Left;
      break;
    case MovingDirection::Left:
      return MovingDirection::Up;
      break;
    case MovingDirection::Right:
      return MovingDirection::Down;
      break;
  }

  return MovingDirection::Up;
}

Point moveForward() {
  // intended position
  Point movingDirectionDelta = getMovingDirectionDelta();
  Point targetPosition = guardPosition + movingDirectionDelta;

  // check if we will be out of bounds
  if (isOutOfBounds(targetPosition)) {
    outOfBounds = true;
    return targetPosition;
  }

  // check if we have obstacle
  if (obstacles[targetPosition.y][targetPosition.x] == 1) {
    // update moving direction
    movingDirection = turnRight();
    movingDirectionDelta = getMovingDirectionDelta();

    // update intended position
    targetPosition = guardPosition + movingDirectionDelta;
  }

  return targetPosition;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    return 0;
  }

  std::ifstream fileStream(argv[1]);
  if (fileStream.is_open()) {
    int sum = 0;

    // build the obstacles
    std::string fileStreamLine;
    while(std::getline(fileStream, fileStreamLine)) {
      if (fileStreamLine.empty()) {
        continue;
      }

      // set the max x
      maxX = fileStreamLine.size();

      // parse the positions of the obstacles
      std::vector<int> row;
      for (size_t i = 0; i < fileStreamLine.size(); i++) {
        // guard position (no obstacle)
        if (fileStreamLine[i] == '^') {
          guardPosition.x = (int)i;
          guardPosition.y = maxY;
          row.push_back(0);
          continue;
        }

        // check for obstacle
        row.push_back(fileStreamLine[i] == '#' ? 1 : 0);
      }

      obstacles.push_back(row);
      maxY++;
    }

    // start moving the guard
    maxX -= 1; maxY -= 1; // for out of bounds check
    visited.insert(guardPosition);
    while (!outOfBounds) {
      guardPosition = moveForward();
      if (outOfBounds) {
        break;
      }

      visited.insert(guardPosition);
    }

    // compute the size
    sum = visited.size();
    std::cout << sum << std::endl;
  }

  return 0;
}

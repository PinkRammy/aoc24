#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

long long concatOperator(const long long& op1, const long long& op2) {
  return std::stoll(std::to_string(op1) + std::to_string(op2));
}

bool canComputeTwo(const long long& result, const int& op1, const int& op2) {
  long long sum = op1 + op2;
  if (sum == result) {
    return true;
  }

  long long mul = op1 * op2;
  if (mul == result) {
    return true;
  }

  long long concat = concatOperator(op1, op2);
  return concat == result;
}

std::vector<std::vector<int>> getOperatorCombinations(const std::vector<int>& operands) {
  int operatorSpaces = operands.size() - 1;
  int totalCombinations = std::pow(3, operatorSpaces);

  std::vector<std::vector<int>> result;
  for (int i = 0; i < totalCombinations; i++) {
    std::vector<int> combinations;
    for (int space = 0; space < operatorSpaces; space++) {
      int op = i / std::pow(3, space);
      combinations.push_back(op % 3);
    }

    result.push_back(combinations);
  }

  return result;
}

bool canCompute(const long long& result, const std::vector<int>& operands) {
  if (operands.size() < 2) {
    return false;
  }

  // check for only 2 numbers
  if (operands.size() == 2) {
    return canComputeTwo(result, operands[0], operands[1]);
  }

  // compute for n > 2 numbers
  std::vector<std::vector<int>> opCombinations = getOperatorCombinations(operands);
  for (const std::vector<int>& ops : opCombinations) {
    long long opResult = operands[0];
    for (size_t i = 0; i < ops.size(); i++) {
      if (ops[i] == 0) {
        opResult += operands[i + 1];
      } else if (ops[i] == 1) {
        opResult *= operands[i + 1];
      } else {
        opResult = concatOperator(opResult, operands[i + 1]);
      }
    }

    if (opResult == result) {
      return true;
    }
  }

  return false;
}

std::string printint128(__int128 value) {
  std::string result;

  while (value > 0) {
    result.insert(result.begin(), '0' + value % 10);
    value /= 10;
  }

  return result;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    return 0;
  }

  std::ifstream fileStream(argv[1]);
  if (fileStream.is_open()) {
    __int128 sum = 0;

    std::string fileStreamLine;
    while(std::getline(fileStream, fileStreamLine)) {
      if (fileStreamLine.empty()) {
        continue;
      }

      // get the numbers for the equation
      long long result = 0;
      std::vector<int> operands;
      std::istringstream stringStream(fileStreamLine);
      std::string buffer;

      // get the result
      std::getline(stringStream, buffer, ':');
      result = std::stoll(buffer);

      // get the operands
      while (stringStream >> buffer) {
        operands.push_back(std::stoll(buffer));
      }

      // check if the equation is true
      if (canCompute(result, operands)) {
        sum += result;
      }
    }

    std::cout << printint128(sum) << std::endl;
  }

  return 0;
}

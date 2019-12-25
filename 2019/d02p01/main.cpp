#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <vector>

int main()
{
  std::ifstream stream{ "input.txt" };
  std::string line;
  std::getline(stream, line);
  std::replace(line.begin(), line.end(), ',', ' ');
  std::istringstream numbers{ line };
  std::vector<int64_t> code{ std::istream_iterator<int64_t>(numbers), {} };

  for (std::size_t i = 0; code[i] != 99;)
  {
    switch (code[i])
    {
    case 1:
    {
      int64_t idx_lhs = code[i + 1];
      int64_t idx_rhs = code[i + 2];
      int64_t idx_trg = code[i + 3];
      code[idx_trg] = code[idx_lhs] + code[idx_rhs];
      i += 4;
      break;
    }
    case 2:
    {
      int64_t idx_lhs = code[i + 1];
      int64_t idx_rhs = code[i + 2];
      int64_t idx_trg = code[i + 3];
      code[idx_trg] = code[idx_lhs] * code[idx_rhs];
      i += 4;
      break;
    }
    default:
      std::terminate();
      break;
    }
  }

  std::cout << code[0] << '\n';
}
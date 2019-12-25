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

  for (code[1] = 0; code[1] < 100; ++code[1])
  {
    for (code[2] = 0; code[2] < 100; ++code[2])
    {
      std::vector<int64_t> code2 = code;
      for (std::size_t i = 0; code[i] != 99;)
      {
        switch (code2[i])
        {
        case 1:
        {
          int64_t idx_lhs = code2[i + 1];
          int64_t idx_rhs = code2[i + 2];
          int64_t idx_trg = code2[i + 3];
          code2[idx_trg] = code2[idx_lhs] + code2[idx_rhs];
          i += 4;
          break;
        }
        case 2:
        {
          int64_t idx_lhs = code2[i + 1];
          int64_t idx_rhs = code2[i + 2];
          int64_t idx_trg = code2[i + 3];
          code2[idx_trg] = code2[idx_lhs] * code2[idx_rhs];
          i += 4;
          break;
        }
        default:
          std::terminate();
          break;
        }
      }

      if (code2[0] == 19690720)
      {
        std::cout << 100 * code[1] + code[2] << '\n';
        break;
      }
    }
  }
}
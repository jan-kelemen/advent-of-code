#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <vector>

using ram = std::vector<int64_t>;

struct registers
{
  int64_t program_counter = 0;
};

enum class address_mode
{
  position = 0,
  immediate = 1,
};

enum class instruction_tag
{
  sum = 1,
  multiply = 2,
  input = 3,
  output = 4,
  halt = 99,
};

int64_t const& read(address_mode const mode, int64_t const& position, ram const& ram)
{
  switch (mode)
  {
  case address_mode::position:
    return ram[position];
  case address_mode::immediate:
    return position;
  default:
    std::terminate();
    break;
  }
}

void write(address_mode const mode, int64_t const& position, int64_t const& value, ram& ram)
{
  switch (mode)
  {
  case address_mode::position:
    ram[position] = value;
    break;
  case address_mode::immediate:
  default:
    std::terminate();
    break;
  }
}

int main()
{
  std::ifstream stream{ "input.txt" };
  std::string line;
  std::getline(stream, line);
  std::replace(line.begin(), line.end(), ',', ' ');
  std::istringstream numbers{ line };

  registers registers;
  ram ram{ std::istream_iterator<int64_t>(numbers), {} };

  while (true)
  {
    address_mode const first_arg_mode = static_cast<address_mode>((ram[registers.program_counter] / 100) % 10);
    address_mode const second_arg_mode = static_cast<address_mode>((ram[registers.program_counter] / 1000) % 10);
    address_mode const third_arg_mode = static_cast<address_mode>((ram[registers.program_counter] / 10000) % 10);
    instruction_tag const opcode = static_cast<instruction_tag>(ram[registers.program_counter] % 100);
    if (opcode == instruction_tag::halt) { break; }

    switch (opcode)
    {
    case instruction_tag::sum:
    {
      int64_t const& lhs = read(first_arg_mode, ram[registers.program_counter + 1], ram);
      int64_t const& rhs = read(second_arg_mode, ram[registers.program_counter + 2], ram);
      write(third_arg_mode, ram[registers.program_counter + 3], lhs + rhs, ram);
      registers.program_counter += 4;
      break;
    }
    case instruction_tag::multiply:
    {
      int64_t const& lhs = read(first_arg_mode, ram[registers.program_counter + 1], ram);
      int64_t const& rhs = read(second_arg_mode, ram[registers.program_counter + 2], ram);
      write(third_arg_mode, ram[registers.program_counter + 3], lhs * rhs, ram);
      registers.program_counter += 4;
      break;
    }
    case instruction_tag::input:
    {
      int64_t value;
      std::cin >> value;
      write(first_arg_mode, ram[registers.program_counter + 1], value, ram);
      registers.program_counter += 2;
      break;
    }
    case instruction_tag::output:
    {
      std::cout << read(first_arg_mode, ram[registers.program_counter + 1], ram) << '\n';
      registers.program_counter += 2;
      break;
    }
    case instruction_tag::halt:
    default:
      std::terminate();
      break;
    }
  }
}
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <functional>
#include <set>
#include <vector>
#include <charconv>

enum class direction
{
  left, right, down, up
};

struct path_step
{
  direction direction;
  std::size_t length;
};

std::istream& operator>>(std::istream& stream, path_step& step)
{
  std::string str;
  if (stream >> str && !str.empty())
  {
    switch (str[0])
    {
    case 'L': step.direction = direction::left; break;
    case 'R': step.direction = direction::right; break;
    case 'U': step.direction = direction::up; break;
    case 'D': step.direction = direction::down; break;
    default: std::terminate();
    }

    std::from_chars(str.c_str() + 1, str.c_str() + str.size(), step.length);
  }

  return stream;
}

std::vector<path_step> read_wire(std::istream& stream)
{
  std::string line;
  std::getline(stream, line);
  std::replace(line.begin(), line.end(), ',', ' ');
  std::istringstream clean{ line };
  return { std::istream_iterator<path_step>(clean), {} };
}

template<typename ForwardIterator>
std::set<std::pair<int64_t, int64_t>> wire_to_trace(ForwardIterator begin, ForwardIterator end)
{
  std::set<std::pair<int64_t, int64_t>> rv;
  std::pair<int64_t, int64_t> current = { 0, 0 };
  for (ForwardIterator it = begin; it != end; ++it)
  {
    path_step const& step = *it;

    std::function<void(void)> op;
    switch (step.direction)
    {
    case direction::left: op = [&current]() { --current.first; }; break;
    case direction::right: op = [&current]() { ++current.first; }; break;
    case direction::down: op = [&current]() { --current.second; }; break;
    case direction::up: op = [&current]() { ++current.second; }; break;
    default:
      std::terminate();
      break;
    }

    for (std::size_t i = 0; i < step.length; ++i)
    {
      op();
      rv.insert(current);
    }
  }
  
  return rv;
}

int64_t manhattan_distance(std::pair<int64_t, int64_t> const& v)
{
  return std::abs(v.first) + std::abs(v.second);
}

int main()
{
  std::ifstream stream{ "input.txt" };
  std::vector<path_step> wire1 = read_wire(stream);
  std::vector<path_step> wire2 = read_wire(stream);
  std::set<std::pair<int64_t, int64_t>> trace1 = wire_to_trace(wire1.cbegin(), wire1.cend());
  std::set<std::pair<int64_t, int64_t>> trace2 = wire_to_trace(wire2.cbegin(), wire2.cend());
  std::vector<std::pair<int64_t, int64_t>> intersection;
  std::set_intersection(
    trace1.cbegin(), trace1.cend(), 
    trace2.cbegin(), trace2.cend(), 
    std::back_inserter(intersection));
  auto it = std::min_element(
    intersection.cbegin(), intersection.cend(),
    [](std::pair<int64_t, int64_t> const& rhs, std::pair<int64_t, int64_t> const& lhs) 
    { 
      return manhattan_distance(rhs) < manhattan_distance(lhs); 
    });
  std::cout << manhattan_distance(*it) << '\n';
}
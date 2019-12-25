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
#include <optional>

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

template<typename ForwardIterator, typename Inserter>
std::set<std::pair<int64_t, int64_t>> wire_to_trace(ForwardIterator begin, ForwardIterator end, Inserter full_trace_inserter)
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
      *full_trace_inserter = current;
      ++full_trace_inserter;
    }
  }

  return rv;
}

int main()
{
  std::ifstream stream{ "input.txt" };
  std::vector<path_step> wire1 = read_wire(stream);
  std::vector<path_step> wire2 = read_wire(stream);
  std::vector<std::pair<int64_t, int64_t>> full_trace1;
  std::set<std::pair<int64_t, int64_t>> trace1 = wire_to_trace(wire1.cbegin(), wire1.cend(), std::back_inserter(full_trace1));
  std::vector<std::pair<int64_t, int64_t>> full_trace2;
  std::set<std::pair<int64_t, int64_t>> trace2 = wire_to_trace(wire2.cbegin(), wire2.cend(), std::back_inserter(full_trace2));

  std::vector<std::pair<int64_t, int64_t>> intersection;
  std::set_intersection(
    trace1.cbegin(), trace1.cend(),
    trace2.cbegin(), trace2.cend(),
    std::back_inserter(intersection));

  std::vector<std::size_t> steps;
  std::transform(
    intersection.cbegin(), intersection.cend(), 
    std::back_inserter(steps), 
    [w1 = std::cref(full_trace1), w2 = std::cref(full_trace2)](std::pair<int64_t, int64_t> const& v)
  {
    auto const steps_to = [](std::vector<std::pair<int64_t, int64_t>> const& v, std::pair<int64_t, int64_t> const& t)
    {
      return std::distance(v.cbegin(), std::find(v.cbegin(), v.cend(), t));
    };

    return steps_to(w1, v) + steps_to(w2, v) + 2;
  });

  std::cout << *std::min_element(steps.cbegin(), steps.cend()) << '\n';
}
#include <fstream>
#include <iostream>
#include <numeric>
#include <optional>
#include <regex>
#include <string>
#include <variant>
#include <vector>

struct file
{
    std::string name;
    std::size_t size;
};

struct directory
{
    directory* parent {nullptr};
    std::string name;
    std::vector<std::variant<file, std::unique_ptr<directory>>> content;
};

directory* find_directory(directory& root, std::string_view name)
{
    if (name == "..")
    {
        return root.parent;
    }

    auto it = std::find_if(std::begin(root.content),
        std::end(root.content),
        [&name](auto&& node)
        {
            return std::holds_alternative<std::unique_ptr<directory>>(node) &&
                std::get<std::unique_ptr<directory>>(node)->name == name;
        });

    return std::get<std::unique_ptr<directory>>(*it).get();
}

size_t calculate_size(directory const& tree,
    std::vector<size_t>& less_than_100k)
{
    size_t total_size {0};
    for (auto const& child : tree.content)
    {
        if (std::holds_alternative<file>(child))
        {
            total_size += std::get<file>(child).size;
        }
        else
        {
            total_size +=
                calculate_size(*std::get<std::unique_ptr<directory>>(child),
                    less_than_100k);
        }
    }

    if (total_size < 100000)
    {
        less_than_100k.push_back(total_size);
    }

    return total_size;
}

int main([[maybe_unused]] int argc, char** argv)
{
    std::ifstream input {argv[1]};

    std::regex const command {R"((?:cd|ls)(?: (.+))?)"};
    std::regex const ls_output {R"((?:dir|(\d+)) (.+))"};

    std::optional<directory> tree {std::nullopt};
    directory* working_directory {nullptr};
    std::string line;
    while (line[0] == '$' || std::getline(input, line))
    {
        std::smatch m;
        if (std::regex_match(std::cbegin(line) + 2,
                std::cend(line),
                m,
                command))
        {
            line.clear();

            auto const is_cd = m[1].matched;
            if (is_cd)
            {
                auto const path_arg = m[1].str();
                if (path_arg == "/")
                {
                    tree = directory {nullptr, path_arg, {}};
                    working_directory = &(*tree);
                }
                else
                {
                    working_directory =
                        find_directory(*working_directory, path_arg);
                }
            }
            else
            {
                while (std::getline(input, line) && line[0] != '$')
                {
                    std::smatch f;
                    if (std::regex_match(line, m, ls_output))
                    {
                        auto const is_file {m[1].matched};
                        if (is_file)
                        {
                            working_directory->content.emplace_back(
                                file {m[2].str(), std::stoull(m[1].str())});
                        }
                        else
                        {
                            working_directory->content.emplace_back(
                                std::unique_ptr<directory>(
                                    new directory {working_directory,
                                        m[2].str(),
                                        {}}));
                        }
                    }
                }
            }
        }
    }

    std::vector<size_t> sizes;
    calculate_size(*tree, sizes);

    std::cout << std::accumulate(std::cbegin(sizes), std::cend(sizes), 0)
              << '\n';
}

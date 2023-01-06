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

size_t calculate_size(directory const& tree)
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
                calculate_size(*std::get<std::unique_ptr<directory>>(child));
        }
    }

    return total_size;
}

size_t find_directories_with_enough_data(directory const& tree,
    size_t const available_space,
    std::vector<size_t>& sizes)
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
            total_size += find_directories_with_enough_data(
                *std::get<std::unique_ptr<directory>>(child),
                available_space,
                sizes);
        }
    }

    if (available_space + total_size >= 30000000)
    {
        sizes.push_back(total_size);
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

    auto free_space_available = 70000000 - calculate_size(*tree);
    std::vector<size_t> sizes;
    find_directories_with_enough_data(*tree, free_space_available, sizes);

    std::cout << *std::min_element(std::cbegin(sizes), std::cend(sizes))
              << '\n';
}

#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <numeric>
#include <optional>
#include <regex>
#include <string>
#include <tuple>


enum class event
{
    begins_shift, falls_asleep, wakes_up
};

struct record_time
{
    record_time() = default;
    record_time(int year, int month, int day, int hour, int minute) : data_{ year, month, day, hour, minute } {}
    record_time(record_time const&) = default;
    record_time(record_time&&) = default;

    [[nodiscard]] constexpr int year() const { return std::get<0>(data_); }
    [[nodiscard]] constexpr int month() const { return std::get<1>(data_); }
    [[nodiscard]] constexpr int day() const { return std::get<2>(data_); }
    [[nodiscard]] constexpr int hour() const { return std::get<3>(data_); }
    [[nodiscard]] constexpr int minute() const { return std::get<4>(data_); }

    record_time& operator=(record_time const&) = default;
    record_time& operator=(record_time&&) = default;

    ~record_time() = default;
private:
    std::tuple<int, int, int, int, int> data_;

    friend bool operator==(record_time const& lhs, record_time const& rhs);
    friend bool operator<(record_time const& lhs, record_time const& rhs);
};

[[nodiscard]] bool operator==(record_time const& lhs, record_time const& rhs) { return lhs.data_ == rhs.data_; }
[[nodiscard]] bool operator!=(record_time const& lhs, record_time const& rhs) { return !(lhs == rhs); }
[[nodiscard]] bool operator<(record_time const& lhs, record_time const& rhs) { return lhs.data_ < rhs.data_; }
[[nodiscard]] bool operator>=(record_time const& lhs, record_time const& rhs) { return !(lhs < rhs); }
[[nodiscard]] bool operator>(record_time const& lhs, record_time const& rhs) { return !(rhs < lhs); }
[[nodiscard]] bool operator<=(record_time const& lhs, record_time const& rhs) { return !(lhs > rhs); }

struct record
{
    inline static std::regex reg = std::regex{ R"(\[(\d{4})-(\d{2})-(\d{2}) (\d{2}):(\d{2})\] (falls asleep|wakes up|(Guard #(\d+) begins shift)))" };

    record_time time;
    event event;
    std::optional<int> guard_id;
};

struct time_ordered_compare
{
    [[nodiscard]] bool operator()(record const& lhs, record const& rhs) const { return lhs.time < rhs.time; }
};


std::istream& operator>>(std::istream& stream, record& record)
{
    constexpr auto capture_groups = 6;
    using namespace std::string_literals;

    auto line = ""s;
    if (std::getline(stream, line))
    {
        auto match = std::smatch{};
        if (!std::regex_search(line, match, record::reg))
            throw std::domain_error{ "" };

        auto arr = std::array<int, 5>{};
        for (auto i = std::size_t{ 1 }; i < arr.size() + 1; ++i)
            arr[i - 1] = std::stol(match[i]);

        record.time = record_time{ arr[0], arr[1], arr[2], arr[3], arr[4] };
        record.event = [&, &id = record.guard_id]() mutable
        {
            id.reset();

            std::string action = match[capture_groups];
            if (action[0] == 'f') return event::falls_asleep;
            if (action[0] == 'w') return event::wakes_up;

            id = std::stol(match[capture_groups + 2]);
            return event::begins_shift;
        }();
    }

    return stream;
}

struct second_value_ordered_compare
{
    [[nodiscard]] bool operator()(std::pair<int, int> const& lhs, std::pair<int, int> const& rhs) const
    {
        return lhs.second < rhs.second;
    }
};

int main()
{
    auto records = std::vector<record>{};
    std::copy(std::istream_iterator<record>{std::cin}, {}, std::back_inserter(records));
    std::sort(records.begin(), records.end(), time_ordered_compare());

    auto guards_to_minutes = std::map<int, int>{};
    auto guards_to_minute_map = std::map<int, std::map<int, int>>{};
    auto current_id = 0;
    auto started_sleeping = 0;
    for (auto const& record : records)
    {
        if (record.event == event::begins_shift)
        {
            current_id = record.guard_id.value();
            started_sleeping = 0;
            continue;
        }

        if (record.event == event::falls_asleep)
        {
            started_sleeping = record.time.minute();
            continue;
        }

        for (auto i = started_sleeping; i != record.time.minute(); ++i)
        {
            ++guards_to_minutes[current_id];
            ++guards_to_minute_map[current_id][i];
        }
    }

    int const guards_which_sleeps_most =
        std::max_element(
            guards_to_minutes.cbegin(), guards_to_minutes.cend(), 
            second_value_ordered_compare())->first;

    auto const& minutes_of_guard_which_sleeps_most = guards_to_minute_map[guards_which_sleeps_most];

    int const most_minutes_slept =
        std::max_element(
            minutes_of_guard_which_sleeps_most.cbegin(), minutes_of_guard_which_sleeps_most.cend(),
            second_value_ordered_compare())->first;

    std::cout << guards_which_sleeps_most * most_minutes_slept << '\n';
}
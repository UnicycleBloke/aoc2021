#include "utils.h"


template <typename T>
auto part1(const T& input)
{
    auto range    = *max_element(input.begin(), input.end());
    auto min_fuel = 1'000'000'000;

    for (auto i = 0; i < range; ++i)
    {
        auto fuel = 0;
        for (auto j = 0; j < input.size(); ++j)
            fuel += abs(input[j] - i);

        min_fuel = min(min_fuel, fuel);
    }

    return min_fuel;
}


template <typename T>
auto part2(const T& input)
{
    auto range    = *max_element(input.begin(), input.end());
    auto min_fuel = 1'000'000'000;

    for (auto i = 0; i < range; ++i)
    {
        auto fuel = 0;
        for (auto j = 0; j < input.size(); ++j)
        {
            auto diff = abs(input[j] - i);
            fuel += diff * (diff + 1) / 2;
        }

        min_fuel = min(min_fuel, fuel);
    }

    return min_fuel;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename);
    auto input = aoc::make_vector<int>(lines[0]);

    auto p1 = part1(input);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 340052);

    auto p2 = part2(input);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 92948968);
}


int main(int argc, char** argv)
{
    try
    {
        if (argc < 2)
        {
            cout << "Provide input file name\n";
            return -1;
        }

        fs::path path{argv[1]};
        if (!fs::is_regular_file(path))
        {
            cout << "Path '" << argv[1] << "' does not exist or is not a file\n";
            return -1;
        }

        run(argv[1]);
    }
    catch (std::exception& e)
    {
        cout << e.what();
    }
}

#include "utils.h"


template <typename T>
auto part1(T input)
{
    aoc::timer timer;
    return 0;
}


template <typename T>
auto part2(T input)
{
    aoc::timer timer;
    return 0;
}


void run(const char* filename)
{
    aoc::timer timer;

    auto lines = aoc::read_lines(filename);
    vector<vector<uint32_t>> input;
    for (auto line: lines)
    {
        auto row = aoc::comprehend(line, [](auto c) -> uint32_t { return c - '0'; });  
        input.push_back(row);
    }

    auto p1 = part1(input);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 0);

    auto p2 = part2(input);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 0);
}


int main(int argc, char** argv)
{
    aoc::timer timer;
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

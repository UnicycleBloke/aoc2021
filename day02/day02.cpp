#include "utils.h"
#include <cassert>


using namespace std;


template <typename T>
int part1(const T& input)
{
    int x = 0;
    int y = 0;

    for (auto [dir, num]: input)
    {
        switch (dir[0])
        {
            case 'd': y += num; break;
            case 'u': y -= num; break;
            case 'f': x += num; break;
        }
    }

    aoc::check_result(x*y, 1451208);
    return x*y;
}


template <typename T>
int part2(const T& input)
{
    int x   = 0;
    int y   = 0;
    int aim = 0;

    for (auto [dir, num]: input)
    {
        switch (dir[0])
        {
            case 'd': aim += num; break;
            case 'u': aim -= num; break;
            case 'f': x += num; y += aim * num; break;
        }
    }

    aoc::check_result(x*y, 1620141160);
    return x*y;
}


int main(int argc, char** argv)
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

    // Every line of input has the same format. Parse to create a vector of tuples.
    auto input = aoc::read_lines<string, int>(argv[1], R"(([\w]+)\s(\d+))");

    cout << "Part1: " << part1(input) << '\n';
    cout << "Part2: " << part2(input) << '\n';
}

#include "utils.h"


template <typename T, typename U>
auto part1(T points, U folds)
{
    aoc::timer timer;
    return 0;
}


template <typename T, typename U>
auto part2(T points, U folds)
{
    aoc::timer timer;
    return 0;
}


void run(const char* filename)
{
    aoc::timer timer;

    auto points = aoc::read_lines<int, int>(filename, R"((\d+),(\d+))");
    auto folds  = aoc::read_lines<char, int>(filename, R"(fold\salong\s(\w)=(\d+))");

    auto p1 = part1(points, folds);
    cout << "Part1: " << p1 << '\n';
    //aoc::check_result(p1, 0);

    auto p2 = part2(points, folds);
    cout << "Part2: is read off the screen\n";
    //aoc::check_result(p2, 0);
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

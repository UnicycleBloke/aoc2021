#include "utils.h"


int part1(vector<int> calls)
{
    return 0;
}


int part2(vector<int> calls)
{
    return 0;
}


void run(const char* filename)
{
    auto input = aoc::read_lines(filename, false);

    auto temp  = aoc::split(input[0], ",", false);
    auto calls = aoc::comprehend(temp, [](auto){return true;}, [](auto s){return stoi(s);});
    for (auto c: calls) cout << c << " "; cout << "\n";

    auto p1 = part1(calls);
    cout << "Part1: " << p1 << '\n';
    //aoc::check_result(p1, 28082);

    auto p2 = part2(calls);
    cout << "Part2: " << p2 << '\n';
    //aoc::check_result(p2, 8224);
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

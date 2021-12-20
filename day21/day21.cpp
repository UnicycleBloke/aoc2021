#include "utils.h"


auto part1(const string& algo, const vector<vector<int>>& grid)
{
    aoc::timer timer;
    return 0;
}


auto part2(const string& algo, const vector<vector<int>>& grid)
{
    aoc::timer timer;
    return 0;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename, false); 

    auto p1 = part1(algo, grid);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 5218);

    auto p2 = part2(algo, grid);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 15527);
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

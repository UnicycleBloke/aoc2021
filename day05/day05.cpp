#include "utils.h"


// Much too big for the stack - lost much time on head-scratching silent failures
// to work this out. Example was working perfectly in no time - GRRR
static constexpr int SIZE = 1000;
array<array<int, SIZE>, SIZE> grid;


template <typename T>
int part1(const T& input)
{
    for (auto [x1, y1, x2, y2] : input)
    {
        if (x1 == x2)
            for (int y = min(y1, y2); y <= max(y1, y2); ++y)
               grid[x1][y] += 1;
        
        if (y1 == y2)
            for (int x = min(x1, x2); x <= max(x1, x2); ++x)
                grid[x][y1] += 1;        

    }

    return overlaps();
}


template <typename T>
int part2(const T& input)
{
    // Shared grid already populated by part1().
    for (auto [x1, y1, x2, y2] : input)
    {
        if (abs(x1 - x2) == abs(y1 - y2))
        {
            int len = abs(x1 - x2);
            for (int i = 0; i <= len; ++i)
            {
                int x = x1 + i * aoc::sgn(x2 - x1);
                int y = y1 + i * aoc::sgn(y2 - y1);
                grid[x][y] += 1;  
            }    
        }              
    }

    return overlaps();
}


int overlaps()
{
    int overlaps{};
    for (int y = 0; y < SIZE; ++y)
        for (int x = 0; x < SIZE; ++x)
            overlaps += (grid[x][y] > 1);

    return overlaps;
}


void run(const char* filename)
{
    auto input = aoc::read_lines<int, int, int, int>(filename, R"((\d+),(\d+)\s->\s(\d+),(\d+))");

    auto p1 = part1(input);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 6687);

    auto p2 = part2(input);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 19851);
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

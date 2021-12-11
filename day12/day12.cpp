#include "utils.h"


template <typename T>
void print(const T& grid)
{
    for (auto i: aoc::range(1, 11, 1))
    {
        for (auto j: aoc::range(1, 11, 1))
        {
            cout << char('0' + grid[i][j]);
        }
        cout << '\n';
    }
    cout << '\n';
}


template <typename T>
int step(T& grid)
{
    int flashes = 0;

    // Increment all octopoodles
    for (auto i: aoc::range(1, 11, 1))
        for (auto j: aoc::range(1, 11, 1))
            ++grid[i][j];

    // The tricksy bit
    // Copy so flashes don't interfere with each other. Much time 
    // lost to this one...
    T grid2 = grid;
    while (true)
    {
        int flashes2 = 0;
        for (auto i: aoc::range(1, 11, 1))
        {
            for (auto j: aoc::range(1, 11, 1))
            {
                if ((grid[i][j] > 9) && (grid[i][j] < 100))
                {
                    ++flashes2;

                    ++grid2[i+1][j+1];
                    ++grid2[i+1][j];
                    ++grid2[i+1][j-1];

                    ++grid2[i][j+1];
                      grid2[i][j] = 100; // Mark as already flashed
                    ++grid2[i][j-1];

                    ++grid2[i-1][j+1];
                    ++grid2[i-1][j];
                    ++grid2[i-1][j-1];
                }
            }

            grid = grid2;
        }

        flashes += flashes2;
        if (flashes2 == 0) break;
    }

    // Restore flashed octopoddlings  
    for (auto i: aoc::range(1, 11, 1))
        for (auto j: aoc::range(1, 11, 1))
            if (grid[i][j] > 9) grid[i][j] = 0;

    return flashes;
}


template <typename T>
auto part1(T grid)
{
    int flashes = 0;
    for (auto s: aoc::range(0, 100, 1))
    {
        int count = step(grid);
        //cout << s << ' '<< count << '\n';
        //print(grid);
        flashes += count;
    }
    return flashes;
}


template <typename T>
auto part2(T grid)
{
    int s = 1;
    while (true)
    {
        int count = step(grid);
        //cout << s << ' '<< count << '\n';
        //print(grid);
        if (count == 100) return s;
        ++s;
    }
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename);

    array<array<int, 12>, 12> input{};
    for (auto i: aoc::range(1, 11, 1))
        for (auto j: aoc::range(1, 11, 1))
            input[i][j] = lines[i-1][j-1] - '0';

    auto p1 = part1(input);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 1681);

    auto p2 = part2(input);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 276);
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

#include "utils.h"


template <typename T>
auto step(const T& grid, int& moves)
{ 
    const int ROWS = grid.size();
    const int COLS = grid[0].size();

    moves = 0;

    auto grid2 = grid; 
    for (auto r: aoc::range(ROWS))
    {
       for (auto c: aoc::range(COLS))
       {
           if ((grid[r][c] == '>') && (grid[r][(c+1)%COLS] == '.'))
           {
               grid2[r][c] = '.';
               grid2[r][(c+1)%COLS] = '>';
               ++moves;
           }
       }
    }

    auto grid3 = grid2; 
    for (auto r: aoc::range(ROWS))
    {
       for (auto c: aoc::range(COLS))
       {
           if ((grid2[r][c] == 'v') && (grid2[(r+1)%ROWS][c] == '.'))
           {
               grid3[r][c] = '.';
               grid3[(r+1)%ROWS][c] = 'v';
               ++moves;
           }
       }
    }

    return grid3;
}


template <typename T>
auto print(const T& grid)
{ 
    for (auto r: grid)
    {
        for (auto c: r)      
           cout << c;
        cout << '\n';
    }
    cout << '\n';
}


template <typename T>
auto part1(T grid)
{ 
    aoc::timer timer;

    int steps = 0;
    //print(grid);
    while (true)
    {
        int moves = 0; 
        grid = step(grid, moves);

        ++steps;
        //cout << "Step " << steps << '\n';
        //print(grid);

        if (moves == 0) 
            break;
    }

    return steps;
}


void run(const char* filename)
{
    auto input = aoc::read_lines(filename);

    auto p1 = part1(input);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 568);
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

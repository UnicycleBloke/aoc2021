#include "utils.h"
//#include "cpp_curses.h"


int value(const vector<vector<int>>& grid, int r, int c)
{
    int result = 0;

    auto rows = grid.size();
    auto cols = grid[0].size();

    // This wraps the indices to make the grid effectively infinite in extent.
    // Or, if not infinite, repeats or maps to a torus.
    int rp = (r + 1) % rows;
    int rm = (r - 1 + rows) % rows;
    int cp = (c + 1) % cols;
    int cm = (c - 1 + cols) % cols;

    result = (result << 1) | (grid[rm][cm] == 1);
    result = (result << 1) | (grid[rm][c]  == 1);
    result = (result << 1) | (grid[rm][cp] == 1);

    result = (result << 1) | (grid[r][cm] == 1);
    result = (result << 1) | (grid[r][c]  == 1);
    result = (result << 1) | (grid[r][cp] == 1);

    result = (result << 1) | (grid[rp][cm] == 1);
    result = (result << 1) | (grid[rp][c]  == 1);
    result = (result << 1) | (grid[rp][cp] == 1);

    return result;
}


void print(const vector<vector<int>>& grid)
{
    for (auto r: aoc::range(1U, grid.size()-1))
    {
        for (auto c: aoc::range(1U, grid[0].size()-1))
        {
            cout << ((grid[r][c] == 0) ? '.' : '#');
        }
        cout << '\n';
    }
    cout << '\n';
}


auto nsteps(string algo, vector<vector<int>> grid, int n)
{
    auto rows = grid.size();
    auto cols = grid[0].size();

    //print(grid);

    auto grid2 = grid;
    for (auto k: aoc::range(n))
    {
        for (auto r: aoc::range(rows))
        {
            for (auto c: aoc::range(cols))
            {
                int index = value(grid, r, c);
                grid2[r][c] = (algo[index] == '#'); 
            }
        }
        grid = grid2;
        //print(grid);
    }

    int count = 0;
    for (auto r: aoc::range(grid.size()))
    {
        for (auto c: aoc::range(grid[0].size()))
        {
            count += grid[r][c];
        }
    }
    return count;
}


auto part1(const string& algo, const vector<vector<int>>& grid)
{
    aoc::timer timer;
    return nsteps(algo, grid, 2);
}


auto part2(const string& algo, const vector<vector<int>>& grid)
{
    aoc::timer timer;
    return nsteps(algo, grid, 50);
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename, false); 

    auto algo = lines[0];
    int  ncols = lines[1].size();
    int  nrows  = lines.size() - 1;

    // Plenty of room around the edges for growth.
    constexpr int EXTRA = 50;
    vector<int> row(ncols + EXTRA*2, 0);
    vector<vector<int>> grid(nrows + EXTRA*2, row);

    for (auto r: aoc::range(nrows))
    {
        auto line = lines[r + 1];
        for (auto c: aoc::range(ncols))
        {
            if (line[c] == '#')
                grid[r + EXTRA][c + EXTRA] = 1;
        }
    }

    auto p1 = part1(algo, grid);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 5044);

    auto p2 = part2(algo, grid);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 18074);
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

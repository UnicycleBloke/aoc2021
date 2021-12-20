#include "utils.h"


int value(const vector<vector<int>>& grid, int r, int c)
{
    int result = 0;

    result = (result << 1) | (grid[r-1][c-1] == 1);
    result = (result << 1) | (grid[r-1][c]   == 1);
    result = (result << 1) | (grid[r-1][c+1] == 1);

    result = (result << 1) | (grid[r][c-1] == 1);
    result = (result << 1) | (grid[r][c]   == 1);
    result = (result << 1) | (grid[r][c+1] == 1);

    result = (result << 1) | (grid[r+1][c-1] == 1);
    result = (result << 1) | (grid[r+1][c]   == 1);
    result = (result << 1) | (grid[r+1][c+1] == 1);

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
    //print(grid);

    //for (auto k: aoc::range(2))
    for (auto k: aoc::range(n))
    {
        // This tripped me up - the example had '.' in the first location, so
        // the infinite grid was all '.'. In my case it toggles on/off.
        auto grid2 = grid;
        int i = value(grid, 1, 1);
        for (auto r: aoc::range(grid.size()))
            for (auto c: aoc::range(grid[0].size()))
                grid2[r][c] = (algo[i] == '#'); 

        for (auto r: aoc::range(3U, grid.size()-3))
        {
            for (auto c: aoc::range(3U, grid[0].size()-3))
            {
                int index = value(grid, r, c);
                grid2[r][c] = (algo[index] == '#'); 
            }
        }
        grid = grid2;
        //print(grid);
    }

    int count = 0;
    for (auto r: aoc::range(1U, grid.size()-1))
    {
        for (auto c: aoc::range(1U, grid[0].size()-1))
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
    // Or could use a map of (x,y) -> on/off.
    vector<int> row(ncols + 400, 0);
    vector<vector<int>> grid(nrows + 400, row);

    for (auto r: aoc::range(nrows))
    {
        auto line = lines[r + 1];
        for (auto c: aoc::range(ncols))
        {
            if (line[c] == '#')
                grid[r + 200][c + 200] = 1;
        }
    }

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

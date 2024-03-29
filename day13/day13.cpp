#include "utils.h"


template <typename T>
auto render(T points)
{
    uint64_t xmax = 0;
    uint64_t ymax = 0;
    for (auto& [x, y]: points)
    {
        xmax = max(x, xmax);
        ymax = max(y, ymax);
    }
    ++xmax;
    ++ymax;

    cout << "xdim=" << xmax << " ydim=" << ymax << '\n';

    vector<int> row(xmax, 0);
    vector<vector<int>> grid(ymax, row);
    for (auto& [x, y]: points)
        grid[y][x] = 1;

    for (const auto& r: grid)
    {
        for (const auto& c: r)
            cout << ((c == 1) ? '#' : ' '); // More readable               
            //cout << ((c == 1) ? '#' : '.'); // Better when rendering all stages               
        cout << '\n';
    }                
    cout << '\n';
}


template <typename T, typename U>
auto fold(T& points, U& folds, bool once)
{ 
    for (auto [dir, off]: folds)
    {
        for (auto& [x, y]: points)
        {                
            // Got my x and y confused somewhere in the input. No matter.
            if (dir == 'x')
                if (x > off) x = off - (x - off);       
            if (dir == 'y')
                if (y > off) y = off - (y - off);       
        }

        if (once) 
            break;

        //render(points);
    }

    if (!once) 
        render(points);

    map<typename T::value_type, int> visible;
    for (auto& [x, y]: points) 
        visible[{x, y}] = 1;

    return visible.size();
}


template <typename T, typename U>
auto part1(T points, U folds)
{
    aoc::timer timer;
    return fold(points, folds, true);
}


template <typename T, typename U>
auto part2(T points, U folds)
{
    aoc::timer timer;
    return fold(points, folds, false);
}


void run(const char* filename)
{
    aoc::timer timer;

    auto points = aoc::read_lines<uint64_t, uint64_t>(filename, R"((\d+),(\d+))");
    auto folds  = aoc::read_lines<char, uint64_t>(filename, R"(fold\salong\s(\w)=(\d+))");

    auto p1 = part1(points, folds);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 763U);

    auto p2 = part2(points, folds);
    cout << "Part2: is read off the screen\n";
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

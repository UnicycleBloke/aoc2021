#include "utils.h"
//#include "cpp_curses.h"


// void render(const vector<vector<int>>& grid)
// {
//     static int frame = 0;
    
//     clear();

//     for (auto r: aoc::range(1U, grid.size()-1))
//     {
//         for (auto c: aoc::range(1U, grid[0].size()-1))
//         {
//             aoc::write((grid[r][c] == 0) ? '.' : '#', c, r);
//         }
//     }

//     ostringstream os;
//     os << "Frame: " << frame++ << " ";
//     aoc::write(os.str(), 2, 2);

//     refresh();
//     getch();
// }


template <typename T>
auto part1(T input)
{
    aoc::timer timer;
    return 0;
}


template <typename T>
auto part2(T input)
{
    aoc::timer timer;
    return 0;
}


void run(const char* filename)
{
    auto input = aoc::read_lines(filename);
    //auto input = aoc::read_lines<int, int, int, int>(filename, R"((\d+),(\d+)\s->\s(\d+),(\d+))");

    auto p1 = part1(input);
    cout << "Part1: " << p1 << '\n';
    //aoc::check_result(p1, 0);

    auto p2 = part2(input);
    cout << "Part2: " << p2 << '\n';
    //aoc::check_result(p2, 0);
}


int main(int argc, char** argv)
{
    //aoc::CursesApp app;

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

#include "utils.h"
#include "/usr/include/ncurses.h"


template <typename T>
auto part1(const T& input)
{
    int total_risk = 0;
    for (auto i: aoc::range{1U, input.size()-1U})
    {
        for (auto j: aoc::range{1U, input[i].size()-1U})
        {
            int  height = input[i][j];
            bool low = true;
            low = low && (height < input[i+1][j]);
            low = low && (height < input[i-1][j]);
            low = low && (height < input[i][j+1]);
            low = low && (height < input[i][j-1]);

            if (low)
            {
                total_risk += height + 1;
            }
        }
    }

    return total_risk;
}


template <typename T>
auto render(T& input)
{
    // for (auto i: aoc::range{1U, input.size()-1U})
    // {
    //     for (auto j: aoc::range{1U, input[i].size()-1U})
    //     {
    //         int height = input[i][j];
    //         auto colour = (height % 2 == 0) ? 1 : 2;
    //         colour = (height == 9) ? 3 : colour;
    //         attron(COLOR_PAIR(colour));
    //         mvaddch(i, j, '0' + height);
    //         attroff(COLOR_PAIR(colour));
    //     }
    // }
    // refresh();
    //getch();
}


// Recursively explore the basin
template <typename T, typename U>
int basin_size_recurse(T& input, U i, U j)
{
    if (input[i][j] < 9)
    {
        // Mark as already counted - higher than all neighbours.
        input[i][j] = 10;
        render(input);

        int size = 1;
        size += basin_size_recurse(input, i+1, j);
        size += basin_size_recurse(input, i-1, j);
        size += basin_size_recurse(input, i, j+1);
        size += basin_size_recurse(input, i, j-1);
        return size;
    }
    return 0;
}


template <typename T, typename U>
int basin_size_iterate(T& input, U i, U j)
{
    int size = 0;

    struct Pos { U x; U y; };
    queue<Pos> valid;

    input[i][j] = 10;
    valid.push({i, j});

    while (valid.size() > 0)
    {
        Pos p = valid.front();
        valid.pop();
        ++size;

        auto check_valid = [&](Pos p)
        {
            if (input[p.x][p.y] < 9)
            {
                // Mark as already counted - higher than all neighbours.
                input[p.x][p.y] = 10;
                valid.push({p.x, p.y});
            }
        };

        check_valid({p.x+1, p.y});
        check_valid({p.x-1, p.y});
        check_valid({p.x, p.y+1});
        check_valid({p.x, p.y-1});

        render(input);
    }

    return size;
}


template <typename T>
auto part2(T& input)
{
    vector<int> sizes;
    for (auto i: aoc::range{1U, input.size()-1U})
    {
        for (auto j: aoc::range{1U, input[i].size()-1U})
        {
            int height = input[i][j];

            bool low = true;
            low = low && (height < input[i+1][j]);
            low = low && (height < input[i-1][j]);
            low = low && (height < input[i][j+1]);
            low = low && (height < input[i][j-1]);

            if (low)
            {
                int size = basin_size_recurse(input, i, j);
                //int size  = basin_size_iterate(input, i, j);
                sizes.push_back(size);

                render(input);
                //getch();
            }
        }
    }

    sort(sizes.begin(), sizes.end());
    reverse(sizes.begin(), sizes.end());

    return sizes[0] * sizes[1] * sizes[2];
}


void run2(const char* filename)
{
    auto lines = aoc::read_lines(filename);
    vector<int> row(lines[0].size()+2, 9);
    vector<vector<int>> input(lines.size()+2, row);

    for (auto i: aoc::range{1U, input.size()-1U})
        for (auto j: aoc::range{1U, input[i].size()-1U})
            input[i][j] = lines[i-1][j-1] - '0';

    auto p1 = part1(input);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 494);

    auto p2 = part2(input);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 1048128);
}


void run(const char* filename)
{
    initscr(); 

    // if (has_colors() == FALSE) 
    // {
    //     endwin();
    //     printf("Your terminal does not support color\n");
    //     return; //exit(1);
    // }

    cbreak();
    noecho();   

    start_color(); // if has_colors()
    init_pair(1, COLOR_RED,   COLOR_BLACK);
    init_pair(2, COLOR_BLUE,  COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);

    run2(filename);

    endwin();
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

        run2(argv[1]);
    }
    catch (std::exception& e)
    {
        cout << e.what();
    }
}

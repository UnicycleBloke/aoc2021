#include "utils.h"


template <typename T>
auto part1(const T& input)
{
    int total_risk = 0;
    for (int i = 1; i <= 100; ++i)
    {
        for (int j = 1; j <= 100; ++j)
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


// Recursively explore the basin
template <typename T>
int basin_size_impl(T& input, int i, int j)
{
    if (input[i][j] < 9)
    {        
        input[i][j] = 10;

        int size = 1;
        size += basin_size_impl(input, i+1, j);
        size += basin_size_impl(input, i-1, j);
        size += basin_size_impl(input, i, j+1);
        size += basin_size_impl(input, i, j-1);
        return size;
    }
    return 0;
}


template <typename T>
auto basin_size(T& input, int i, int j)
{
    // Already counted this cell
    if (input[i][j] == 10) return 0;
    // Not in any basin
    if (input[i][j] == 9) return 0;

    int  height = input[i][j];
    bool low = true;
    low = low && (height < input[i+1][j]);
    low = low && (height < input[i-1][j]);
    low = low && (height < input[i][j+1]);
    low = low && (height < input[i][j-1]);

    if (low)
    {        
        return basin_size_impl(input, i, j);
    }
    else
    {
        return 0;
    }
}


template <typename T>
auto part2(T& input)
{
    vector<int> sizes;
    for (int i = 1; i <= 101; ++i)
    {
        for (int j = 1; j <= 101; ++j)
        {            
            int size = basin_size(input, i, j);
            sizes.push_back(size);
        }
    }

    sort(sizes.begin(), sizes.end());
    reverse(sizes.begin(), sizes.end());

    return sizes[0] * sizes[1] * sizes[2];
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename);
    array<array<int, 102>, 102> input{};

    // Use a grid with a border of 9s to obviate complicated boundary tests.
    for (int i = 0; i <= 101; ++i)
    {
        for (int j = 0; j <= 101; ++j)
        {
            input[i][j] = 9;
        }
    }

    for (int i = 1; i <= lines.size(); ++i)
    {
        for (int j = 1; j <= lines[0].size(); ++j)
        {
            input[i][j] = lines[i-1][j-1] - '0';
        }
    }

    auto p1 = part1(input);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 494);

    auto p2 = part2(input);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 1048128);
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

#include "utils.h"
#include <cassert>


using namespace std;


template <typename T>
vector<int> count_ones(const T& input)
{
    const size_t SIZE = input[0].size();

    vector<int> ones(SIZE, 0);
    for (size_t i = 0; i < SIZE; ++i)
    {
        ones[i] = count_if(input.begin(), input.end(), [i](auto line){ return line[i] == '1'; });
    }
    return ones;
}


template <typename T>
int part1(const T& input)
{
    const auto SIZE = input[0].size();
    auto ones = count_ones(input);

    int gamma = 0;
    size_t limit = input.size() / 2;
    for (size_t i = 0; i < SIZE; ++i)
    {
        gamma *= 2;
        gamma += ones[i] > limit;
    }

    int epsilon = ((1 << SIZE) - 1 - gamma);
    return gamma * epsilon;
}


template <typename T, typename OP>
int part2_impl(const T& input, int col = 0)
{  
    const auto SIZE = input[0].size();

    if (input.size() > 1)
    {
        auto ones = count_ones(input);

        vector<string> keep;
        size_t limit = input.size();

        OP op;
        auto check = op((ones[col] * 2), limit) ? '1' : '0';
        copy_if(input.begin(), input.end(), back_inserter(keep), 
            [col, check](auto line) { return line[col] == check; });

        return part2_impl<T, OP>(keep, col + 1);
    }
    else
    {
        int result = 0;
        for (size_t i = 0; i < SIZE; ++i)
        {
            result *= 2;
            result += (input[0][i] == '1');
        }
        return result;
    }
}


template <typename T>
int part2(const T& input)
{
    auto oxygen = part2_impl<decltype(input), greater_equal<int>>(input);
    auto carbon = part2_impl<decltype(input), less<int>>(input);
    return oxygen * carbon;
}


int main(int argc, char** argv)
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

    auto input = aoc::read_lines(argv[1], false);
    
    auto p1 = part1(input);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 3309596);

    auto p2 = part2(input);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 2981085);
}

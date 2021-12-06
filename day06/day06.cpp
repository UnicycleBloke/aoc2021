#include "utils.h"


uint64_t reproduce(vector<int> input, int days)
{
    array<uint64_t, 9> n{};

    uint64_t total = input.size();
    for (int f: input)
    {
        ++n[(f + 1)];
    }

    for (int d = 0; d <= days; ++d)
    {
        auto rep = n[0];
        rotate(n.begin(), n.begin() + 1, n.end());
        total += rep;
        n[6]  += rep;
        n[8]   = rep;
    }

    return total;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename);
    auto input = aoc::make_vector<int>(lines[0]);

    auto p1 = reproduce(input, 80);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 390011UL);

    auto p2 = reproduce(input, 256);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 1746710169834UL);
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

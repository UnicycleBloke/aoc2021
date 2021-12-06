#include "utils.h"


int part1(vector<int> input)
{
    for (int c = 0; c < 80; ++c)
    {
        vector<int> temp;
        temp.reserve(1000000);
        for (int f: input)
        {
            if (f > 0)
            {
                temp.push_back(f - 1);
            }
            else
            {
                temp.push_back(6);
                temp.push_back(8);
            }
        }

        input = temp;
        cout << " " << input.size();
    }

    return input.size();
}


uint64_t part2(vector<int> input)
{
    array<uint64_t, 10> n{};

    uint64_t total = input.size();
    for (int f: input)
    {
        ++n[(f + 1)];
    }

    for (int d = 0; d < 257; ++d)
    {
        auto temp = n[0]; 
        total += temp;

        n[0] = n[1];
        n[1] = n[2];
        n[2] = n[3];
        n[3] = n[4];
        n[4] = n[5];
        n[5] = n[6];
        n[6] = n[7] + temp;
        n[7] = n[8];
        n[8] = temp;
    }

    return total;
}


void run(const char* filename)
{
    ifstream is{filename};
    vector<int> input;
    while (is)
    {
        int i;
        char c;
        is >> i >> c;
        input.push_back(i);
    }

    //auto p1 = part1(input);
    //cout << "Part1: " << p1 << '\n';
    //aoc::check_result(p1, 0);

    auto p2 = part2(input);
    cout << "Part2: " << p2 << '\n';
    //aoc::check_result(p2, 0);
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

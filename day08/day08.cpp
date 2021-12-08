#include "utils.h"


int to_int(string s)
{
    int res = 0;
    for (auto c: s)
    {
        switch (c)
        {
            case 'a': res += (1 << 0); break;
            case 'b': res += (1 << 1); break;
            case 'c': res += (1 << 2); break;
            case 'd': res += (1 << 3); break;
            case 'e': res += (1 << 4); break;
            case 'f': res += (1 << 5); break;
            case 'g': res += (1 << 6); break;
        }
    }
    return res;
}


void run(const char* filename)
{
    size_t part1 = 0;
    size_t part2 = 0;

    auto lines = aoc::read_lines(filename);
    for (auto line: lines)
    {
        auto s      = aoc::split(line, "|", false);
        auto segs   = aoc::split(s[0], " ", false);
        auto digits = aoc::split(s[1], " ", false);

        // Part 1
        part1 += count_if(digits.begin(), digits.end(), [](auto s) { return s.size() == 2; });
        part1 += count_if(digits.begin(), digits.end(), [](auto s) { return s.size() == 4; });
        part1 += count_if(digits.begin(), digits.end(), [](auto s) { return s.size() == 3; });
        part1 += count_if(digits.begin(), digits.end(), [](auto s) { return s.size() == 7; });

        // Part 2
        array<size_t, 10> si{};
        map<size_t, size_t> sm{};

        for (auto s: segs)
        {
            switch (s.size())
            {
                case 2: si[1] = to_int(s); sm[si[1]] = 1; break;
                case 4: si[4] = to_int(s); sm[si[4]] = 4; break;
                case 3: si[7] = to_int(s); sm[si[7]] = 7; break;
                case 7: si[8] = to_int(s); sm[si[8]] = 8; break;
            }
        }

        // Got  1,4,7,8
        // Need 2,3,5,6,9,0
        for (auto s: segs)
        {
            switch (s.size())
            {
                case 5: // 2,3,5
                    if ((to_int(s) & si[1]) == si[1])
                    {
                        si[3] = to_int(s);
                        sm[si[3]] = 3;
                    }
                    break;

                case 6: // 0,9,6
                    if ((to_int(s) & si[4]) == si[4])
                    {
                        si[9] = to_int(s);
                        sm[si[9]] = 9;
                    }
                    if ((to_int(s) & si[7]) != si[7])
                    {
                        si[6] = to_int(s);
                        sm[si[6]] = 6;
                    }
                    break;
            }
        }

        // Got  1,3,4,6,7,8,9
        // Need 2,5,0
        for (auto s: segs)
        {
            switch (s.size())
            {
                case 5: // 2,5
                    if ((to_int(s) & si[6]) == to_int(s))
                    {
                        si[5] = to_int(s);
                        sm[si[5]] = 5;
                    }
                    break;

                case 6: // 0
                    if (to_int(s) != si[6] && to_int(s) != si[9])
                    {
                        si[0] = to_int(s);
                        sm[si[0]] = 0;
                    }
                    break;
            }
        }

        // Got  1,3,4,5,6,7,8,9,0
        // Need 2
        for (auto s: segs)
        {
            switch (s.size())
            {
                case 5: // 2
                    if (to_int(s) != si[3] && to_int(s) != si[5])
                    {
                        si[2] = to_int(s);
                        sm[si[2]] = 2;
                    }
                    break;
            }
        }

        size_t val = 0;
        val += 1000 * sm[to_int(digits[0])];
        val += 100 * sm[to_int(digits[1])];
        val += 10 * sm[to_int(digits[2])];
        val += sm[to_int(digits[3])];

        part2 += val;
    }

    cout << "Part 1; " << part1 << '\n';
    cout << "Part 2; " << part2 << '\n';
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

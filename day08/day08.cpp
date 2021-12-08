#include "utils.h"


template <typename T>
auto part1(const T& input)
{
    auto fuel = numeric_limits<typename T::value_type>::max();
    for (auto i = 0; i < *max_element(input.begin(), input.end()); ++i)
        fuel = min(fuel, accumulate(input.begin(), input.end(), 0, [i](auto sum, auto j)
        { 
            return sum + abs(i - j); 
        }));
    return fuel;
}


template <typename T>
auto part2(const T& input)
{
    auto fuel = numeric_limits<typename T::value_type>::max();
    for (auto i = 0; i < *max_element(input.begin(), input.end()); ++i)
        fuel = min(fuel, accumulate(input.begin(), input.end(), 0, [i](auto sum, auto j)
        { 
            auto f = abs(i - j); 
            return sum + f * (f + 1) / 2; 
        }));
    return fuel;
}


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
    int n1 = 0;
    int n4 = 0;
    int n7 = 0;
    int n8 = 0;

    int total = 0;
    auto lines = aoc::read_lines(filename);
    for (auto line: lines)
    {
        auto s      = aoc::split(line, "|", false);
        auto segs   = aoc::split(s[0], " ", false);
        auto digits = aoc::split(s[1], " ", false);

        n1 += count_if(digits.begin(), digits.end(), [](auto s) { return s.size() == 2; });
        n4 += count_if(digits.begin(), digits.end(), [](auto s) { return s.size() == 4; });
        n7 += count_if(digits.begin(), digits.end(), [](auto s) { return s.size() == 3; });
        n8 += count_if(digits.begin(), digits.end(), [](auto s) { return s.size() == 7; });

        array<int, 10> si{};
        map<int, int> sm{};

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

        // Got 1,4,7,8 
        for (auto s: segs)
        {
            switch (s.size())
            {
                case 5: // 2,3,5
                    if ((to_int(s) & si[1]) == si[1])
                    {
                        cout << "got 3: " << s << "\n";
                        si[3] = to_int(s);
                        sm[si[3]] = 3;
                    }
                    break;

                case 6: // 0,9,6
                    if ((to_int(s) & si[4]) == si[4])
                    {
                        cout << "got 9: " << s << "\n";
                        si[9] = to_int(s);
                        sm[si[9]] = 9;
                    } 
                    if ((to_int(s) & si[7]) != si[7])
                    {
                        cout << "got 6: " << s << "\n";
                        si[6] = to_int(s);
                        sm[si[6]] = 6;
                    } 
                    break;
            }
        } 

        // Got 1,3,4,6,7,8,9 
        for (auto s: segs)
        {
            switch (s.size())
            {
                case 5: // 2,5
                    if ((to_int(s) & si[6]) == to_int(s))
                    {
                        cout << "got 5: " << s << "\n";
                        si[5] = to_int(s);
                        sm[si[5]] = 5;
                    }
                    break;

                case 6: // 0
                    if (to_int(s) != si[6] && to_int(s) != si[9])
                    {
                        cout << "got 0: " << s << "\n";
                        si[0] = to_int(s);
                        sm[si[0]] = 0;
                    } 
                    break;
            }
        } 

        for (auto s: segs)
        {
            switch (s.size())
            {
                case 5: // 2
                    if (to_int(s) != si[3] && to_int(s) != si[5])
                    {
                        cout << "got 2: " << s << "\n";
                        si[2] = to_int(s);
                        sm[si[2]] = 2;
                    } 
                    break;
            }
        } 

        // for (auto [k, v] : sm)
        // {
        //     cout << k << ':' << v << '\n';
        // }
        // cout << '\n';

        int val = 0;
        val += 1000 * sm[to_int(digits[0])];
        val += 100 * sm[to_int(digits[1])];
        val += 10 * sm[to_int(digits[2])];
        val += sm[to_int(digits[3])];

        //for (auto d: digits)        
        //    cout << d << '>' << sm[to_int(d)] << '\n';
        cout << val << '\n';
        total += val;
    }
    //auto input = aoc::make_vector<int>(lines[0]);
    cout << total << '\n';

    cout << n1 << ' ' << n4 << ' ' << n7 << ' ' << n8 << ' ' << (n1+n4+n7+n8);

    // auto p1 = part1(input);
    // cout << "Part1: " << p1 << '\n';
    // aoc::check_result(p1, 340052);

    // auto p2 = part2(input);
    // cout << "Part2: " << p2 << '\n';
    // aoc::check_result(p2, 92948968);
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

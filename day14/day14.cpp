#include "utils.h"


using Pair  = pair<char, char>; 
using State = map<Pair, uint64_t>;
using Rules = map<Pair, pair<Pair, Pair>>;


// It seems I could just directly manipulate a frequency counting table rather 
// than the indirection I have here. 
auto grow(State src, Rules& rules)
{
    State dst;

    for (auto [k, v]: src)
    {
        auto rule = rules[k];
        auto A = rule.first;
        auto B = rule.second;

        if (dst.find(A) == dst.end()) dst[A] = 0;
        if (dst.find(B) == dst.end()) dst[B] = 0;

        dst[A] += v;
        dst[B] += v;
    }

    return dst;
}


auto solve(State init, Pair ends, Rules& rules, int steps)
{
    aoc::timer timer;

    uint64_t cmax =  0;
    uint64_t cmin = -1;

    // Grow the numbers of pairs of characters in use.
    for (int i: aoc::range(steps))
        init = grow(init, rules);

    // Extract the numbers of each pair and sum for each 
    // character. This results in double counting. We have 
    // to adjust for the ends, which are not double counted
    // in the loop.
    map<char, uint64_t> counts;
    counts[ends.first] = 1;
    counts[ends.second] = (ends.second == ends.first) ? 2 : 1;

    for (auto [k, v]: init)
    {
        auto A = k.first;
        auto B = k.second;

        if (counts.find(A) == counts.end()) counts[A] = 0;
        if (counts.find(B) == counts.end()) counts[B] = 0;

        counts[A] += v;
        counts[B] += v;
    }

    // Extract the min and max. A map was easier than trying to 
    // browbeat min_element to ignore zeroes.
    for (auto [k, v]: counts)
    {
        v = v / 2;
        cmax = max(cmax, v);
        cmin = min(cmin, v);
    }

    return cmax - cmin;
}


void run(const char* filename)
{
    aoc::timer timer;

    auto init = aoc::read_lines(filename)[0];
    auto input   = aoc::read_lines<char, char, char>(filename, R"((\w)(\w)\s->\s(\w))");

    Rules rules;
    for (auto [a, b, c] : input)
    {
        Pair A{a, b};
        Pair B{a, c};
        Pair C{c, b};
        rules[A] = {B, C};
    }

    State state;
    for (size_t i: aoc::range(init.size()-1))
    {
        auto a = init[i];
        auto b = init[i+1];
        Pair A{a, b};
        if (state.find(A) == state.end()) state[A] = 0;
        state[A] += 1;
    }

    // Needed to adjust to totals because the end characters are counted once 
    // but all the others are counted twice. Result is half the max - min.
    Pair ends{*init.begin(), *(init.end()-1)};

    auto p1 = solve(state, ends, rules, 10);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 2027U);

    auto p2 = solve(state, ends, rules, 40);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 2265039461737U);
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

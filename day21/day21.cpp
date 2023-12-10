#include "utils.h"


int roll_count = 0;
int roll()
{
    static int value = -1;

    ++roll_count;
    value = (value + 1) % 100;
    return value +1;
}


auto part1(int p1, int p2)
{
    aoc::timer timer;

    int s1 = 0;
    int s2 = 0;

    while (true)
    {
        int r1 = roll() + roll() + roll();
        p1 = (p1 + r1 - 1) % 10 + 1;
        s1 += p1;
        if (s1 >= 1000) break;

        int r2 = roll() + roll() + roll();
        p2 = (p2 + r2 - 1) % 10 + 1;
        s2 += p2;
        if (s2 >= 1000) break;
    }

    return min(s1, s2) * roll_count;
}


struct Wins
{
    uint64_t p1{};
    uint64_t p2{};

    Wins& operator+=(Wins w)
    {
        p1 += w.p1;
        p2 += w.p2;
        return *this;
    };

    friend Wins operator+(Wins w1, Wins w2)
    {
        return Wins{w1.p1 + w2.p1, w1.p2 + w2.p2 };
    }

    friend Wins operator*(Wins w, int s)
    {
        return Wins{w.p1 * s, w.p2 * s};
    }
};



// Beware key collisions when creating a hash and using a regular map.
size_t make_key(const vector<int>& vec) 
{
    size_t seed = 0;   
    for(auto& i : vec)
        seed = (seed * 100) + i;
    return seed;
}


// Sum of three rolls of the Dirac Die
const array<int, 9> rolls{3,4,5,6,7,8,9};
// Frequency of each sum - an optimisation.
const array<int, 9> scale{1,3,6,7,6,3,1};


Wins play2(int turn, int s1, int p1, int r1, int s2, int p2, int r2);


map<size_t, Wins> games;


Wins play1(int turn, int s1, int p1, int r1, int s2, int p2, int r2)
{
    p1  = (p1 + r1 - 1) % 10 + 1;
    s1 += p1;
    if (s1 >= 21) return {1U, 0U};

    // Have we seen this state vector before? What were the counts?
    vector<int> x{1, s1, p1, s2, p2};
    auto k = make_key(x); 
    if (games.find(k) != games.end()) return games[k];

    Wins result{};
    for (auto i: aoc::range{rolls.size()})
        result += play2(turn, s1, p1, r1, s2, p2, rolls[i]) * scale[i];

    games[k] = result;
    return result;
}


Wins play2(int turn, int s1, int p1, int r1, int s2, int p2, int r2)
{
    p2  = (p2 + r2 - 1) % 10 + 1;
    s2 += p2;
    if (s2 >= 21) return {0U, 1U};

    // Have we seen this state vector before? What were the counts?
    vector<int> x{2, s1, p1, s2, p2};
    auto k = make_key(x); 
    if (games.find(k) != games.end()) return games[k];

    Wins result{};
    for (auto i: aoc::range{rolls.size()})
        result += play1(turn+1, s1, p1, rolls[i], s2, p2, r2) * scale[i];

    games[k] = result;
    return result;
}


auto part2(int p1start, int p2start)
{
    aoc::timer timer;

    Wins result{};
    for (auto i: aoc::range{rolls.size()})
        result += play1(1, 0, p1start, rolls[i], 0, p2start, 0) * scale[i];
    return max(result.p1, result.p2);
}


int main(int argc, char** argv)
{
    auto p1 = part1(4, 10);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 855624U);

    auto p2 = part2(4, 10);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 187451244607486U);
}

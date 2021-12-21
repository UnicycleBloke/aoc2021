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
};


size_t make_key(const vector<int>& vec) 
{
    size_t seed = 0;   
    for(auto& i : vec)
        seed = (seed * 100) + i;
    return seed;
}


const array<int, 27> rolls
{
    3,4,5, 4,5,6, 5,6,7, 
    4,5,6, 5,6,7, 6,7,8, 
    5,6,7, 6,7,8, 7,8,9
};


Wins play2(int turn, int s1, int p1, int r1, int s2, int p2, int r2);


map<size_t, Wins> games;


Wins play1(int turn, int s1, int p1, int r1, int s2, int p2, int r2)
{
    p1  = (p1 + r1 - 1) % 10 + 1;
    s1 += p1;
    if (s1 >= 21) return {1U, 0U};

    vector<int> x{turn, 1, s1, p1, r1, s2, p2, r2};
    auto k = make_key(x); 
    if (games.find(k) != games.end()) return games[k];

    Wins result{};
    for (auto i: aoc::range{rolls.size()})
        result += play2(turn, s1, p1, r1, s2, p2, rolls[i]);

    games[k] = result;
    return result;
}


Wins play2(int turn, int s1, int p1, int r1, int s2, int p2, int r2)
{
    p2  = (p2 + r2 - 1) % 10 + 1;
    s2 += p2;
    if (s2 >= 21) return {0U, 1U};

    vector<int> x{turn, 2, s1, p1, r1, s2, p2, r2};
    auto k = make_key(x); 
    if (games.find(k) != games.end()) return games[k];

    Wins result{};
    for (auto i: aoc::range{rolls.size()})
        result += play1(turn+1, s1, p1, rolls[i], s2, p2, r2);

    games[k] = result;
    return result;
}


auto part2(int p1start, int p2start)
{
    Wins result{};
    for (auto i: aoc::range{rolls.size()})
        result += play1(1, 0, p1start, rolls[i], 0, p2start, 0);
    return max(result.p1, result.p2);
}


int main(int argc, char** argv)
{
    auto p1 = part1(9, 6);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 1'004'670);

    auto p2 = part2(9, 6);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 492'043'106'122'795U);
}

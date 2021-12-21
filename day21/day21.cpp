#include "utils.h"
//#include "cpp_curses.h"


// void render(const vector<vector<int>>& grid)
// {
//     static int frame = 0;
    
//     clear();

//     for (auto r: aoc::range(1U, grid.size()-1))
//     {
//         for (auto c: aoc::range(1U, grid[0].size()-1))
//         {
//             aoc::write((grid[r][c] == 0) ? '.' : '#', c, r);
//         }
//     }

//     ostringstream os;
//     os << "Frame: " << frame++ << " ";
//     aoc::write(os.str(), 2, 2);

//     refresh();
//     getch();
// }


int rolls = 0;
int roll()
{
    static int value = -1;

    ++rolls;
    value = (value + 1) % 100;
    return value +1;
}


auto part1()
{
    int p1 = 4; //9;
    int p2 = 8; // 6;

    int s1 = 0;
    int s2 = 0;

    while (true)
    {
        int r1 = roll() + roll() + roll();
        p1 = (p1 + r1 - 1) % 10 + 1;
        s1 += p1;
        ///cout << "p1 " << p1 << " " << s1 << '\n';
        if (s1 >= 1000) break;

        int r2 = roll() + roll() + roll();
        p2 = (p2 + r2 - 1) % 10 + 1;
        s2 += p2;
        //cout << "p2 " << p2 << " " << s2 << '\n';
        if (s2 >= 1000) break;
    }

    //cout << "p1 " << p1 << " " << s1 << '\n';
    //cout << "p2 " << p2 << " " << s2 << '\n';

    return min(s1, s2) * rolls;
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


map<size_t, Wins> games;
int keys = 0;


size_t make_key(const vector<int>& vec) 
{
    size_t seed = vec.size();
    for(auto& i : vec) 
        seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    return seed;
}


Wins wins(int turn, bool play1, int s1, int p1, int r1, int s2, int p2, int r2)
{
    constexpr array<int, 27> rolls
    {
        3,4,5, 4,5,6, 5,6,7, 
        4,5,6, 5,6,7, 6,7,8, 
        5,6,7, 6,7,8, 7,8,9
    };

    vector<int> x{turn, play1 ? 1 : 2, s1, p1, r1, s2, p2, r2};
    size_t k = make_key(x); 
    if (games.find(k) != games.end()) return games[k];

    if (play1)
    {
        p1  = (p1 + r1 - 1) % 10 + 1;
        s1 += p1;
        if (s1 >= 21) return {1U, 0U};

        Wins result{};
        for (auto i: aoc::range{rolls.size()})
            result += wins(turn+1, false, s1, p1, 0, s2, p2, rolls[i]);

        games[k] = result;
        return result;
    }
    else
    {        
        p2  = (p2 + r2 - 1) % 10 + 1;
        s2 += p2;
        if (s2 >= 21) return {0U, 1U};

        Wins result{};
        for (auto i: aoc::range{rolls.size()})
            result += wins(turn+1, true, s1, p1, rolls[i], s2, p2, 0);

        games[k] = result;
        return result;
    }

    return {};
}


auto part2()
{
    constexpr array<int, 27> rolls
    {
        3,4,5, 4,5,6, 5,6,7, 
        4,5,6, 5,6,7, 6,7,8, 
        5,6,7, 6,7,8, 7,8,9
    };

    int p1start = 4;
    int p2start = 8;

    Wins result;
    // 27 possible universes in which P1 can win.
    for (auto i: aoc::range{rolls.size()})
        result += wins(1, true, 0, p1start, rolls[i], 0, p2start, 0);

    cout << p1start << ' ' << p2start << '\n';
    cout << result.p1 << '\n';       
    cout << result.p2 << '\n';      

    // for (auto [k,v]: games)
    // {
    //     auto [p1, p2] = v;
    //     cout << k << ' ' << p1 << ' ' << p2 << '\n';       
    // } 
    // cout << keys << ' ' << games.size() << '\n';       

    return max(result.p1, result.p2);
}


void run(const char* filename)
{
    auto p1 = part1();
    cout << "Part1: " << p1 << '\n';
    //aoc::check_result(p1, 0);

    auto p2 = part2();
    cout << "Part2: " << p2 << '\n';
    //aoc::check_result(p2, 0);
}


int main(int argc, char** argv)
{
    //aoc::CursesApp app;

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

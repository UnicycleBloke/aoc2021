#include "utils.h"


// This problem initially looked like writing an interpreter for a simple assembly language.
// I wasted time doing so - a massive distraction. Analysis of the input shows that the code 
// is a list a fourteen functions/stages identical aside from three constants. This table shows the 
// constants for each functions.
constexpr int64_t constants[14][3] =
{
    // DIV, ADD1, ADD2
    {  1, 13,  6 },  
    {  1, 11, 11 },  
    {  1, 12,  5 },  
    {  1, 10,  6 },  
    {  1,  14,  8 }, 
    { 26,  -1, 14 },
    {  1,  14,  9 }, 
    { 26, -16,  4 },
    { 26,  -8,  7 },
    {  1,  12, 13 }, 
    { 26, -16, 11 },
    { 26, -13, 11 },
    { 26, -6, 6 },
    { 26, -6, 1 },
};


// The key insight that I overlooked was how to optimise the calculation. The final result 
// of the calculation must place 0 into the Z register. This is only possible if the Zin value
// for the final stage is lower than the DIV. For each earlier stage, multiply the DIV values 
// of the following stages to find the maximum Zin that could possibly work. 
constexpr int64_t calc_max_zout(int prog)
{
    int64_t z = 1;
    for (int p = prog; p < 14; ++p)
        z *= constants[p][0];
    return z;
}


constexpr int64_t max_zout[14] =
{
    calc_max_zout(0),
    calc_max_zout(1),
    calc_max_zout(2),
    calc_max_zout(3),
    calc_max_zout(4),
    calc_max_zout(5),
    calc_max_zout(6),
    calc_max_zout(7),
    calc_max_zout(8),
    calc_max_zout(9),
    calc_max_zout(10),
    calc_max_zout(11),
    calc_max_zout(12),
    calc_max_zout(13)
};


// Each stage in input code basically boils down to this function. The values of 
// the X and Y registers are irrelevant. We only care about the digit read into W and 
// the existing value in Z, Zin. It turns out that DIV is either 1 or 26. Zout will be 
// at most ~Zin * 26.
int64_t calc_zout(int prog, int64_t digit, int64_t zin)
{
    const int64_t DIV  = constants[prog][0];
    const int64_t ADD1 = constants[prog][1];
    const int64_t ADD2 = constants[prog][2];

    if (((zin % 26) + ADD1) == digit)
        return zin / DIV;
    return (zin / DIV) * 26 + digit + ADD2;
}


// DFS to find the MONAD we need.
void solve(int prog, int64_t zin, int64_t working, bool& done, int64_t& result, bool up)
{   
    if (prog == 14)
    {
        if (zin == 0)
        {
            done   = true;
            result = working;
        }
        return;
    }

    if (done) return;

    if (zin >= max_zout[prog])
        return;

    // Part 1 counts digits downwards to find largest solution first.
    // Part 2 counts digits upwards to find smallest solution first.
    int wfrom = up ?  1 :  9;
    int wto   = up ? 10 :  0;
    int wstep = up ?  1 : -1;

    for (auto digit: aoc::range(wfrom, wto, wstep))
    {
        auto zout = calc_zout(prog, digit, zin);
        auto working2 = working * 10 + digit;
        solve(prog+1, zout, working2, done, result, up);
    }
}


int64_t part1()
{
    aoc::timer timer;

    bool done = false;
    int64_t result = 0;
    solve(0, 0, 0, done, result, false);

    return result;
}


int64_t part2()
{
    aoc::timer timer;

    bool done = false;
    int64_t result = 0;
    solve(0, 0, 0, done, result, true);

    return result;
}


void run(const char* filename)
{
    auto p1 = part1();
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 94992992796199);

    auto p2 = part2();
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 11931881141161);
}


int main(int argc, char** argv)
{
    aoc::timer timer;
    try
    {
        run(argv[1]);
    }
    catch (std::exception& e)
    {
        cout << e.what();
    }
}

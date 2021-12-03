#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <regex>
#include "utils.h"


using namespace std;


int part1(const vector<int>& depths)
{
    int count = 0;
    for (int i = 1; i < depths.size(); ++i)
    {
        count += depths[i] > depths[i - 1];
    }

    return count;
}


// This misses a trick - no need to sum - just compare the to ends of the window.
int part2(const vector<int>& depths)
{
    vector<int> windows;
    for (int i = 2; i < depths.size(); ++i)
    {
        int sum = depths[i] + depths[i-1] + depths[i-2];
        windows.push_back(sum);
    }

    return part1(windows);
}


int main(int argc, char** argv)
{
    if (argc < 2)
    {
        cout << "Provide input file name\n";
        return -1;
    }

    ifstream is{argv[1]};
    auto depths = aoc::read_ints<int>(is);

    cout << "Part1: " << part1(depths) << '\n';
    cout << "Part2: " << part2(depths) << '\n';
}

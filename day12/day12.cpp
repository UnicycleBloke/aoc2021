#include "utils.h"
#include <chrono>


using Map = map<string, vector<string>>;


namespace cron = std::chrono;
class Timer
{

public:
    Timer()
    : start{cron::steady_clock::now()}
    {
    }
    ~Timer()
    {
        auto end = cron::steady_clock::now();
        cron::duration<double> diff = end - start;
        std::cout << "Time elapsed: " << diff.count() << '\n';
    }

private:
    cron::time_point<cron::steady_clock> start{}; 
};


void search(Map& paths, string curr, int& count, vector<string>& path, string twice)
{
    if (curr == "end")
    {
        ++count;
        return;
    }

    const auto& caves = paths[curr];
    for (const auto& cave: caves)
    {
        if (cave == "start")
            continue;

        auto twice2 = twice;
        // Is this a small cave?
        if ((cave[0] >= 'a') && (cave[0] <= 'z'))
        {
            // Is the cave already on our path?
            if (find(path.begin(), path.end(), cave) != path.end())
            {
                // Have we already visited a small cave twice
                if (twice != "Part2")
                    continue;
                twice2 = cave;
            }
        }

        path.push_back(cave);        
        search(paths, cave, count, path, twice2);
        path.pop_back();        
    }
}


// Alternative without recursion - based on u/Biggergig's solution
auto search2(Map& paths)
{
    Timer timer;

    int result1{}, result2{};

    using Node = tuple<string, set<string>, bool>;

    queue<Node> nodes;
    nodes.push(make_tuple("start", set<string>{"start"}, false));

    while (!nodes.empty())
    {
        auto [cave, small, seen] = nodes.front();
        nodes.pop();

        if (cave == "end")
        {
            if (!seen)
                ++result1;
            ++result2;
        }
        else
        {
            for (auto& cave2: paths[cave])
            {
                // Is this a large cave?
                if (!islower(cave2[0]))
                {
                    // Yes - visit this cave
                    nodes.push({cave2, small, seen});
                }
                else
                {
                    // It's a small cave
                    // Have we seen it already?
                    if (small.find(cave2) == small.end())
                    {
                        // No - visit this cave
                        auto small2 = small;
                        small2.insert(cave2);
                        nodes.push({cave2, small2, seen});
                    }
                    else
                    {
                        // Yes - visit this cave if we haven't visited any small cave twice
                        if (!seen)
                            nodes.push({cave2, small, true});
                    }
                }
            }
        }        
    }

    return pair{result1, result2};
}


template <typename T>
auto part1(T paths)
{ 
    Timer timer;

    int count = 0;   
    vector<string> path{"start"};
    search(paths, "start", count, path, "Part1");
    return count;
}


template <typename T>
auto part2(T paths)
{
    Timer timer;
    int count = 0;   

    vector<string> path{"start"};
    search(paths, "start", count, path, "Part2");
    return count;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines<string, string>(filename, R"((\w+)-(\w+))");

    Map input;
    for (auto [from, to]: lines)
    {
        if (input.find(from) == input.end()) input[from] = vector<string>{};
        if (input.find(to)   == input.end()) input[to]   = vector<string>{};
        
        // Redundant
        if (to != "start")   input[from].push_back(to);
        if (from != "start") input[to].push_back(from);
    }

    auto p1 = part1(input);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 3410);

    auto p2 = part2(input);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 98796);

    auto [q1, q2] = search2(input);
    cout << "Part1: " << q1 << '\n';
    aoc::check_result(q1, 3410);
    cout << "Part2: " << q2 << '\n';
    aoc::check_result(q2, 98796);
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

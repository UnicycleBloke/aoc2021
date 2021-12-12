#include "utils.h"


using Map = map<string, vector<string>>;


void search(Map& paths, string curr, int& count, vector<string> path, string twice)
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

        auto path2 = path;
        path2.push_back(cave);        
        search(paths, cave, count, path2, twice2);
    }
}


template <typename T>
auto part1(T paths)
{ 
    int count = 0;   
    search(paths, "start", count, vector<string>{"start"}, "Part1");
    return count;
}


template <typename T>
auto part2(T paths)
{
    int count = 0;   
    search(paths, "start", count, vector<string>{"start"}, "Part2");
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
        input[from].push_back(to);
        input[to].push_back(from);
    }

    auto p1 = part1(input);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 3410);

    auto p2 = part2(input);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 98796);
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

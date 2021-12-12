#include "utils.h"


struct Node
{
    string        name{};
    vector<Node*> caves{};
    int           count{};
};


void search(Node* curr, Node* twice, int& part1, int& part2)
{
    if (curr->name == "end")
    {
        if (!twice)
            ++part1;
        ++part2;
        return;
    }

    for (auto cave: curr->caves)
    {
        if (cave->name == "start")
            continue;

        if (islower(cave->name[0]))
        {
            if (cave->count > 0)
            {
                if (!twice)
                    twice = cave;
                else
                    continue;
            } 
        }

        ++cave->count;   
        search(cave, twice, part1, part2);
        --cave->count;

        if (twice == cave)
            twice = nullptr;   
    }
}


void run(const char* filename)
{
    auto lines = aoc::read_lines<string, string>(filename, R"((\w+)-(\w+))");

    map<string, Node*> input;
    for (auto [from, to]: lines)
    {
        // Very naughty - couldn't be asked fighting with the compiler over smart pointers.
        if (input.find(from) == input.end()) input[from] = new Node{from};
        if (input.find(to)   == input.end()) input[to]   = new Node{to};
        
        // Redundant
        input[from]->caves.push_back(input[to]);
        input[to]->caves.push_back(input[from]);
    }

    aoc::timer timer;

    int p1{}, p2{};
    search(input["start"], nullptr, p1, p2);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 3410);
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

#include "utils.h"


template <typename T>
auto part1(T input)
{
    aoc::timer timer;

    const uint32_t COLS = static_cast<uint32_t>(input[0].size());
    const uint32_t ROWS = static_cast<uint32_t>(input.size());

    using Pos = pair<uint32_t, uint32_t>;

    // 1. Mark all nodes unvisited. Create a set of all the unvisited nodes called the unvisited set.
    set<Pos> unvisited;
    for (auto r: aoc::range{ROWS})
        for (auto c: aoc::range{COLS})
            unvisited.insert({r, c});

    // 2. Assign to every node a tentative distance value: set it to zero for our initial node 
    // and to infinity for all other nodes. The tentative distance of a node v is the length of 
    // the shortest path discovered so far between the node v and the starting node. Since initially 
    // no path is known to any other vertex than the source itself (which is a path of length zero), 
    // all other tentative distances are initially set to infinity. Set the initial node as current.
    vector<uint32_t> distance_row(COLS, 0xFFFF'FFFF); // Infinity
    vector<vector<uint32_t>> distance(ROWS, distance_row);
    distance[0][0] = 0;
    Pos current = {0, 0};

    while (unvisited.size() > 0)
    {
        // 3. For the current node, consider all of its unvisited neighbors and calculate their tentative 
        // distances through the current node. Compare the newly calculated tentative distance to the current 
        // assigned value and assign the smaller one. For example, if the current node A is marked with a 
        // distance of 6, and the edge connecting it with a neighbor B has length 2, then the distance to B 
        // through A will be 6 + 2 = 8. If B was previously marked with a distance greater than 8 then change 
        // it to 8. Otherwise, the current value will be kept.
        auto [x, y] = current;

        // All neighbours
        vector<Pos> neighbours;
        if (x < (COLS-1)) neighbours.push_back({x + 1, y});
        if (y < (ROWS-1)) neighbours.push_back({x, y + 1});
        if (x > 0)        neighbours.push_back({x - 1, y});
        if (y > 0)        neighbours.push_back({x, y - 1});

        for (auto pos: neighbours)
        {
            // Ignore neighbours already visited
            if (unvisited.find(pos) == unvisited.end())
                continue;

            // Maybe reduce the tentative distance
            auto [i, j] = pos;    
            distance[i][j] = min(distance[i][j], distance[x][y] + input[i][j]);
        }

        // 4. When we are done considering all of the unvisited neighbors of the current node, mark the 
        // current node as visited and remove it from the unvisited set. A visited node will never be 
        // checked again.
        unvisited.erase(current);

        // 5. If the destination node has been marked visited (when planning a route between two specific nodes) 
        // or if the smallest tentative distance among the nodes in the unvisited set is infinity (when planning 
        // a complete traversal; occurs when there is no connection between the initial node and remaining unvisited 
        // nodes), then stop. The algorithm has finished.
        if (current == Pos{ ROWS-1, COLS-1})
            return distance[ROWS-1][COLS-1];

        // 6. Otherwise, select the unvisited node that is marked with the smallest tentative distance, 
        // set it as the new current node, and go back to step 3.
        uint32_t min_distance = 0xFFFF'FFFF;
        for (auto [x, y]: unvisited)
        {
            if (distance[x][y] < min_distance)
            {
                min_distance = distance[x][y]; 
                current = Pos{x, y};
            }
        }
    }

    return 0xFFFF'FFFF;
}


template <typename T>
auto part2(T input)
{
    aoc::timer timer;
    return 0;
}


void run(const char* filename)
{
    aoc::timer timer;

    auto lines = aoc::read_lines(filename);
    vector<vector<uint32_t>> input;
    for (auto line: lines)
    {
        auto row = aoc::comprehend(line, [](auto c) -> uint32_t { return c - '0'; });  
        input.push_back(row);
    }

    auto p1 = part1(input);
    cout << "Part1: " << p1 << '\n';
    //aoc::check_result(p1, 0);

    auto p2 = part2(input);
    cout << "Part2: is read off the screen\n";
    //aoc::check_result(p2, 0);
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

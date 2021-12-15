#include "utils.h"


template <typename T>
auto dijkstra(T input)
{
    aoc::timer timer;

    const uint32_t COLS = static_cast<uint32_t>(input[0].size());
    const uint32_t ROWS = static_cast<uint32_t>(input.size());

    using Pos = pair<uint32_t, uint32_t>;
    set<Pos> unvisited;
    for (auto r: aoc::range{ROWS})
        for (auto c: aoc::range{COLS})
            unvisited.insert({r, c});

    // This is an optimisation. The algo says to select the node with the 
    // lowest tentative distance to be the next current node. The initial full 
    // set of 250,000 nodes makes this very slow. Use this set to hold the nodes
    // with non-infinite distances but which have not yet been visited - a much 
    // smaller set.
    set<Pos> marked;

    constexpr uint32_t HUGE_DISTANCE = 0xFFFF'FFFF;
    vector<uint32_t> distance_row(COLS, HUGE_DISTANCE); 
    vector<vector<uint32_t>> distance(ROWS, distance_row);
    distance[0][0] = 0;
    Pos current = {0, 0};

    while (unvisited.size() > 0)
    {
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
            marked.insert({i,j});
        }

        unvisited.erase(current);
        marked.erase(current);

        if (current == Pos{ ROWS-1, COLS-1})
            return distance[ROWS-1][COLS-1];

        uint32_t min_distance = HUGE_DISTANCE;
        // A significant optimisation.
        //for (auto [x, y]: unvisited)
        for (auto [x, y]: marked)
        {
            if (distance[x][y] < min_distance)
            {
                min_distance = distance[x][y]; 
                current = Pos{x, y};
            }
        }
    }

    return HUGE_DISTANCE;
}


template <typename T>
auto part1(T input)
{
    aoc::timer timer;
    return dijkstra(input);
}


template <typename T>
auto part2(T input)
{
    aoc::timer timer;

    const uint32_t COLS = static_cast<uint32_t>(input[0].size());
    const uint32_t ROWS = static_cast<uint32_t>(input.size());

    vector<uint32_t> large_row(COLS*5, 0);
    vector<vector<uint32_t>> large(ROWS*5, large_row);

    for (auto r: aoc::range{ROWS})
        for (auto c: aoc::range{COLS})
            auto value = input[r][c];

    for (auto r: aoc::range{ROWS})
    {
        for (auto c: aoc::range{COLS})
        {
            auto orig = input[r][c];
            auto roff = 0;
            auto coff = 0;

            for (auto i: aoc::range{0, 5})
            {
                auto value = orig;
                for (auto j: aoc::range{0, 5})
                {
                    large[r + roff][c + coff] = value;
                    value = (value == 9) ? 1 : value + 1;
                    coff += COLS;
                }

                coff  = 0;
                roff += ROWS;

                orig = (orig == 9) ? 1 : orig + 1;
            }
        }
    }

    // This is S--L--O--W (21s) - must be doing something wrong.
    return dijkstra(large);
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
    aoc::check_result(p1, 462U);

    auto p2 = part2(input);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 2846U);
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

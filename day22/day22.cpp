#include "utils.h"


struct Cuboid
{
    bool on{};
    int64_t x1; int64_t x2;
    int64_t y1; int64_t y2;
    int64_t z1; int64_t z2;
    int64_t sign{1};
};


int64_t volume(const Cuboid& t)
{
    return t.sign * (t.x2-t.x1+1) * (t.y2-t.y1+1) * (t.z2-t.z1+1);
}


Cuboid intersect(const Cuboid& t, const Cuboid& u)
{
    auto x1 = max(t.x1, u.x1);
    auto x2 = max(min(t.x2, u.x2), x1-1);

    auto y1 = max(t.y1, u.y1);
    auto y2 = max(min(t.y2, u.y2), y1-1);

    auto z1 = max(t.z1, u.z1);
    auto z2 = max(min(t.z2, u.z2), z1-1);

    auto sign  = -1 * t.sign * u.sign;

    return Cuboid{true, x1, x2, y1, y2, z1, z2, sign};
}


int64_t part2(const vector<Cuboid>& input, bool init)
{
    aoc::timer timer;

    vector<Cuboid> rules{};

    size_t x = 0;
    do 
    {
        auto t = input[x++];
       
        if (init)
        {
            if (t.x1 < -50) continue;
            if (t.y1 < -50) continue;
            if (t.z1 < -50) continue;

            if (t.x2 > 50) continue;
            if (t.y2 > 50) continue;
            if (t.z2 > 50) continue;
        }

        vector<Cuboid> rules2;
        if (t.on) 
        {
            rules2 = rules;
            rules2.push_back(t);

            for (auto u: rules)
            {
                auto i = intersect(u, t);
                if (volume(i) != 0)
                {
                    rules2.push_back(i);
                }
            }
        }
        else
        {
            for (auto u: rules)
            {
                constexpr int CUBEMAX = 10'000'000; 
                constexpr int CUBEMIN = -CUBEMAX; 
                auto i1 = intersect(u, Cuboid{true, t.x2+1,  CUBEMAX, CUBEMIN, CUBEMAX, CUBEMIN, CUBEMAX, -1});
                if (volume(i1) != 0) rules2.push_back(i1);
                auto i2 = intersect(u, Cuboid{true, CUBEMIN, t.x1-1,  CUBEMIN, CUBEMAX, CUBEMIN, CUBEMAX, -1});
                if (volume(i2) != 0) rules2.push_back(i2);
                auto i3 = intersect(u, Cuboid{true, t.x1,    t.x2,    t.y2+1,  CUBEMAX, CUBEMIN, CUBEMAX, -1});
                if (volume(i3) != 0) rules2.push_back(i3);
                auto i4 = intersect(u, Cuboid{true, t.x1,    t.x2,    CUBEMIN, t.y1-1,  CUBEMIN, CUBEMAX, -1});
                if (volume(i4) != 0) rules2.push_back(i4);
                auto i5 = intersect(u, Cuboid{true, t.x1,    t.x2,    t.y1,    t.y2,    t.z2+1,  CUBEMAX, -1});
                if (volume(i5) != 0) rules2.push_back(i5);
                auto i6 = intersect(u, Cuboid{true, t.x1,    t.x2,    t.y1,    t.y2,    CUBEMIN, t.z1-1,  -1});
                if (volume(i6) != 0) rules2.push_back(i6);
            }
        }

        rules = rules2;
    }
    while (x < input.size());

    int64_t sum = 0;
    for (auto u: rules)
        sum += volume(u);

    return sum;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines<string, int, int, int, int, int, int>(filename, R"((\w+) x=(-?\d+)..(-?\d+),y=(-?\d+)..(-?\d+),z=(-?\d+)..(-?\d+))");

    vector<Cuboid> input;
    for (auto [on, x1,x2, y1,y2, z1,z2] : lines)
    {
        Cuboid c;
        c.on = (on == "on");
        c.x1 = x1;
        c.x2 = x2;
        c.y1 = y1;
        c.y2 = y2;
        c.z1 = z1;
        c.z2 = z2;
        input.push_back(c);
    };

    auto p1 = part2(input, true);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 591365);

    auto p2 = part2(input, false);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 1211172281877240);
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

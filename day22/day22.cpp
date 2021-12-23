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

    int64_t sum = 0;
    vector<Cuboid> cuboids{};
    for (const auto& t: input) 
    {
        // Warm up.
        if (init)
        {
            if ((t.x1 < -50) || (t.x2 > 50)) continue;
            if ((t.y1 < -50) || (t.y2 > 50)) continue;
            if ((t.z1 < -50) || (t.z2 > 50)) continue;
        }

        // Just maintain a collection of cuboids, of increasing numbers of intersected initial cuboids, with sign 
        // sign changing on each new intersection. The logic is that the volume of:
        // - A and B combined is vol(A) + vol(B) - vol(A|B). 
        // - A, B and C combined is vol(A) + vol(B) - vol(A|B) + vol(C) - vol(A|C) - vol(B|C) + vol(A|B|C). 
        // - ...
        for (auto j: aoc::range{cuboids.size()})
        {
            const auto& u = cuboids[j];
            auto i = intersect(u, t);
            auto v = volume(i);
            if (v != 0)
            {
                sum += v;
                cuboids.push_back(i);
            }
        }

        // The negative volumes in the input really threw me for a long time, but many doodles indicated that 
        // any intersection which started from a negative volume made no contribution to the total volume. Taking
        // out the whole chain of intersection is just a case of not placing the original negative volume in the 
        // collection.
        if (t.on)
        {
            sum += volume(t);
            cuboids.push_back(t);
        } 
    }

    return sum;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines<string, int, int, int, int, int, int>(filename, R"((\w+) x=(-?\d+)..(-?\d+),y=(-?\d+)..(-?\d+),z=(-?\d+)..(-?\d+))");

    // Created Cuboid only to add the sign value. Pity.
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

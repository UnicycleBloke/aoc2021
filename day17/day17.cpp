#include "utils.h"


struct State
{
    int x{};
    int y{};
    int dx{};
    int dy{};

    int xmin{};
    int ymin{};
    int xmax{};
    int ymax{};

    int ypeak{};

    void step()
    {
        x += dx;
        y += dy;

        dx -= aoc::sgn(dx);
        dy -= 1;

        ypeak = max(ypeak, y);
    }

    bool done() const
    {
        return ((x >= xmin) && (x <= xmax) && (y >= ymin) && (y <= ymax));
    }

    bool missed() const
    {
        if (y < ymin) return true;
        if ((dx == 0) && ((x < xmin) || (x > xmax))) return true;
        return false;
    }
};




auto part2(int xmin, int xmax, int ymin, int ymax)
{
    constexpr int DYMIN = -500;
    constexpr int DYMAX = 500;

    aoc::timer timer;

    int count = 0;
    int peak  = ymin;

    for (int dx: aoc::range{1, xmax + 1}) // One step would overshoot anyway
    {
        for (int dy: aoc::range{DYMIN, DYMAX})
        {
            State state;
            state.dx    = dx;
            state.dy    = dy;

            state.xmin  = xmin;
            state.xmax  = xmax;
            state.ymin  = ymin;
            state.ymax  = ymax;
            state.ypeak = -10000;

            while (!state.done() && !state.missed())
                state.step();

            if (state.done())
            {
                peak = max(peak, state.ypeak);
                ++count;
            }
        }
    }

    return pair{peak, count};
}


void run(const char* filename)
{
    auto input = aoc::read_lines<int, int, int, int>(filename, R"(target area: x=(-?\d+)..(-?\d+), y=(-?\d+)..(-?\d+))");
    auto [xmin, xmax, ymin, ymax] = input[0];

    auto ans = part2(xmin, xmax, ymin, ymax);
    cout << "Part1: " << ans.first << '\n';
    aoc::check_result(ans.first, 6555);
    cout << "Part2: " << ans.second << '\n';
    aoc::check_result(ans.second, 4973);
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

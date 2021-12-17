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


int part1(int xmin, int xmax, int ymin, int ymax)
{
    aoc::timer timer;

    int ypeak = -10000;

    for (int dx: aoc::range{-1000, 1000})
    {
        for (int dy: aoc::range{-1000, 1000})
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
                ypeak = max(ypeak, state.ypeak);
                // cout << "(" << dx << "," << dy << ") -> " << state.ypeak << "\n";
            }
        }

    }

    return ypeak;
}


int part2(int xmin, int xmax, int ymin, int ymax)
{
    aoc::timer timer;

    int count = 0;

    for (int dx: aoc::range{-1000, 1000})
    {
        for (int dy: aoc::range{-1000, 1000})
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
                ++count;
                // cout << "(" << dx << "," << dy << ") -> " << state.ypeak << "\n";
            }
        }

    }

    return count;
}


void run(const char* filename)
{
    aoc::timer timer;

    auto input = aoc::read_lines<int, int, int, int>(filename, R"(target area: x=(-?\d+)..(-?\d+), y=(-?\d+)..(-?\d+))");
    auto [xmin, xmax, ymin, ymax] = input[0];
    cout << xmin << " " <<  xmax << " " <<  ymin << " " <<  ymax << '\n';

    auto p1 = part1(xmin, xmax, ymin, ymax);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 6555);

    auto p2 = part2(xmin, xmax, ymin, ymax);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 4973);
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

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
    aoc::timer timer;

    int count = 0;
    int peak  = ymin;
   
    // A speed of N peters out at x = N(N+1)/2, so dxmin is ~(sqrt(2*xmin)-1).
    // A speed of xmax + 1 overshoots immediately.
    for (int dx: aoc::range{int(sqrt(2*xmin)-1), xmax + 1}) 
    {
        // A speed of more than yrange would overshoot the target whether it is 
        // above or below the origin. Going down would overshoot on the first step.
        // Going up would overshoot when it returns to the ground - the y speed would 
        // be the same as for the first step.
        int yrange = max(abs(ymin), abs(ymax)) + 1;    
        for (int dy: aoc::range{-yrange, yrange})
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
    aoc::check_result(ans.first, 3003);
    cout << "Part2: " << ans.second << '\n';
    aoc::check_result(ans.second, 940);
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

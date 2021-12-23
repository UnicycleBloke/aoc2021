#include "utils.h"


constexpr int units[] = {1, 10, 100, 1000};
constexpr int cols[]  = {2, 4, 6, 8};
constexpr int slots[] = {0, 1, 3, 5, 7, 9, 10};
constexpr int IMPOSSIBLE = 1'000'000'000;


struct Burrow
{
    array<array<char, 11>, 5> cells{};
    int cost{};
    int home{};
};


int at_home(const Burrow& b)
{
    int result = 0;
    for (auto c: aoc::range(4))
    {
        char type = 'A' + c;
        int  col  = cols[c];

        if (b.cells[4][col] == type)
        {
            ++result;
            if (b.cells[3][col] == type)
            {
                ++result;
                if (b.cells[2][col] == type)
                {
                    ++result;
                    if (b.cells[1][col] == type)
                        ++result;
                }
            }
        }
    }

    return result;
}


// Check that it is permitted to enter this room.
bool can_enter(const Burrow& b, char type)
{
    int col = cols[type -'A'];
    if (b.cells[1][col] && (b.cells[1][col] != type)) return false;
    if (b.cells[2][col] && (b.cells[2][col] != type)) return false;
    if (b.cells[3][col] && (b.cells[3][col] != type)) return false;
    if (b.cells[4][col] && (b.cells[4][col] != type)) return false;
    return true;
}


bool can_exit(const Burrow& b, char type)
{
    return !can_enter(b, type);
}


int enter(Burrow& b, int cfrom, char type)
{
    int result = 0; 
    int unit   = units[type - 'A'];
    int cto    = cols[type -'A'];
    int dir    = aoc::sgn(cto - cfrom);

    // Moved along the shared row.
    int c = cfrom;
    while (c != cto)
    {
        if (b.cells[0][c+dir] != 0) return IMPOSSIBLE;
        c      += dir;
        result += unit;
    }

    // Move down into the room.
    int r;
    for (r = 0; r < 4; ++r)
    {
        if (b.cells[r+1][cto] != 0) 
            break;
        result += unit;
    }

    // Actually do the move.
    b.cells[r][cto]  = type;    
    b.cells[0][cfrom]  = 0;  
    b.cost            += result; 
    ++b.home;            

    return result;
}


int exit(Burrow& b, int cfrom, int cto)
{
    int row = 1;
    while (b.cells[row][cfrom] == 0) ++row;
    char type = b.cells[row][cfrom];

    int unit   = units[type - 'A'];
    int dir    = aoc::sgn(cto - cfrom);
    int result = unit * row; 

    // Moved along the shared row.
    int c = cfrom;
    while (c != cto)
    {
        if (b.cells[0][c+dir] != 0) return IMPOSSIBLE;
        c      += dir;
        result += unit;
    }

    // Actually do the move.
    b.cells[0][cto]      = type;    
    b.cells[row][cfrom]  = 0;  
    b.cost              += result; 

    return result;
}


void print(Burrow& b)
{
    for (auto r: aoc::range(b.cells.size()))
    {
        for (auto c: b.cells[r])
            cout << ((c != 0) ? c : '.');
        cout << '\n';
    }
    cout << b.home << " " << b.cost << '\n';
    cout << '\n';
}


int64_t counter = 0;
void part2(Burrow b, int& min_cost)
{
    //print(b);
    
    // Have we finished? 
    if (b.home == 16)
    {       
        min_cost = min(b.cost, min_cost);
     
        ++counter;
        if ((counter % 100000) == 0)
            cout << "min_cost " << min_cost << " " << counter << '\n';
        return;
    }
    if (b.cost >= min_cost) return;

    // Can we move anything into its home slot?
    for (int col: slots)
    {
        char type = b.cells[0][col];
        if ((type != 0) && can_enter(b, type))
        {
            // Find cost
            // Adjust and go again
            auto b2 = b;
            if (enter(b2, col, type) != IMPOSSIBLE)
                part2(b2, min_cost);
        }
    }

    // Can we move anything out of its initial slot?
    for (int cfrom: cols)
    {
        char type = 'A' + (cfrom - 2) / 2;
        if (can_exit(b, type))
        {
            for (int cto: slots)
            {
                auto b2 = b;
                if (exit(b2, cfrom, cto) != IMPOSSIBLE)
                    part2(b2, min_cost);
            }
        }
    }
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename);

    Burrow b;
    for (auto i: aoc::range(2U, 6U))
    {
        auto line = lines[i];
        b.cells[i-1][2] = line[3];
        b.cells[i-1][4] = line[5];
        b.cells[i-1][6] = line[7];
        b.cells[i-1][8] = line[9];
    }
    b.home = at_home(b);

    aoc::timer timer;
    
    int min_cost = IMPOSSIBLE;
    part2(b, min_cost);
    cout << "final min_cost " << min_cost << '\n';
    // cout << "Part2: " << p1 << '\n';
    //aoc::check_result(p1, 3410);

    // auto p2 = part2(b);
    // cout << "Part2: " << p2 << '\n';
    //aoc::check_result(p2, 98796);
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

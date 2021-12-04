#include "utils.h"


struct Board
{
    array<array<int, 5>, 5>  cells{};
    array<array<bool, 5>, 5> ticks{};

    void set_row(int r, string data)
    {
        istringstream is{data};
        for (int c = 0; c < 5; ++c)
        {
            int val;
            is >> val;
            cells[r][c] = val;
        }
    }

    bool mark_val(int val)
    {
        for (int r = 0; r < 5; ++r)
        {
            for (int c = 0; c < 5; ++c)
            {
                if (cells[r][c] == val)
                    ticks[r][c] = true;
            }
        }
        return wins();
    }

    bool row_wins(int r) const
    {
        bool res = true;
        for (int c = 0; c < 5; ++c)
            res &= ticks[r][c];
        return res;
    }

    bool col_wins(int c) const
    {
        bool res = true;
        for (int r = 0; r < 5; ++r)
            res &= ticks[r][c];
        return res;
    }

    bool wins() const
    {
        bool res = false;
        for (int r = 0; r < 5; ++r)
            res |= row_wins(r);
        for (int c = 0; c < 5; ++c)
            res |= col_wins(c);
        return res;
    }

    int result(int call) const
    {
        int total = 0;
        for (int r = 0; r < 5; ++r)
            for (int c = 0; c < 5; ++c)
                if (!ticks[r][c])
                    total += cells[r][c];
        return total * call;            
    }
};


int part1(vector<int> calls, vector<Board> boards)
{
    for (int call: calls)
    {
        for (auto& board: boards)
        {         
            board.mark_val(call);
            if (board.wins())
                return board.result(call);
        }
    }

    return 0;
}


int part2(vector<int> calls, vector<Board> boards)
{
    int count = 0;

    for (int call: calls)
    {
        for (auto& board: boards)
        {         
            if (board.wins())
                continue;

            board.mark_val(call);
            if (board.wins())
                ++count;

            if (count == boards.size())
                return board.result(call);
        }
    }

    return 0;
}


void run(const char* filename)
{
    auto input = aoc::read_lines(filename, false);

    vector<int> calls;
    auto temp  = aoc::split(input[0], ",", false);
    for (auto str: temp)
    {
        calls.push_back(stoi(str));
    }

    vector<Board> boards;
    int i = 1;
    while (i < input.size())
    {
        Board board;
        board.set_row(0, input[i++]);
        board.set_row(1, input[i++]);
        board.set_row(2, input[i++]);
        board.set_row(3, input[i++]);
        board.set_row(4, input[i++]);
        boards.push_back(board);
    }

    auto p1 = part1(calls, boards);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 28082);

    auto p2 = part2(calls, boards);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 8224);
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

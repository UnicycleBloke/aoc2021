#include "utils.h"

enum class Op { inp, add, mul, div, mod, eql };

map<string, Op> ops =
{
    { "inp", Op::inp},
    { "add", Op::add},
    { "mul", Op::mul},
    { "div", Op::div},
    { "mod", Op::mod},
    { "eql", Op::eql}
};


int64_t read_input()
{
    return 10;
}


template <typename T>
bool execute(T input)
{
    int64_t regs[4]{};

    for (auto i: input)
    {
        auto instr = aoc::split(i, " ", false);
 
        Op      op   = ops[instr[0]];
        int     reg1 = instr[1][0] - 'w';
        int     reg2 = -1;
        int64_t imm  = 0;
        if (instr.size() > 2)
        {
            if (isdigit(instr[2][0]) || instr[2][0] == '-')
                imm = stoi(instr[2]);
            else
                reg2 = instr[2][0] - 'w';
        }

        int64_t val2 = (reg2 == -1) ? imm : regs[reg2]; 

        switch (op)
        {
            case Op::inp:
                regs[reg1] = read_input();
                break;

            case Op::add:
                regs[reg1] += val2; 
                break;
                
            case Op::mul:
                regs[reg1] *= val2; 
                break;

            case Op::div:
            {
                if (val2 == 0) return false;

                auto sign = aoc::sgn(regs[reg1]); 
                val2 = (abs(regs[reg1]) / val2) * sign;
                cout << val2 << '\n';
                regs[reg1] = val2; 
                break;
            }

            case Op::mod:
                if (regs[reg1] < 0) return false;
                if (val2 <= 0) return false;

                regs[reg1] = regs[reg1] % val2; 
                break;

            case Op::eql:       
                regs[reg1] = (regs[reg1] == val2);
                break; 
        }

        for (auto r: aoc::range(4))
            cout << char('w'+r) << "="<< regs[r] << " ";
        cout << "\n"; 
    }

    cout << "complete" << '\n';
    return true;
}


template <typename T>
auto part1(T input)
{
    aoc::timer timer;
    execute(input);
    return 0;
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

    auto input = aoc::read_lines(filename);

    auto p1 = part1(input);
    cout << "Part1: " << p1 << '\n';
    //aoc::check_result(p1, 0);

    auto p2 = part2(input);
    cout << "Part2: " << p2 << '\n';
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

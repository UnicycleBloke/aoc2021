#include "utils.h"


struct Packet
{
    uint16_t version{};
    uint16_t type{};        // Literal or operator
    uint16_t id{};          // Length of operator data
    uint16_t len{};         // Meaning depends on id
    uint64_t literal{};     // For literals
    vector<Packet> operands{};  // For operators

    uint32_t total_ver() const
    {
        return version + accumulate(operands.begin(), operands.end(), 0U, [](auto a, auto b) { return a + b.total_ver(); });
    }

    uint64_t value() const
    {
        switch (type)
        {
            // This is cleaner and shorter than the original code, using the algorithms, but likely wastes time re-evaluating sub expressions.
            // Note that initial value must be uint64 or the return values will lose high bits.
            case 0: return accumulate(operands.begin(), operands.end(), 0ULL, [](auto a, auto b) { return a + b.value(); });
            case 1: return accumulate(operands.begin(), operands.end(), 1ULL, [](auto a, auto b) { return a * b.value(); });
            case 2: return min_element(operands.begin(), operands.end(), [](auto a, auto b) { return a.value() < b.value(); })->value();
            case 3: return max_element(operands.begin(), operands.end(), [](auto a, auto b) { return a.value() < b.value(); })->value();
            case 5: return operands[0].value() > operands[1].value();
            case 6: return operands[0].value() < operands[1].value();
            case 7: return operands[0].value() == operands[1].value();
            case 4: return literal;
        }
        return 0U;
    }

    void print(int indent = 0) const
    {
        for (auto i: aoc::range(indent * 4)) cout << ' ';

        switch (type)
        {
            // This is cleaner and shorter than the original code, using the algorithms, but likely wastes time re-evaluating sub expressions.
            // Note that initial value must be uint64 or the return values will lose high bits.
            case 0: cout << "sum "; break;
            case 1: cout << "mul "; break;
            case 2: cout << "min "; break;
            case 3: cout << "max "; break;
            case 5: cout << "gt  "; break;
            case 6: cout << "lt  "; break;
            case 7: cout << "eq  "; break;
            case 4: cout << "lit ";
        }
        cout << value() << "\n"; 
        for (auto op: operands) 
            op.print(indent + 1); 
    }
};


struct Input
{
    vector<int> data;
    size_t      pos;

    uint32_t take(uint8_t num)
    {
        uint32_t res = 0;
        for (auto i: aoc::range(num))
            res = (res << 1) | data[pos++];
        return res;
    }

    bool good() const
    {
        return (data.size() - pos) > 6;
    }

    void pad_tail()
    {
        while ((pos % 4) != 0) ++pos;
    }
};


Packet read_packet(Input& input, bool pad_tail)
{
    Packet packet{};

    packet.version = input.take(3);
    packet.type    = input.take(3);

    if (packet.type == 0b100)
    {
        uint8_t  temp  = input.take(5);
        uint64_t value = temp & 0b1111;
        while (temp & 0b10000) // Not the last
        {
            temp  = input.take(5);
            value = (value << 4) | (temp & 0b1111);
        }
        packet.literal = value;
    }
    else // Operator
    {
        packet.id = input.take(1);

        if (packet.id == 0)
        {
            // Number of bits
            packet.len = input.take(15);
            auto pos = input.pos + packet.len;
            while (input.pos < pos)
            {
                auto operand = read_packet(input, false);
                packet.operands.push_back(operand);
            }
        }
        else
        {
            // Number of packets
            packet.len = input.take(11);
            for (auto i: aoc::range(packet.len))
            {
                auto operand = read_packet(input, false);
                packet.operands.push_back(operand);
            }
        }
    }

    // This was a distraction - sub-packets not extended.
    if (pad_tail)
        input.pad_tail();

    return packet;
}


uint8_t from_hex(char c)
{
    if ((c >= '0') && (c <= '9')) return c - '0';
    if ((c >= 'A') && (c <= 'F')) return c - 'A' + 10;
    if ((c >= 'a') && (c <= 'f')) return c - 'a' + 10;
    return 0;
}


// I parsed the expression in an AST and then used that to calculated the answers. 
// Fun but a waste of time. All I really needed to do for Part 1 was this, which 
// skips everything to add the versions on the fly.
uint64_t part1(Input& input)
{
    uint64_t result = input.take(3);
    if (input.take(3) != 4)
    {
        if (input.take(1) == 0)
        {
            auto pos = input.pos + input.take(15);
            while (input.pos < pos)
                result += part1(input);
        }
        else
        {
            for (auto i: aoc::range(input.take(11)))
                result += part1(input);
        }
    }
    else
    {
        while ((input.take(5) & 0b10000) > 0);
    }

    return result;
}


// As above, no AST was required. Just create vector of subvalues on which to 
// use the operator. NOTE: This works perfectly for Visual Studio but not GCC
// under Ubuntu. There is a ton of diagnostic rubbish here, which shows that the 
// GCC version has several places in which the operations are out of order. For 
// example it will try to find the minimum value of a subexpression before reading
// the literal value which goes in the subexpression. So it operates on an empty 
// vector (boom) or one without all the expected values in it. It is deterministic, 
// but I have not been able to work out what's going on.
uint64_t part2(Input& input)
{
    uint16_t ver  = input.take(3);
    uint16_t type = input.take(3);
    cout << "....................... type " << type << endl; 

    if (type != 4)
    {
        vector<uint64_t> subs;
        if (input.take(1) == 0)
        {
            auto pos = input.pos + input.take(15);
            while (input.pos < pos)
            {
                auto temp = part2(input);
                subs.push_back(temp);
            }
            cout << "------------> subs0 " << type << ' ' << subs.size() << endl;
        }
        else  
        {
            for (auto i: aoc::range(input.take(11)))
            {
                auto temp = part2(input);
                subs.push_back(temp);
            }
             cout << "------------> subs1 " << type << ' ' << subs.size() << endl;

        }

        if (subs.size() == 0) 
        {
            subs.push_back(3);
            cout << "....................... push " << type << endl; 
        } 

        uint64_t value{};
        switch (type)
        {
            case 0: value = accumulate(subs.begin(), subs.end(), 0ULL); break;
            case 1: value = accumulate(subs.begin(), subs.end(), 1ULL, [](auto a, auto b){ return a * b;}); break;
            case 2: value = *min_element(subs.begin(), subs.end()); break;
            case 3: value = *max_element(subs.begin(), subs.end()); break;
            case 5: value = subs[0] > subs[1]; break;
            case 6: value = subs[0] < subs[1]; break;
            case 7: value = subs[0] == subs[1]; break;
        }

        cout << "op " << type << " = " << value << '\n';
        return value;
    }
    else
    {
        uint8_t  temp  = input.take(5);
        uint64_t value = temp & 0b1111;
        while (temp & 0b10000) // Not the last
        {
            temp  = input.take(5);
            value = (value << 4) | (temp & 0b1111);
        }

        cout << "lit " << value << '\n';
        return value;
    }

    return 0U;
}


void run(const char* filename)
{
    aoc::timer timer;

    auto lines = aoc::read_lines(filename);
    auto bytes = aoc::comprehend(lines[0], [](auto c) -> uint8_t { return from_hex(c); });  

    vector<int> bits;
    for (auto i: bytes)
    {
        bits.push_back((i & 8) > 0); 
        bits.push_back((i & 4) > 0); 
        bits.push_back((i & 2) > 0); 
        bits.push_back((i & 1) > 0); 
    }

    Input input{bits, 0U};
    Packet toplevel = read_packet(input, true);
    toplevel.print();

    Input input1{bits, 0U};
    //auto p1 = toplevel.total_ver();
    auto p1 = part1(input1);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 889U);

    Input input2{bits, 0U};
    //auto p2 = toplevel.value();
    auto p2 = part2(input2);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 739303923668U);
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

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

    auto p1 = toplevel.total_ver();
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 889U);

    auto p2 = toplevel.value();
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

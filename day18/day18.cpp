#include "utils.h"


struct Number
{
    int value{};
    vector<Number> subs{};
    static int index; 
    
    bool isdigit() const
    {
        return subs.size() == 0;  
    }

    void print(ostream& os, bool end)
    {
        if (isdigit()) 
        {
            os << value;
        }
        else
        {
            os << '[';        
            subs[0].print(os, false);
            os << ',';
            subs[1].print(os, false);
            os << ']';        
        }

        if (end) os << '\n';
    }

    void graph(ostream& os, int& id)
    {
        static const vector<string> colours =
        {
            "red", "blue", "green", "cyan", "yellow", "magenta", 
            "coral", "aquamarine", "blueviolet", "chartreuse"
        };

        int node_id = ++id;
        if (isdigit()) 
        {
            os << "Node_" << node_id << " [style=filled, label=\"" << value << "\"";
            //os << ", color=" << colours[value];
            //os << ", fontcolor=" << colours[value];
            os << ", fillcolor=" << colours[value % 10];
            os << "]" << '\n';

        }
        else
        {
            os << "Node_" << node_id << " [label=\"" << magnitude() << "\"";
            os << ", color=" << colours[magnitude() % 10];
            //os << ", fontcolor=" << colours[value];
            //os << ", fillcolor=" << colours[value];
            os << "]" << '\n';

            os << "Node_" << node_id << " -> " << "Node_" << (id +1) << '\n';
            subs[0].graph(os, id);

            os << "Node_" << node_id << " -> " << "Node_" << (id +1) << '\n';
            subs[1].graph(os, id);
        }
    }

    void graph() 
    {
        char filename[32];
        snprintf(filename, 32, "dot/day18-%06d.dot", index++);
        ofstream os{filename};
        os << "digraph Day18 {\n";
        
        int id = 0;
        graph(os, id);

        os << "}\n";
    }

    void print()
    {
        print(cout, true);
    }

    string repr()
    {
        ostringstream os;
        print(os, false);
        return os.str();
    }

    int magnitude()
    {
        int result = 0;
        if (isdigit()) 
        {
            result += value;
        }
        else
        {
            result += subs[0].magnitude() * 3;
            result += subs[1].magnitude() * 2;
        }
        return result;
    }
};


int Number::index = 0; 


struct Splitter
{
    bool split(Number& number)
    {
        if (was_split) return was_split;

        if (!number.isdigit()) 
        {
            split(number.subs[0]);
            split(number.subs[1]);
        }
        else
        {
            if (number.value >= 10)
            {
                number.subs.push_back({number.value /2});
                number.subs.push_back({(number.value + 1) /2});
                number.value = 0;
                was_split = true;
            }
        }

        return was_split;
    }

    bool was_split{};
};


// Working out how to do this step was by far the hardest part of the problem. Some 
// three hours of head-scratching, hacking and debugging.
struct Exploder
{
    void explode(Number& number, int level)
    {
        if (!number.isdigit() && (level == 4) && !exploded)
        {
            left_value  = number.subs[0].value;
            right_value = number.subs[1].value;

            number.value = 0;
            number.subs.clear();

            exploded = true;
            return;
        }

        if (!number.isdigit()) 
        {
            explode(number.subs[0], level + 1);
            explode(number.subs[1], level + 1);
        }
        else
        {
            if (exploded)
            {
                if (!right_num) right_num = &number;
            }
            else
            {
                left_num = &number;
            }
        }
    }

    bool explode(Number& number)
    {
        explode(number, 0);
        if (left_num)  left_num->value  += left_value;
        if (right_num) right_num->value += right_value;
        return exploded;
    }

    Number* left_num{};
    Number* right_num{};
    int     left_value{};
    int     right_value{};
    bool    exploded{};
};


Number parse(const string& line, int& pos)
{
    Number res;

    char c = line[pos++];
    if (c == '[')
    {
        res.subs.push_back(parse(line, pos));
        ++pos; // Comma     
        res.subs.push_back(parse(line, pos));
        ++pos; // ]    
    }
    else
    {
        res.value = c - '0';
    }

    return res;
}


Number parse(const string& line)
{
    int pos = 0;
    return parse(line, pos);
}


void reduce(Number& number)
{
    number.graph();

    bool stop = false;
    while (!stop)
    {
        stop = true;

        Exploder e;
        if (e.explode(number))
        {
            stop = false;
            number.graph();
            continue;
        }

        Splitter s;
        if (s.split(number))
        {
            stop = false;
            number.graph();
            continue;
        }
    }
}


Number add(const Number& left, const Number& right)
{
    Number res{0};
    res.subs.push_back(left);
    res.subs.push_back(right);
    reduce(res);
    return res;
}


template <typename T>
auto part1(T input)
{ 
    aoc::timer timer;

    Number left = parse(input[0]);
    for (auto i: aoc::range(1U, input.size()))
    {
        Number right = parse(input[i]);
        left = add(left, right);
    }

    return left.magnitude();
}


template <typename T>
auto part2(T input)
{
    aoc::timer timer;

    int mag = 0;

    vector<Number> numbers;
    for (auto s: input)
        numbers.push_back(parse(s));

    for (auto i: aoc::range(numbers.size()))
    {
        for (auto j: aoc::range(i + 1, numbers.size()))
        {
            mag = max(mag, add(numbers[i], numbers[j]).magnitude());
            mag = max(mag, add(numbers[j], numbers[i]).magnitude());
        }
    }

    return mag;
}


void test_magnitde(string str, int exp)
{
    auto num = parse(str);
    aoc::check_result(num.magnitude(), exp);
}


void test_explode(string str, string exp)
{
    auto num = parse(str);
    Exploder e;
    e.explode(num);
    if (num.repr() != exp)
        std::cout << "ERROR: expected = " << exp << " but got = " << num.repr() << '\n';
}


void run(const char* filename)
{
    auto input = aoc::read_lines(filename);

    //test_magnitde("[[1,2],[[3,4],5]]", 143);
    //test_magnitde("[[[[0,7],4],[[7,8],[6,0]]],[8,1]]", 1384);
    //test_magnitde("[[[[1,1],[2,2]],[3,3]],[4,4]]", 445);
    //test_magnitde("[[[[3,0],[5,3]],[4,4]],[5,5]]", 791);
    //test_magnitde("[[[[5,0],[7,4]],[5,5]],[6,6]]", 1137);
    //test_magnitde("[[[[8,7],[7,7]],[[8,6],[7,7]]],[[[0,7],[6,6]],[8,7]]]", 3488);

    //test_explode("[[[[[9,8],1],2],3],4]", "[[[[0,9],2],3],4]");
    //test_explode("[7,[6,[5,[4,[3,2]]]]]" ,"[7,[6,[5,[7,0]]]]");
    //test_explode("[[6,[5,[4,[3,2]]]],1]" ,"[[6,[5,[7,0]]],3]");
    //test_explode("[[3,[2,[1,[7,3]]]],[6,[5,[4,[3,2]]]]]" ,"[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]");
    //test_explode("[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]" ,"[[3,[2,[8,0]]],[9,[5,[7,0]]]]");

    //auto left  = parse("[[[[4,3],4],4],[7,[[8,4],9]]]");
    //auto right = parse("[1,1]");
    //auto sum = add(left, right);
    //string exp{"[[[[0,7],4],[[7,8],[6,0]]],[8,1]]"};
    //if (sum.repr() != exp)
    //    std::cout << "ERROR: expected = " << exp << " but got = " << sum.repr() << '\n';

    // vector<string> test1 = {
    //     "[[[0,[4,5]],[0,0]],[[[4,5],[2,6]],[9,5]]]",
    //     "[7,[[[3,7],[4,3]],[[6,3],[8,8]]]]",
    //     "[[2,[[0,8],[3,4]]],[[[6,7],1],[7,[1,6]]]]",
    //     "[[[[2,4],7],[6,[0,5]]],[[[6,8],[2,8]],[[2,1],[4,5]]]]",
    //     "[7,[5,[[3,8],[1,4]]]]",
    //     "[[2,[2,2]],[8,[8,1]]]",
    //     "[2,9]",
    //     "[1,[[[9,3],9],[[9,0],[0,7]]]]",
    //     "[[[5,[7,4]],7],1]",
    //     "[[[[4,2],2],6],[8,7]]" };
    // auto res1 = part1(test1);

    // vector<string> test2 = {
    //     "[[[0,[5,8]],[[1,7],[9,6]]],[[4,[1,2]],[[1,4],2]]]",
    //     "[[[5,[2,8]],4],[5,[[9,9],0]]]",
    //     "[6,[[[6,2],[5,6]],[[7,6],[4,7]]]]",
    //     "[[[6,[0,7]],[0,9]],[4,[9,[9,0]]]]",
    //     "[[[7,[6,4]],[3,[1,3]]],[[[5,5],1],9]]",
    //     "[[6,[[7,3],[3,2]]],[[[3,8],[5,7]],4]]",
    //     "[[[[5,4],[7,7]],8],[[8,3],8]]",
    //     "[[9,3],[[9,9],[6,[4,9]]]]",
    //     "[[2,[[7,7],7]],[[5,8],[[9,3],[0,2]]]]",
    //     "[[[[5,2],5],[8,[3,7]]],[[5,[7,5]],[4,4]]]" };
    // auto res2 = part1(test2);
    // aoc::check_result(res2, 4140);

    auto p1 = part1(input);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 3647);

    // auto p2 = part2(input);
    // cout << "Part2: " << p2 << '\n';
    // aoc::check_result(p2, 4600);
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

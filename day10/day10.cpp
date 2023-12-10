#include "utils.h"


// Original part 1
// void validate(const string& line, size_t& pos, size_t& err, char exp)
// {
//     while ((pos < line.size()) && (err == 0))
//     {
//         auto c = line[pos];
//         ++pos;

//         switch (c)
//         {
//             case ')': if (c != exp) { err = 3; }     return;
//             case ']': if (c != exp) { err = 57; }    return;
//             case '}': if (c != exp) { err = 1197; }  return;
//             case '>': if (c != exp) { err = 25137; } return;

//             case '(': validate(line, pos, err, ')'); break;
//             case '[': validate(line, pos, err, ']'); break;
//             case '{': validate(line, pos, err, '}'); break;
//             case '<': validate(line, pos, err, '>'); break;
//         }
//     }
// }


// Original part 2
// void complete(string& line, size_t& pos, size_t& err, char exp)
// {
//     while (pos < line.size())
//     {
//         auto c = line[pos];
//         ++pos;

//         switch (c)
//         {
//             case ')':
//             case ']':
//             case '}':
//             case '>': return;

//             case '(': complete(line, pos, err, ')'); break;
//             case '[': complete(line, pos, err, ']'); break;
//             case '{': complete(line, pos, err, '}'); break;
//             case '<': complete(line, pos, err, '>'); break;
//         }
//     }

//     switch (exp)
//     {
//         case ')': err = err * 5 + 1; break;
//         case ']': err = err * 5 + 2; break;
//         case '}': err = err * 5 + 3; break;
//         case '>': err = err * 5 + 4; break;
//     }
// }


// Alternative stack-based solution. Better than recursion.
size_t validate(const string& line, size_t& score)
{
    stack<char> s;
    for (char c: line)
    {
        switch (c)
        {
            case ')': if (s.top() != '(') { return 3; }     s.pop(); break;
            case ']': if (s.top() != '[') { return 57; }    s.pop(); break;
            case '}': if (s.top() != '{') { return 1197; }  s.pop(); break;
            case '>': if (s.top() != '<') { return 25137; } s.pop(); break;

            case '(':
            case '[':
            case '{':
            case '<': s.push(c); break;
        }
    }

    score = 0;
    while (s.size())
    {
        switch (s.top())
        {
            case '(': score = score * 5 + 1; break;
            case '[': score = score * 5 + 2; break;
            case '{': score = score * 5 + 3; break;
            case '<': score = score * 5 + 4; break;
        }
        s.pop();
    }

    return 0;
}


template <typename T>
auto part1(const T& input)
{
    size_t errors = 0;

    for (auto line: input)
    {
        size_t dummy;
        auto error = validate(line, dummy);
        errors += error;
    }

    return errors;
}


template <typename T>
auto part2(T& input)
{
    vector<size_t> scores;

    for (auto line: input)
    {
        size_t score;
        if (validate(line, score) == 0)
            scores.push_back(score);
    }

    sort(scores.begin(), scores.end());
    return scores[scores.size() / 2];
}


void run(const char* filename)
{
    auto input = aoc::read_lines(filename);

    auto p1 = part1(input);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 394647U);

    auto p2 = part2(input);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 2380061249U);
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

#include "utils.h"
#ifdef __unix__
#include "/usr/include/ncurses.h"
#endif


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
size_t validate2(const string& line, size_t& score)
{
    int pos = 0;
    vector<char> chars;

    stack<char> s;
    for (char c: line)
    {
        switch (c)
        {
            case ')': if (s.top() != '(') { return 3; }     s.pop(); chars.pop_back(); break;
            case ']': if (s.top() != '[') { return 57; }    s.pop(); chars.pop_back(); break;
            case '}': if (s.top() != '{') { return 1197; }  s.pop(); chars.pop_back(); break;
            case '>': if (s.top() != '<') { return 25137; } s.pop(); chars.pop_back(); break;

            case '(':
            case '[':
            case '{':
            case '<': s.push(c); chars.push_back(c); break;
        }

        for (auto i: aoc::range<int>(chars.size()))
        {
            int colour = 1;
            switch (chars[i])
            {
                case '(': colour = 1; break;
                case '[': colour = 2; break;
                case '{': colour = 3; break;
                case '<': colour = 4; break;
            }
            attron(COLOR_PAIR(colour));
            mvaddch(i, pos, chars[i]);
            attroff(COLOR_PAIR(colour));
        }
        refresh();
        //getch();
        ++pos;
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

        chars.pop_back();
        for (auto i: aoc::range<int>(chars.size()))
        {
            int colour = 1;
            switch (chars[i])
            {
                case '(': colour = 1; break;
                case '[': colour = 2; break;
                case '{': colour = 3; break;
                case '<': colour = 4; break;
            }
            attron(COLOR_PAIR(colour));
            mvaddch(i, pos, chars[i]);
            attroff(COLOR_PAIR(colour));
        }
        refresh();
        //getch();
        ++pos;
    }

    return 0;
}


template <typename T>
auto part1(const T& input)
{
    size_t errors = 0;

    for (auto line: input)
    {
        clear();

        char buffer[128];
        snprintf(buffer, 128, "Total: %u", errors);
        mvaddstr(22, 0, buffer);
        refresh();

        size_t dummy;
        auto error = validate2(line, dummy);
        errors += error;

        snprintf(buffer, 128, "Total: %u", errors);
        mvaddstr(22, 0, buffer);
        refresh();

        getch();
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
        if (validate2(line, score) == 0)
            scores.push_back(score);
    }

    sort(scores.begin(), scores.end());
    return scores[scores.size() / 2];
}


void run_impl(const char* filename)
{
    auto input = aoc::read_lines(filename);

    auto p1 = part1(input);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 299793U);

    auto p2 = part2(input);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 3654963618U);
}


void run(const char* filename)
{
#ifdef __unix__
    initscr();
    cbreak();
    noecho();
    start_color();
    init_pair(1, COLOR_RED,     COLOR_RED);
    init_pair(2, COLOR_BLUE,    COLOR_BLUE);
    init_pair(3, COLOR_GREEN,   COLOR_GREEN);
    init_pair(4, COLOR_MAGENTA, COLOR_MAGENTA);
#endif

    run_impl(filename);

#ifdef __unix__
    endwin();
#endif
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

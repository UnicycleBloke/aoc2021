#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <map>
#include <set>
#include <array>
#include <queue>
#include <stack>
#include <numeric>
#include <algorithm>
#include <limits>

#include <string>
#include <regex>
#include <filesystem>
#include <type_traits>
#include <chrono>
#include <tuple>

#include "conway.h"
#include "range.h"


using namespace std;
namespace fs = std::filesystem;
namespace cr = std::chrono;


namespace aoc {


void ltrim(std::string& s);
void rtrim(std::string& s);
void trim(std::string& s);

std::string replace(std::string source, const std::string& search, const std::string& replace);
std::vector<std::string> split(std::string source, std::string delim, bool allow_blanks);

std::vector<std::string> read_lines(std::istream& is, bool allow_blanks = false);
std::vector<std::string> read_lines(std::string filename, bool allow_blanks = false);

std::vector<std::string> read_groups(std::istream& is);
std::vector<std::string> read_groups(std::string filename);

// Convert a list of delimited values into an array. Assumes all values are of the same type.
// "1,2,3" => vector<int>{1, 2, 3}.
template <typename T>
std::vector<T> make_vector(std::string input, std::string delim = ",")
{
    trim(input);
    input = replace(input, delim, " ");

    std::vector<T> result;
    std::istringstream is{input};
    while (!is.eof())
    {
        T value;
        is >> value;
        result.emplace_back(value);
    }

    return result;
}

template <typename T>
std::vector<T> read_ints(std::istream& is)
{
    //static_assert(std::is_integral_v<T> && !std::is_same_v<T, bool> && sizeof(T) >= 4);
    static_assert(std::is_integral_v<T>);
    static_assert(!std::is_same_v<T, bool>);
    static_assert(sizeof(T) >= 4);

    std::vector<T> ints;

    while (!is.eof())
    {
        std::string line;
        std::getline(is, line);
        trim(line);

        std::istringstream is{line};
        T value;
        is >> value;
        ints.push_back(value);
    }

    return ints;
}


// Extract arguments from a formatted string into a tuple, via regex.
// .      not newline any character except line terminators (LF, CR, LS, PS).
// \t     tab (HT)
// \n     newline (LF)
// \v     vertical tab (VT)
// \f     form feed (FF)
// \r     carriage return (CR)
// \xhh	  ASCII character
// \uhhhh unicode character
// \d     digit
// \D     not digit
// \s     whitespace
// \S     not whitespace
// \w     word: an alphanumeric or underscore character (same as [_[:alnum:]]).
// \W     not word
template <class... Args>
std::tuple<Args...> parse_line(const std::regex& re, const std::string& str, bool& matched)
{
    // Capture values from a string using regex, and stuff these
    // into a new space-delimited string.
    std::smatch matches;
    std::stringstream ss{};

    matched = std::regex_match(str, matches, re);
    if (matched)
    {
        for (int i = 1; i < matches.size(); ++i)
        {
            const auto& match = matches[i];
            ss << match.str() << " ";
        }
    }

    // Read from the space-delimited string into a tuple.
    auto res = std::tuple<Args...>{};
    std::apply
    (
        [&ss](Args&... args)
        {
            ((ss >> args), ...);
        }, res
    );
    return res;
}


template <class... Args>
std::tuple<Args...> parse_line(const std::string& pat, const std::string& str)
{
    bool dummy;
    const std::regex re(pat);
    return parse_line<Args...>(re, str, dummy);
}


// Read all lines from a file and parse with regex to create array of tuples.
template <class... Args>
std::vector<std::tuple<Args...>> read_lines(std::istream& is, std::string pat)
{
    std::vector<std::tuple<Args...>> result;
    const std::regex re(pat);

    auto lines = read_lines(is, false);
    for (const auto& line: lines)
    {
        // Skip lines which don't match - useful for reading files with two or 
        // more lines formats. Call read_lines for each format.
        bool matched = false;
        auto temp    = parse_line<Args...>(re, line, matched);
        if (matched)
            result.push_back(temp);
    }

    return result;
}


template <class... Args>
std::vector<std::tuple<Args...>> read_lines(std::string filename, std::string pat)
{
    std::ifstream is{filename};
    return read_lines<Args...>(is, pat);
}


template <typename T, typename U>
void check_result(T value, U expected)
{
    static_assert(std::is_integral_v<T> && std::is_integral_v<U>);
    static_assert(sizeof(T) >= sizeof(U));
    static_assert(std::is_signed_v<T> == std::is_signed_v<U>);

    if (value != static_cast<T>(expected))
    {
        std::cout << "ERROR: expected = " << expected << " but got = " << value << '\n';
    }
}


// Used for a default predicate below
template <typename T>
struct AlwaysTrue
{
    bool operator()(T) const { return true; }
};


// Used for a default transform below
template <typename T>
struct NullTransform
{
    T operator()(T t) const { return t; }
};


// Simple equivalent to Python: [trans(x) for x in src if pred(x)].
//    std::string s = "a234234dfadfkjasdfkjadfjl;akdf";
//    // Effectively [x for x in s].
//    auto x = comprehend(s);  
//    // Effectively [trans(x) for x in s].
//    auto x = comprehend(s, [](auto c) -> std::size_t { return c; });  
//    // Effectively [trans(x) for x in s if pred(x)].
//    auto x = comprehend(s, [](auto c) -> std::size_t { return c; }, [](auto c) { return c % 10 == 7; });  
template <typename Src, 
    typename Trans = NullTransform<typename Src::value_type>, 
    typename Pred = AlwaysTrue<typename Src::value_type>>
auto comprehend(const Src src, 
    const Trans trans = NullTransform<typename Src::value_type>{}, 
    const Pred pred = AlwaysTrue<typename Src::value_type>{})
{
    std::vector<decltype(trans(src[0]))> dest;
    for (const auto& value: src)
    {
        if (pred(value))
            dest.emplace_back(trans(value));
    }
    return dest;
}


template <typename T>
int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}


class timer
{
public:
    timer()
    : m_start{cr::steady_clock::now()}
    {
    }
    
    ~timer()
    {
        auto end = cr::steady_clock::now();
        cr::duration<double> diff = end - m_start;
        std::cout << "Time elapsed: " << (diff.count() * 1000) << "ms\n";
    }

private:
    cr::time_point<cr::steady_clock> m_start{}; 
};


} // namespace aoc {



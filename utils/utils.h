#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <map>
#include <set>
#include <array>
#include <numeric>
#include <algorithm>

#include <string>
#include <regex>
#include <filesystem>
#include <type_traits>
#include <chrono>
#include <tuple>

#include "conway.h"


using namespace std;
namespace fs = std::filesystem;


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
std::tuple<Args...> parse_line(const std::regex& re, const std::string& str)
{
    // Capture values from a string using regex, and stuff these 
    // into a new space-delimited string.
    std::smatch matches;
    std::stringstream ss{};
    if (std::regex_match(str, matches, re))
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
    const std::regex re(pat);
    return parse_line<Args...>(re, str);
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
        result.push_back(parse_line<Args...>(re, line));
    }

    return result;
}


template <class... Args>
std::vector<std::tuple<Args...>> read_lines(std::string filename, std::string pat)
{
    std::ifstream is{filename};
    return read_lines<Args...>(is, pat);
}


template <typename T>
void check_result(T value, T expected)
{
    if (value != expected)
    {
        std::cout << "\nERROR: expected = " << expected << " but got = " << value << '\n';
    }
}


// Simple equivalent to Python: [trans(x) for x in src if pred(x)].
template <typename Src, typename Pred, typename Trans>
auto comprehend(const Src src, const Pred pred, const Trans trans)
{
    static_assert(is_same_v<Src, vector<typename Src::value_type>>);

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


} // namespace aoc {



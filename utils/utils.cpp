#include "utils.h"
#include <sstream>


namespace aoc {


void ltrim(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
        [](unsigned char ch) { return !std::isspace(ch); }));
}


void rtrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(),
        [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
}


void trim(std::string& s)
{
    ltrim(s);
    rtrim(s);
}


std::string replace(std::string source, const std::string& search, const std::string& replace)
{
    size_t pos = 0;
    while ((pos = source.find(search, pos)) != std::string::npos)
    {
        source.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return source;
}


std::vector<std::string> split(std::string source, std::string delim, bool allow_blanks)
{
    std::vector<std::string> result;

    size_t start = 0;
    size_t end = source.find(delim);
    while (end != std::string::npos)
    {
        auto sub = source.substr(start, end - start);
        trim(sub);
        if (allow_blanks || (sub.size() > 0))
            result.push_back(sub);

        start = end + delim.length();
        end   = source.find(delim, start);
    }

    auto sub = source.substr(start, end - start);
    trim(sub);
    if (allow_blanks || (sub.size() > 0))
        result.push_back(sub);

    return result;
}


std::vector<std::string> read_lines(std::istream& is, bool allow_blanks)
{
    std::vector<std::string> lines;

    while (!is.eof())
    {
        std::string line;
        std::getline(is, line);
        trim(line);
        if (allow_blanks || (line.size() > 0))
            lines.push_back(line);
    }

    return lines;
}


std::vector<std::string> read_lines(std::string filename, bool allow_blanks)
{
    std::ifstream is{filename};
    return read_lines(is, allow_blanks);
}


std::vector<std::string> read_groups(std::istream& is)
{
    std::ostringstream       ss;
    std::vector<std::string> groups;
    std::vector<std::string> lines = read_lines(is, true);

    for (const auto& line: lines)
    {
        if (line.size() > 0)
        {
            ss << line << ' ';
        }
        else
        {
            auto group = ss.str();
            if (group.size() > 0)
                groups.push_back(group);
            ss.str("");
            ss.clear();
        }        
    }

    auto group = ss.str();
    if (group.size() > 0)
        groups.push_back(group);

    return groups;
}


std::vector<std::string> read_groups(std::string filename)
{
    std::ifstream is{filename};
    return read_groups(is);
}


} // namespace aoc {

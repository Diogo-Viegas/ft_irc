#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>

struct IRCmsg
{
    std::string prefix;
    std::string command;
    std::vector<std::string> args;
};

class Parser
{
    public:
        static IRCmsg parseLine(const std::string &line);
        static std::string toUpper(const std::string &str);
};

#endif
#pragma once
#include <string>

class IParser {
public:
    virtual ~IParser() = default;
    virtual void parse_line(std::string line) = 0;
};


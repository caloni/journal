#pragma once
#include "iparser.h"

class InterceptingParser : public IParser {
public:
    void parse_line(std::string line);
};

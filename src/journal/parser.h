#pragma once
#include "iparser.h"
#include <string>

class Parser : public IParser {
    void parse_line(std::string line);
};

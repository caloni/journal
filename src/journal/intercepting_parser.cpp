#include "journal.h"

class InterceptingParser : public IParser {
public:
    void parse_line(std::string line);
};

void InterceptingParser::parse_line(std::string line) {
    ///@todo call interceptor and then parser
}


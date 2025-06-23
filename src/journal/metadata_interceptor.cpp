#include "journal.h"

class MetadataInterceptor : public IParserInterceptor {
public:
    void intercept(std::string& line);
};

void MetadataInterceptor::intercept(std::string& line) {
    ///@todo treat metadata before markdown parser
}


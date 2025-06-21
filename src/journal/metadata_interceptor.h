#pragma once
#include "iparser_interceptor.h"

class MetadataInterceptor : public IParserInterceptor {
public:
    void intercept(std::string& line);
};

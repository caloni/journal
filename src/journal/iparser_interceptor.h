#pragma once
#include <string>

class IParserInterceptor {
public:
    virtual ~IParserInterceptor() = default;
    virtual void intercept(std::string& line) = 0;
};


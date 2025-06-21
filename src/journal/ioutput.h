#pragma once

class IOutput {
public:
    virtual ~IOutput() = default;
    virtual int generate() = 0;
};

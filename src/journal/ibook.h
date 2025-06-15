#pragma once
#include <fstream>

namespace fs = std::filesystem;

class IBook {
public:
    virtual ~IBook() = default;
    virtual int create(fs::path basedir, fs::path scriptdir, bool includeprivate = true) = 0;
};

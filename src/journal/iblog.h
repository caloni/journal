#pragma once
#include <fstream>

namespace fs = std::filesystem;

class IBlog {
public:
    virtual ~IBlog() = default;
    virtual int create(fs::path basedir, fs::path scriptdir) = 0;
};

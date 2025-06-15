#pragma once
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

class Blog {
public:
    int create(fs::path basedir, fs::path scriptdir);
};


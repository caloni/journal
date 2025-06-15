#pragma once
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

class Book {
public:
    int create(fs::path basedir, fs::path scriptdir, bool includeprivate = true);
};


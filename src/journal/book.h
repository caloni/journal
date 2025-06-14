#pragma once
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int create_book(fs::path basedir, fs::path scriptdir);


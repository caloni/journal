#pragma once
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int create_blog(fs::path basedir, fs::path scriptdir);


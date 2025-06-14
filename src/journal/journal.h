#pragma once
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

void setup_encoding();
std::string run_command(const std::string& cmd);
std::string current_datetime();
void clear_directory(const fs::path& dir);

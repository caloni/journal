#pragma once
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace Util {
    void setup_encoding();
    std::string run_command(const std::string& cmd);
    std::string current_datetime();
    void clear_directory(const fs::path& dir);
    void create_backup(const fs::path& basedir);
    void run_script(const std::string& script_path);
    void git_commit_push(const fs::path& path, const std::string& message);
}

#pragma once
#include "ishell.h"
#include <fstream>
#include <string>

namespace fs = std::filesystem;

class Shell : public IShell {
public:
    void setup_encoding();
    std::string run_command(const std::string& cmd);
    std::string current_datetime();
    void clear_directory(const fs::path& dir);
    void create_backup(const fs::path& basedir);
    void run_script(const std::string& script_path);
    void git_commit_push(const fs::path& path, const std::string& message);
    void current_path(const fs::path& path);
    std::ostream& cout();
    std::ostream& cerr();
};

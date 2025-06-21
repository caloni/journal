#pragma once
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

class IShell {
public:
    virtual ~IShell() = default;
    virtual void setup_encoding() = 0;
    virtual std::string run_command(const std::string& cmd) = 0;
    virtual std::string current_datetime() = 0;
    virtual void clear_directory(const fs::path& dir) = 0;
    virtual void create_backup(const fs::path& basedir) = 0;
    virtual void run_script(const std::string& script_path) = 0;
    virtual void git_commit_push(const fs::path& path, const std::string& message) = 0;
    virtual void current_path(const fs::path& path) = 0;
    virtual std::ostream& cout() = 0;
    virtual std::ostream& cerr() = 0;
};


#pragma once
#include <fstream>

namespace fs = std::filesystem;

class IConfig {
public:
    virtual ~IConfig() = default;
    virtual bool parse(int argc, char* argv[]) = 0;
    virtual void print_usage() = 0;
    virtual bool is_blog() = 0;
    virtual bool is_book() = 0;
    virtual bool is_publish() = 0;
    virtual bool is_backup() = 0;
    virtual bool is_git_commit_push_private_repo() = 0;
    virtual fs::path get_basedir() = 0;
    virtual fs::path get_scriptdir() = 0;
    virtual fs::path get_private_dir() = 0;
    virtual fs::path get_private_repo() = 0;
};


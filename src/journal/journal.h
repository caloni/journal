#pragma once
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;

class IConfig {
public:
    virtual ~IConfig() = default;
    virtual bool parse(int argc, char* argv[]) = 0;
    virtual std::string usage() = 0;
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

class IGenerator {
public:
    virtual ~IGenerator() = default;
    virtual int generate() = 0;
};

class IParser {
public:
    virtual ~IParser() = default;
    virtual void parse_line(std::string line) = 0;
};

class ParserContext
{
public:
    std::unordered_map<std::string, std::string> Metadata;
};

class IParserInterceptor {
public:
    virtual ~IParserInterceptor() = default;
    virtual void intercept(std::string& line) = 0;
};

IConfig* create_config(const char* version = __DATE__ " " __TIME__);
IShell* create_shell();
IGenerator* create_blog_generator(IConfig* config, IShell* shell);
IGenerator* create_book_generator(IConfig* config, IShell* shell);
IGenerator* create_composite_generator(IConfig* config, IShell* shell);

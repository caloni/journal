#pragma once
#include "iconfig.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <string>
#include <vector>

namespace fs = std::filesystem;

class Config : public IConfig {
public:
    bool parse(int argc, char* argv[]);
    void print_usage();
    bool is_blog();
    bool is_book();
    bool is_publish();
    bool is_backup();
    bool is_git_commit_push_private_repo();
    fs::path get_basedir();
    fs::path get_scriptdir();
    fs::path get_private_dir();
    fs::path get_private_repo();

private:
    using ConfigMap = std::unordered_map<std::string, std::string>;
    struct ParsedArgs {
        std::string command;
        ConfigMap options;
    };
    ConfigMap load_config_file(const std::string& filename);
    ConfigMap load_config_default();
    // Parses: [command] [--key=value | --key]
    ParsedArgs parse_arguments(int argc, char* argv[]);
    ConfigMap merge_configs(const ConfigMap& base, const ConfigMap& overrides);

    std::string m_command;
    ConfigMap m_options;
};


#pragma once
#include "journal.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;

class Config : public IConfig {
public:
    Config(const char* version) : m_version(version) {}
    bool parse(int argc, char* argv[]);
    std::string usage();
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

    const char* m_version;
    std::string m_command;
    ConfigMap m_options;
};

IConfig* create_config(const char* version) {
    return new Config(version);
}

Config::ConfigMap Config::load_config_file(const std::string& filename) {
    ConfigMap config;
    std::ifstream file(filename);
    if (!file) {
        return config;
    }

    std::string line;
    while (std::getline(file, line)) {
        auto comment_pos = line.find('#');
        if (comment_pos != std::string::npos)
            line = line.substr(0, comment_pos);
        if (line.empty())
            continue;

        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            config[key] = value;
        }
    }

    return config;
}

Config::ConfigMap Config::load_config_default() {
    ConfigMap config;
    config["basedir"] = fs::current_path().string();
    config["scriptdir"] = (fs::current_path() / "src").string();
    config["private_dir"] = (fs::current_path() / "private").string();
    config["private_repo"] = (fs::current_path() / "private" / ".git").string();
    return config;
}

// Parses: [command] [--key=value | --key]
Config::ParsedArgs Config::parse_arguments(int argc, char* argv[]) {
    ParsedArgs parsed;
    bool command_found = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (!command_found && arg.rfind("--", 0) != 0) {
            parsed.command = arg;
            command_found = true;
        }
        else if (arg.rfind("--", 0) == 0) {
            arg = arg.substr(2);
            auto pos = arg.find('=');
            if (pos != std::string::npos) {
                std::string key = arg.substr(0, pos);
                std::string value = arg.substr(pos + 1);
                parsed.options[key] = value;
            }
            else {
                parsed.options[arg] = "true"; // automatic boolean
            }
        }
    }

    return parsed;
}

Config::ConfigMap Config::merge_configs(const ConfigMap& base, const ConfigMap& overrides) {
    ConfigMap result = base;
    for (const auto& [key, value] : overrides) {
        result[key] = value;
    }
    return result;
}

bool Config::parse(int argc, char* argv[]) {
    std::string config_path = "journal_config.txt";

    ConfigMap config_default = load_config_default();
    ConfigMap config_file = load_config_file(config_path);
    ParsedArgs parsed = parse_arguments(argc, argv);
    m_command = parsed.command;
    m_options = merge_configs(config_default, config_file);
    m_options = merge_configs(m_options, parsed.options);

    return is_blog() || is_book();
}

std::string Config::usage() {
    std::string usage = R"(Usage: journal command [options]
Command:
  blog             Build blog
  book             Build book
  both             Build both
Options:
  --publish          Publish content)";

    return std::string("Journal v. ") + m_version + "\n" + usage;
}

bool Config::is_blog() {
    return m_command == "blog" || m_command == "both";
}
bool Config::is_book() {
    return m_command == "book" || m_command == "both";
}
bool Config::is_publish() {
    return m_options["publish"] == "true";
}
bool Config::is_backup() {
    return m_options["backup"] == "true";
}
bool Config::is_git_commit_push_private_repo() {
    return m_options["git_commit_push_private_repo"] == "true";
}
fs::path Config::get_basedir() {
    return m_options["basedir"];
}
fs::path Config::get_scriptdir() {
    return m_options["scriptdir"];
}
fs::path Config::get_private_dir() {
    return m_options["private_dir"];
}
fs::path Config::get_private_repo() {
    return m_options["private_repo"];
}

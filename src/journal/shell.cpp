#include "shell.h"
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <chrono>
#include <sstream>
#include <array>
#include <strsafe.h>

namespace fs = std::filesystem;

void Shell::setup_encoding() {
#ifdef _WIN32
        (void)_putenv("LC_ALL=en_US.UTF-8");
#else
        setenv("LC_ALL", "en_US.UTF-8", 1);
#endif
}

std::string Shell::run_command(const std::string& cmd) {
    std::array<char, 128> buffer;
    std::string result;

    FILE* pipe = _popen(cmd.c_str(), "r");
    if (!pipe) throw std::runtime_error("popen() failed!");

    while (fgets(buffer.data(), (int)buffer.size(), pipe) != nullptr)
        result += buffer.data();

    _pclose(pipe);
    result.erase(result.find_last_not_of(" \n\r\t") + 1); // Trim
    return result;
}

std::string Shell::current_datetime() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    char buf[100];
    tm ltime;
    localtime_s(&ltime, &now_c);
    std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S%z", &ltime);
    return std::string(buf);
}

void Shell::clear_directory(const fs::path& dir) {
    for (const auto& entry : fs::directory_iterator(dir)) {
        try {
            auto name = entry.path().filename().string();

            // Skip ".git" directory
            if (name == ".git") continue;

            if (fs::is_directory(entry)) {
                fs::remove_all(entry);
            } else {
                fs::remove(entry);
            }

        } catch (const std::exception& e) {
            std::cerr << "Failed to delete " << entry.path() << ". Reason: " << e.what() << '\n';
        }
    }
}

void Shell::create_backup(const fs::path& basedir) {
    fs::path backup_dir = basedir / ".." / "backup";
    fs::create_directories(backup_dir);
    fs::path temp_dir = fs::temp_directory_path() / "journal_backup";

    if (fs::exists(temp_dir)) {
        fs::remove_all(temp_dir);
    }

    fs::create_directories(temp_dir);

    // Copy everything except .git
    for (const auto& entry : fs::recursive_directory_iterator(basedir)) {
        if (entry.path().string().find(".git") != std::string::npos) {
            continue;
        }
        fs::path relative_path = fs::relative(entry.path(), basedir);
        fs::path target_path = temp_dir / relative_path;

        if (entry.is_directory()) {
            fs::create_directories(target_path);
        } else if (entry.is_regular_file()) {
            fs::copy_file(entry.path(), target_path, fs::copy_options::overwrite_existing);
        }
    }

    // Zip it (requires external zip command like 7z or zip)
    std::string zip_cmd = "zip -r \"" + (backup_dir / "journal.zip").string() + "\" \"" + temp_dir.string() + "\"";
    std::system(zip_cmd.c_str());

    std::cout << "backup created at " << (backup_dir / "journal.zip") << "\n";
}

void Shell::run_script(const std::string& script_path) {
    std::string cmd = "python \"" + script_path + "\"";
    int result = std::system(cmd.c_str());
    if (result != 0) {
        std::cerr << "Script failed with code: " << result << "\n";
    }
}

void Shell::git_commit_push(const fs::path& path, const std::string& message) {
    current_path(path);
    std::system("git add --all");
    std::string commit_cmd = "git commit -m \"" + message + "\"";
    std::system(commit_cmd.c_str());
    std::system("git push");
}

void Shell::current_path(const fs::path& path)
{
    fs::current_path(path);
}

std::ostream& Shell::cout()
{
    return std::cout;
}

std::ostream& Shell::cerr()
{
    return std::cerr;
}

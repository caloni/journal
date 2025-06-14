#include "journal.h"
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <chrono>
#include <sstream>
#include <array>
#include <strsafe.h>

namespace fs = std::filesystem;

void setup_encoding() {
#ifdef _WIN32
        (void)_putenv("LC_ALL=en_US.UTF-8");
#else
        setenv("LC_ALL", "en_US.UTF-8", 1);
#endif
}

std::string run_command(const std::string& cmd) {
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

std::string current_datetime() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    char buf[100];
    tm ltime;
    localtime_s(&ltime, &now_c);
    std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S%z", &ltime);
    return std::string(buf);
}

void clear_directory(const fs::path& dir) {
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


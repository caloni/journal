#include "journal.h"
#include "util.h"
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <chrono>
#include <sstream>
#include <array>
#include <strsafe.h>

namespace fs = std::filesystem;

void Journal::run() {
    Util::setup_encoding();
    auto original_current_path = fs::current_path();
    try {
        std::cout << "basedir: " << m_config.get_basedir().string() << '\n';
        fs::current_path(m_config.get_basedir());

        if (m_config.is_blog()) {
            if (m_config.is_publish() && m_config.is_backup()) {
                Util::create_backup(fs::current_path());
            }
            m_blog.create(m_config.get_basedir(), m_config.get_scriptdir());

            if (m_config.is_git_commit_push_private_repo()) {
                Util::git_commit_push(m_config.get_private_dir(), "Add journal changes");
            }

            if( m_config.is_publish() ) {
                Util::git_commit_push(fs::current_path(), "Add journal changes");
                Util::git_commit_push("public/blog", "Publish lastest changes");
            }
        }
        if (m_config.is_book()) {
            m_book.create(m_config.get_basedir(), m_config.get_scriptdir());
        }

    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
    }
    fs::current_path(original_current_path);
}


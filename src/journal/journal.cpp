#include "journal.h"
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

void Journal::run() {
    m_shell.setup_encoding();
    auto original_current_path = fs::current_path();
    try {
        std::cout << "basedir: " << m_config.get_basedir().string() << '\n';
        m_shell.current_path(m_config.get_basedir());

        if (m_config.is_blog()) {
            if (m_config.is_publish() && m_config.is_backup()) {
                m_shell.create_backup(fs::current_path());
            }
            m_blog.create(m_config.get_basedir(), m_config.get_scriptdir());

            if (m_config.is_git_commit_push_private_repo()) {
                m_shell.git_commit_push(m_config.get_private_dir(), "Add journal changes");
            }

            if( m_config.is_publish() ) {
                m_shell.git_commit_push(fs::current_path(), "Add journal changes");
                m_shell.git_commit_push("public/blog", "Publish lastest changes");
            }
        }
        if (m_config.is_book()) {
            m_book.create(m_config.get_basedir(), m_config.get_scriptdir());
        }

    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
    }
    m_shell.current_path(original_current_path);
}


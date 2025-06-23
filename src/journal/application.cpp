#include "application.h"

void Application::run() {
    m_shell->setup_encoding();
    auto original_current_path = fs::current_path();
    try {
        m_shell->cout() << "basedir: " << m_config->get_basedir().string() << '\n';
        m_shell->current_path(m_config->get_basedir());
        m_generator->generate();
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
    }
    m_shell->current_path(original_current_path);
}


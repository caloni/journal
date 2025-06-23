#pragma once
#include "journal.h"

class OutputFactory : public IOutputFactory {
public:
    OutputFactory(IConfig* config, IShell* shell) : m_config(config), m_shell(shell) {}
    OutputList generate();
private:
    IConfig* m_config;
    IShell* m_shell;
};

IOutputFactory* create_output_factory(IConfig* config, IShell* shell) {
    return new OutputFactory(config, shell);
}

IOutputFactory::OutputList OutputFactory::generate()
{
    IOutputFactory::OutputList result;
    if (m_config->is_blog()) {
        result.push_back(create_blog_generator(m_config, m_shell));
    }
    if (m_config->is_book()) {
        result.push_back(create_book_generator(m_config, m_shell));
    }
    return result;
}

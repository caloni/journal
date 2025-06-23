#include "journal.h"
#include <fstream>
#include <memory>
#include <vector>

class CompositeGenerator : public IGenerator {
public:
    CompositeGenerator(IConfig* config, IShell* shell);
    int generate();
private:
    typedef std::vector<std::unique_ptr<IGenerator>> GeneratorsList;
    GeneratorsList m_generators;
};

IGenerator* create_composite_generator(IConfig* config, IShell* shell) {
    return new CompositeGenerator(config, shell);
}

CompositeGenerator::CompositeGenerator(IConfig* config, IShell* shell) {
    if (config->is_blog()) {
        m_generators.push_back(GeneratorsList::value_type(create_blog_generator(config, shell)));
    }
    if (config->is_book()) {
        m_generators.push_back(GeneratorsList::value_type(create_book_generator(config, shell)));
    }
}

int CompositeGenerator::generate() {
    for( auto& generator: m_generators ) {
        generator->generate();
    }
    return 0;
}

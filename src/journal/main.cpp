#include "journal.h"
#include <memory>

int main(int argc, char*argv[]) {
    try {
        std::unique_ptr<IConfig> config(create_config());
        std::unique_ptr<IShell> shell(create_shell());
        if (!config->parse(argc, argv)) {
            shell->cout() << config->usage() << std::endl;
            return 1;
        }
        std::unique_ptr<IGenerator> generator(create_composite_generator(config.get(), shell.get()));
        shell->setup_encoding();
        auto original_current_path = fs::current_path();
        shell->cout() << "basedir: " << config->get_basedir().string() << '\n';
        shell->current_path(config->get_basedir());
        generator->generate();
        shell->current_path(original_current_path);
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}

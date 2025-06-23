#include "application.h"
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
        Application application(config.get(), shell.get(), generator.get());
        application.run();
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}

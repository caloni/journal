# Journal

A modular C++ command-line tool for generating and managing digital content such as blogs and books. Journal automates the process of building, publishing, backing up, and versioning your content with a flexible, extensible architecture.

## Features

- **Multi-format Generation**: Build blogs, books, or both simultaneously
- **Automated Publishing**: Git-based publishing with commit and push operations
- **Backup System**: Automatic backup creation before publishing
- **Modular Architecture**: Extensible design based on interfaces
- **Cross-platform**: Works on Windows, Linux, and macOS
- **Configuration Management**: Flexible configuration via command-line and config files
- **Version Control Integration**: Built-in git operations for content management

## Installation

### Prerequisites

- CMake 3.7 or higher
- C++20 compatible compiler
- Git (for version control operations)
- Zip utility (for backup creation)

### Building from Source

```bash
# Clone the repository
git clone https://github.com/yourusername/journal.git
cd journal

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make

# Install (optional)
make install
```

The default installation path is `C:/tools` on Windows. You can modify this in `CMakeLists.txt`.

## Usage

### Basic Commands

```bash
# Build blog content
journal blog

# Build book content
journal book

# Build both blog and book
journal both

# Build and publish
journal blog --publish

# Build with backup and publish
journal blog --publish --backup

# Build with git commit and push to private repository
journal blog --git_commit_push_private_repo
```

### Configuration

Journal supports configuration through command-line arguments and config files. The tool automatically loads configuration from:

1. Default configuration
2. Config file (if specified)
3. Command-line overrides

### Configuration Options

- `--basedir`: Base directory for content
- `--scriptdir`: Directory containing build scripts
- `--private_dir`: Private content directory
- `--private_repo`: Private git repository path
- `--publish`: Enable publishing mode
- `--backup`: Create backup before publishing
- `--git_commit_push_private_repo`: Commit and push to private repository

## Architecture

Journal is built with a modular, interface-based architecture:

### Core Interfaces

- **IConfig**: Configuration management and parsing
- **IShell**: System operations and command execution
- **IGenerator**: Content generation (blog, book, composite)
- **IParser**: Content parsing and processing
- **IParserInterceptor**: Line interception and modification

### Main Components

- **Application**: Main orchestrator that coordinates all operations
- **Config**: Implements configuration parsing and management
- **Shell**: Handles system operations, git commands, and file management
- **BlogGenerator**: Generates blog content
- **BookGenerator**: Generates book content
- **CompositeGenerator**: Runs multiple generators in sequence

### Workflow

1. **Configuration**: Parse command-line arguments and load config files
2. **Setup**: Initialize encoding and change to base directory
3. **Generation**: Execute appropriate generators (blog, book, or both)
4. **Publishing**: Commit changes and push to repositories (if enabled)
5. **Cleanup**: Restore original working directory

## Development

### Project Structure

```
journal/
├── application.cpp/h      # Main application orchestrator
├── blog_generator.cpp     # Blog content generator
├── book_generator.cpp     # Book content generator
├── composite_generator.cpp # Multi-generator coordinator
├── config.cpp            # Configuration management
├── intercepting_parser.cpp # Content parsing with interception
├── journal.h             # Core interfaces and declarations
├── main.cpp              # Entry point
├── markdown_parser.cpp   # Markdown processing
├── metadata_interceptor.cpp # Metadata handling
├── output_factory.cpp    # Generator factory
├── shell.cpp             # System operations
├── tests/                # Test suite
└── CMakeLists.txt        # Build configuration
```

### Building for Development

```bash
# Enable tests
cmake -DBUILD_TESTING=ON ..
make

# Run tests
make test
```

### Adding New Generators

To add a new content generator:

1. Implement the `IGenerator` interface
2. Create a factory function following the pattern in `journal.h`
3. Add the generator to `CompositeGenerator` if needed
4. Update configuration options if required

Example:
```cpp
class MyGenerator : public IGenerator {
public:
    MyGenerator(IConfig* config, IShell* shell) : m_config(config), m_shell(shell) {}
    int generate() override {
        // Your generation logic here
        return 0;
    }
private:
    IConfig* m_config;
    IShell* m_shell;
};

IGenerator* create_my_generator(IConfig* config, IShell* shell) {
    return new MyGenerator(config, shell);
}
```

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Development Guidelines

- Follow the existing code style and architecture patterns
- Add tests for new functionality
- Update documentation for new features
- Ensure cross-platform compatibility
- Use the existing interface patterns for extensibility

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Built with modern C++20 features
- Uses CMake for cross-platform building
- Integrates with Git for version control
- Inspired by static site generators and content management systems

## Support

For issues, questions, or contributions, please:

1. Check existing issues on GitHub
2. Create a new issue with detailed information
3. Provide reproduction steps for bugs
4. Include system information and build details

---

**Journal** - Automate your digital content workflow with style and efficiency. 
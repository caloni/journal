# Journal - C# Translation

This is a C# translation of the original C++ journal/blog management system. The translation maintains the exact same behavior and architecture as the original C++ version.

## Project Structure

```
Journal/
├── Interfaces/           # Abstract interfaces (equivalent to C++ .h files)
│   ├── IBlog.cs
│   ├── IBook.cs
│   ├── IConfig.cs
│   ├── IShell.cs
│   └── IJournal.cs
├── Config.cs            # Configuration management
├── Shell.cs             # System interaction layer
├── Blog.cs              # Blog creation functionality
├── Book.cs              # Book creation functionality
├── Journal.cs           # Main orchestrator
├── Program.cs           # Entry point
├── Journal.csproj       # Project file
├── Tests/               # Unit tests
│   ├── TestConfig.cs
│   ├── TestJournal.cs
│   └── TestMain.cs
└── README.md
```

## Features

### Commands
- `blog` - Build blog content
- `book` - Build book content  
- `both` - Build both blog and book

### Options
- `--publish` - Publish content to Git repositories
- `--backup` - Create backups before operations
- `--git_commit_push_private_repo` - Commit and push to private repository

## Usage

### Build and Run
```bash
# Build the project
dotnet build

# Run with blog command
dotnet run -- blog

# Run with book command and publish
dotnet run -- book --publish

# Run both blog and book
dotnet run -- both --publish --backup
```

### Configuration
The system uses a `journal_config.txt` file for configuration. If not found, it uses default values:

```
basedir=current_directory
scriptdir=current_directory/src
private_dir=current_directory/private
private_repo=current_directory/private/.git
```

### Testing
```bash
# Run all tests
dotnet test

# Run specific test
dotnet test --filter "TestConfig"
```

## Architecture

The C# translation maintains the same clean architecture as the original C++ version:

1. **Interface-based design**: All components implement interfaces for loose coupling
2. **Dependency injection**: Components are injected through constructors
3. **Separation of concerns**: Each class has a single responsibility
4. **Error handling**: Comprehensive exception handling throughout

## Key Differences from C++ Version

1. **File extensions**: `.cpp`/`.h` → `.cs`
2. **Testing framework**: Doctest → xUnit
3. **Mocking**: FakeIt → Moq
4. **Build system**: CMake → MSBuild (via .csproj)
5. **Process management**: Uses `System.Diagnostics.Process` for shell commands
6. **File operations**: Uses .NET `System.IO` classes

## Dependencies

- .NET 8.0
- xUnit (for testing)
- Moq (for mocking)

## Notes

- The `Blog` and `Book` classes contain placeholder implementations since the original C++ source files don't show the actual creation logic
- The `Shell` class uses Windows `cmd.exe` for command execution to match the original behavior
- All error handling and logging behavior matches the original C++ version exactly 
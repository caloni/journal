#include "journal.h"
#include <array>
#include <fstream>
#include <unordered_map>

class BlogGenerator final : public IGenerator
{
public:
    BlogGenerator(IConfig* config, IShell* shell) : m_config(config), m_shell(shell)
    {
    }

    int generate()
    {
        if (m_config->is_publish() && m_config->is_backup())
        {
            m_shell->create_backup(fs::current_path());
        }
        int result = generate_blog();

        if (m_config->is_git_commit_push_private_repo())
        {
            m_shell->git_commit_push(m_config->get_private_dir(), "Add journal changes");
        }

        if (m_config->is_publish())
        {
            m_shell->git_commit_push(fs::current_path(), "Add journal changes");
            m_shell->git_commit_push("public/blog", "Publish lastest changes");
        }
        return result;
    }

private:
    int generate_blog()
    {
        fs::path basedir = m_config->get_basedir();
        fs::path scriptdir = m_config->get_scriptdir();
        std::string build_version = m_shell->run_command("git rev-parse --short HEAD");
        std::string current_date = m_shell->current_datetime();

        fs::path blog_public = basedir / "public" / "blog";
        if (!fs::exists(blog_public))
        {
            fs::create_directories(blog_public);
        }

        m_shell->clear_directory(blog_public);

        fs::copy(basedir / "blog", blog_public, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
        fs::copy(basedir / "img" / "blog", blog_public / "img",
                 fs::copy_options::recursive | fs::copy_options::overwrite_existing);

        // Run awk scripts
        std::string util_awk = (scriptdir / "Util.awk").string();
        std::string meta_awk = (scriptdir / "MetadataParser.awk").string();
        std::string writer_awk = (scriptdir / "MetadataWriter.awk").string();
        std::string md_file = "journal.md";

        std::string cmd = "gawk -f \"" + util_awk + "\" -f \"" + meta_awk + "\" -f \"" + writer_awk + "\" \"" + md_file
            + "\"";
        int ret = std::system(cmd.c_str());
        if (ret != 0) std::cerr << "MetadataWriter.awk returned " << ret << '\n';

        // Append metadata
        {
            std::ofstream metadata((basedir / "public" / "metadata.txt").string(), std::ios::app | std::ios::binary);
            metadata << "metadata_current_date " << current_date << "\n";
            metadata << "metadata_build_version " << build_version << "\n";
        }

        // Run second awk command
        std::string parser_awk = (scriptdir / "MarkdownParser.awk").string();
        std::string blogwriter_awk = (scriptdir / "BlogWriter.awk").string();
        std::string metadata_file = (basedir / "public" / "metadata.txt").string();

        cmd = "gawk -f \"" + util_awk + "\" -f \"" + parser_awk + "\" -f \"" + blogwriter_awk + "\" \"" + metadata_file
            + "\" \"" + md_file + "\"";
        ret = std::system(cmd.c_str());
        if (ret != 0) std::cerr << "txt2blog.awk returned " << ret << '\n';

        m_shell->cout() << "blog generated\n";

        return 0;
    }

    IConfig* m_config;
    IShell* m_shell;
};

IGenerator* create_blog_generator(IConfig* config, IShell* shell)
{
    return new BlogGenerator(config, shell);
}

class BookGenerator : public IGenerator
{
public:
    BookGenerator(IConfig* config, IShell* shell) : m_config(config), m_shell(shell)
    {
    }

    int generate()
    {
        fs::path basedir = m_config->get_basedir();
        fs::path scriptdir = m_config->get_scriptdir();
        bool includeprivate = true; ///@todo configurable
        std::string build_version = m_shell->run_command("git rev-parse --short HEAD");
        std::string current_date = m_shell->current_datetime();

        fs::path book_public = basedir / "public" / "book";
        if (!fs::exists(book_public))
        {
            fs::create_directories(book_public);
        }

        m_shell->clear_directory(book_public);

        fs::copy(basedir / "book", book_public, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
        fs::copy(basedir / "img" / "book", book_public / "EPUB" / "img",
                 fs::copy_options::recursive | fs::copy_options::overwrite_existing);
        fs::copy(basedir / "img" / "blog", book_public / "EPUB" / "img",
                 fs::copy_options::recursive | fs::copy_options::overwrite_existing);

        // Run awk scripts
        std::string util_awk = (scriptdir / "Util.awk").string();
        std::string meta_awk = (scriptdir / "MetadataParser.awk").string();
        std::string writer_awk = (scriptdir / "MetadataWriter.awk").string();
        std::string md_file = "journal.md";
        std::string md_file_private = (fs::path("private") / "journal.md").string();

        std::string cmd = "gawk -f \"" + util_awk + "\" -f \"" + meta_awk + "\" -f \"" + writer_awk + "\" \"" + md_file
            + "\"";
        int ret = std::system(cmd.c_str());
        if (ret != 0) std::cerr << "MetadataWriter.awk returned " << ret << '\n';

        // Append metadata
        {
            std::ofstream metadata((basedir / "public" / "metadata.txt").string(), std::ios::app | std::ios::binary);
            metadata << "metadata_current_date " << current_date << "\n";
            metadata << "metadata_build_version " << build_version << "\n";
        }

        // Run second awk command
        std::string parser_awk = (scriptdir / "MarkdownParser.awk").string();
        std::string bookwriter_awk = (scriptdir / "BookWriter.awk").string();
        std::string metadata_file = (basedir / "public" / "metadata.txt").string();

        cmd = "gawk -f \"" + util_awk + "\" -f \"" + parser_awk + "\" -f \"" + bookwriter_awk + "\" \"" + metadata_file
            + "\" \"" + md_file + "\"";
        if (includeprivate)
        {
            cmd += " \"" + md_file_private + "\"";
        }
        ret = std::system(cmd.c_str());
        if (ret != 0) std::cerr << "txt2book.awk returned " << ret << '\n';

        // Pack the book
        m_shell->current_path(book_public);
        cmd = "python repack.py";
        ret = std::system(cmd.c_str());
        if (ret != 0) std::cerr << "repack.py returned " << ret << '\n';
        m_shell->current_path(basedir);

        m_shell->cout() << "book generated\n";

        return 0;
    }

private:
    IConfig* m_config;
    IShell* m_shell;
};

IGenerator* create_book_generator(IConfig* config, IShell* shell)
{
    return new BookGenerator(config, shell);
}

class CompositeGenerator : public IGenerator
{
public:
    CompositeGenerator(IConfig* config, IShell* shell)
    {
        if (config->is_blog())
        {
            m_generators.push_back(GeneratorsList::value_type(create_blog_generator(config, shell)));
        }
        if (config->is_book())
        {
            m_generators.push_back(GeneratorsList::value_type(create_book_generator(config, shell)));
        }
    }

    int generate()
    {
        for (auto& generator : m_generators)
        {
            generator->generate();
        }
        return 0;
    }

private:
    typedef std::vector<std::unique_ptr<IGenerator>> GeneratorsList;
    GeneratorsList m_generators;
};

IGenerator* create_composite_generator(IConfig* config, IShell* shell)
{
    return new CompositeGenerator(config, shell);
}

class Config : public IConfig
{
public:
    Config(const char* version) : m_version(version)
    {
    }

    bool parse(int argc, char* argv[])
    {
        std::string config_path = "journal_config.txt";

        ConfigMap config_default = load_config_default();
        ConfigMap config_file = load_config_file(config_path);
        ParsedArgs parsed = parse_arguments(argc, argv);
        m_command = parsed.command;
        m_options = merge_configs(config_default, config_file);
        m_options = merge_configs(m_options, parsed.options);

        return is_blog() || is_book();
    }

    std::string usage()
    {
        std::string usage = R"(Usage: journal command [options]
Command:
  blog             Build blog
  book             Build book
  both             Build both
Options:
  --publish          Publish content)";

        return std::string("Journal v. ") + m_version + "\n" + usage;
    }

    bool is_blog() { return m_command == "blog" || m_command == "both"; }
    bool is_book() { return m_command == "book" || m_command == "both"; }
    bool is_publish() { return m_options["publish"] == "true"; }
    bool is_backup() { return m_options["backup"] == "true"; }
    bool is_git_commit_push_private_repo() { return m_options["git_commit_push_private_repo"] == "true"; }
    fs::path get_basedir() { return m_options["basedir"]; }
    fs::path get_scriptdir() { return m_options["scriptdir"]; }
    fs::path get_private_dir() { return m_options["private_dir"]; }
    fs::path get_private_repo() { return m_options["private_repo"]; }

private:
    using ConfigMap = std::unordered_map<std::string, std::string>;

    struct ParsedArgs
    {
        std::string command;
        ConfigMap options;
    };

    ConfigMap load_config_file(const std::string& filename)
    {
        ConfigMap config;
        std::ifstream file(filename);
        if (!file)
        {
            return config;
        }

        std::string line;
        while (std::getline(file, line))
        {
            auto comment_pos = line.find('#');
            if (comment_pos != std::string::npos)
                line = line.substr(0, comment_pos);
            if (line.empty())
                continue;

            std::istringstream iss(line);
            std::string key, value;
            if (std::getline(iss, key, '=') && std::getline(iss, value))
            {
                config[key] = value;
            }
        }

        return config;
    }

    ConfigMap load_config_default()
    {
        ConfigMap config;
        config["basedir"] = fs::current_path().string();
        config["scriptdir"] = (fs::current_path() / "src").string();
        config["private_dir"] = (fs::current_path() / "private").string();
        config["private_repo"] = (fs::current_path() / "private" / ".git").string();
        return config;
    }

    // Parses: [command] [--key=value | --key]
    ParsedArgs parse_arguments(int argc, char* argv[])
    {
        ParsedArgs parsed;
        bool command_found = false;

        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];

            if (!command_found && arg.rfind("--", 0) != 0)
            {
                parsed.command = arg;
                command_found = true;
            }
            else if (arg.rfind("--", 0) == 0)
            {
                arg = arg.substr(2);
                auto pos = arg.find('=');
                if (pos != std::string::npos)
                {
                    std::string key = arg.substr(0, pos);
                    std::string value = arg.substr(pos + 1);
                    parsed.options[key] = value;
                }
                else
                {
                    parsed.options[arg] = "true"; // automatic boolean
                }
            }
        }

        return parsed;
    }

    ConfigMap merge_configs(const ConfigMap& base, const ConfigMap& overrides)
    {
        ConfigMap result = base;
        for (const auto& [key, value] : overrides)
        {
            result[key] = value;
        }
        return result;
    }

    const char* m_version;
    std::string m_command;
    ConfigMap m_options;
};

IConfig* create_config(const char* version)
{
    return new Config(version);
}

class InterceptingParser : public IParser
{
public:
    void parse_line(std::string line)
    {
        ///@todo call interceptor and then parser
    }
};

class MarkdownParser : public IParser
{
    void parse_line(std::string line)
    {
    }
};

class MetadataInterceptor : public IParserInterceptor
{
public:
    void intercept(std::string& line)
    {
        ///@todo treat metadata before markdown parser
    }
};

class Shell : public IShell
{
public:
    void setup_encoding()
    {
#ifdef _WIN32
        (void)_putenv("LC_ALL=en_US.UTF-8");
#else
        setenv("LC_ALL", "en_US.UTF-8", 1);
#endif
    }

    std::string run_command(const std::string& cmd)
    {
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

    std::string current_datetime()
    {
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        char buf[100];
        tm ltime;
        localtime_s(&ltime, &now_c);
        std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S%z", &ltime);
        return std::string(buf);
    }

    void clear_directory(const fs::path& dir)
    {
        for (const auto& entry : fs::directory_iterator(dir))
        {
            try
            {
                auto name = entry.path().filename().string();

                // Skip ".git" directory
                if (name == ".git") continue;

                if (fs::is_directory(entry))
                {
                    fs::remove_all(entry);
                }
                else
                {
                    fs::remove(entry);
                }
            }
            catch (const std::exception& e)
            {
                std::cerr << "Failed to delete " << entry.path() << ". Reason: " << e.what() << '\n';
            }
        }
    }

    void create_backup(const fs::path& basedir)
    {
        fs::path backup_dir = basedir / ".." / "backup";
        fs::create_directories(backup_dir);
        fs::path temp_dir = fs::temp_directory_path() / "journal_backup";

        if (fs::exists(temp_dir))
        {
            fs::remove_all(temp_dir);
        }

        fs::create_directories(temp_dir);

        // Copy everything except .git
        for (const auto& entry : fs::recursive_directory_iterator(basedir))
        {
            if (entry.path().string().find(".git") != std::string::npos)
            {
                continue;
            }
            fs::path relative_path = fs::relative(entry.path(), basedir);
            fs::path target_path = temp_dir / relative_path;

            if (entry.is_directory())
            {
                fs::create_directories(target_path);
            }
            else if (entry.is_regular_file())
            {
                fs::copy_file(entry.path(), target_path, fs::copy_options::overwrite_existing);
            }
        }

        // Zip it (requires external zip command like 7z or zip)
        std::string zip_cmd = "zip -r \"" + (backup_dir / "journal.zip").string() + "\" \"" + temp_dir.string() + "\"";
        std::system(zip_cmd.c_str());

        std::cout << "backup created at " << (backup_dir / "journal.zip") << "\n";
    }

    void run_script(const std::string& script_path)
    {
        std::string cmd = "python \"" + script_path + "\"";
        int result = std::system(cmd.c_str());
        if (result != 0)
        {
            std::cerr << "Script failed with code: " << result << "\n";
        }
    }

    void git_commit_push(const fs::path& path, const std::string& message)
    {
        current_path(path);
        std::system("git add --all");
        std::string commit_cmd = "git commit -m \"" + message + "\"";
        std::system(commit_cmd.c_str());
        std::system("git push");
    }

    void current_path(const fs::path& path)
    {
        fs::current_path(path);
    }

    std::ostream& cout()
    {
        return std::cout;
    }

    std::ostream& cerr()
    {
        return std::cerr;
    }
};

IShell* create_shell()
{
    return new Shell();
}

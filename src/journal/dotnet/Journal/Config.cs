using Journal.Interfaces;
using System.Text;

namespace Journal;

public class Config : IConfig
{
    private string m_command = "";
    private Dictionary<string, string> m_options = new();

    public bool Parse(string[] args)
    {
        string configPath = "journal_config.txt";

        var configDefault = LoadConfigDefault();
        var configFile = LoadConfigFile(configPath);
        var parsed = ParseArguments(args);
        m_command = parsed.Command;
        m_options = MergeConfigs(configDefault, configFile);
        m_options = MergeConfigs(m_options, parsed.Options);

        return IsBlog() || IsBook();
    }

    public void PrintUsage()
    {
        Console.WriteLine("Usage: journal command [options]");
        Console.WriteLine("Command:");
        Console.WriteLine("  blog             Build blog");
        Console.WriteLine("  book             Build book");
        Console.WriteLine("  both             Build both");
        Console.WriteLine("Options:");
        Console.WriteLine("  --publish          Publish content");
    }

    public bool IsBlog()
    {
        return m_command == "blog" || m_command == "both";
    }

    public bool IsBook()
    {
        return m_command == "book" || m_command == "both";
    }

    public bool IsPublish()
    {
        return m_options.GetValueOrDefault("publish") == "true";
    }

    public bool IsBackup()
    {
        return m_options.GetValueOrDefault("backup") == "true";
    }

    public bool IsGitCommitPushPrivateRepo()
    {
        return m_options.GetValueOrDefault("git_commit_push_private_repo") == "true";
    }

    public string GetBasedir()
    {
        return m_options.GetValueOrDefault("basedir", "");
    }

    public string GetScriptdir()
    {
        return m_options.GetValueOrDefault("scriptdir", "");
    }

    public string GetPrivateDir()
    {
        return m_options.GetValueOrDefault("private_dir", "");
    }

    public string GetPrivateRepo()
    {
        return m_options.GetValueOrDefault("private_repo", "");
    }

    private Dictionary<string, string> LoadConfigFile(string filename)
    {
        var config = new Dictionary<string, string>();
        
        if (!File.Exists(filename))
        {
            Console.Error.WriteLine($"Warning: config file not found: {filename}");
            return config;
        }

        foreach (var line in File.ReadAllLines(filename))
        {
            var trimmedLine = line.Trim();
            var commentPos = trimmedLine.IndexOf('#');
            if (commentPos != -1)
                trimmedLine = trimmedLine.Substring(0, commentPos);
            
            if (string.IsNullOrEmpty(trimmedLine))
                continue;

            var parts = trimmedLine.Split('=', 2);
            if (parts.Length == 2)
            {
                config[parts[0].Trim()] = parts[1].Trim();
            }
        }

        return config;
    }

    private Dictionary<string, string> LoadConfigDefault()
    {
        var config = new Dictionary<string, string>();
        var currentPath = Directory.GetCurrentDirectory();
        config["basedir"] = currentPath;
        config["scriptdir"] = Path.Combine(currentPath, "src");
        config["private_dir"] = Path.Combine(currentPath, "private");
        config["private_repo"] = Path.Combine(currentPath, "private", ".git");
        return config;
    }

    private class ParsedArgs
    {
        public string Command { get; set; } = "";
        public Dictionary<string, string> Options { get; set; } = new();
    }

    // Parses: [command] [--key=value | --key]
    private ParsedArgs ParseArguments(string[] args)
    {
        var parsed = new ParsedArgs();
        bool commandFound = false;

        for (int i = 0; i < args.Length; i++)
        {
            string arg = args[i];

            if (!commandFound && !arg.StartsWith("--"))
            {
                parsed.Command = arg;
                commandFound = true;
            }
            else if (arg.StartsWith("--"))
            {
                arg = arg.Substring(2);
                var pos = arg.IndexOf('=');
                if (pos != -1)
                {
                    string key = arg.Substring(0, pos);
                    string value = arg.Substring(pos + 1);
                    parsed.Options[key] = value;
                }
                else
                {
                    parsed.Options[arg] = "true"; // automatic boolean
                }
            }
        }

        return parsed;
    }

    private Dictionary<string, string> MergeConfigs(Dictionary<string, string> baseConfig, Dictionary<string, string> overrides)
    {
        var result = new Dictionary<string, string>(baseConfig);
        foreach (var kvp in overrides)
        {
            result[kvp.Key] = kvp.Value;
        }
        return result;
    }
} 
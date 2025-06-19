using System;
using System.ComponentModel.Design;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Linq;

class BlogBuilder
{
    static void Main()
    {
        var basePath = Directory.GetCurrentDirectory();
        var scriptDir = "src";
        string blogPath = Path.Combine("public", "blog");

        Console.WriteLine("basedir: " + basePath);

        if (Directory.Exists(blogPath))
        {
            Directory.SetCurrentDirectory(blogPath);
            foreach (var filename in Directory.GetFileSystemEntries("."))
            {
                var name = Path.GetFileName(filename);
                try
                {
                    if (File.Exists(name) || IsSymlink(name))
                    {
                        File.Delete(name);
                    }
                    else if (Directory.Exists(name) && name != ".git")
                    {
                        Directory.Delete(name, true);
                    }
                }
                catch (Exception e)
                {
                    Console.WriteLine($"Failed to delete {name}. Reason: {e.Message}");
                }
            }
            Directory.SetCurrentDirectory(basePath);
        }
        else
        {
            Directory.CreateDirectory(blogPath);
        }

        CopyDirectory("blog", blogPath);
        CopyDirectory(Path.Combine("img", "blog"), Path.Combine(blogPath, "img"));

        Environment.SetEnvironmentVariable("LC_ALL", "en_US.UTF-8");

        RunProcess("gawk", $"-f {Path.Combine(scriptDir, "Util.awk")} -f {Path.Combine(scriptDir, "MetadataParser.awk")} -f {Path.Combine(scriptDir, "MetadataWriter.awk")} journal.md");

        var now = DateTimeOffset.Now;
        var offset = (now.Offset < TimeSpan.Zero ? "-" : "+") + now.Offset.ToString("hhmm");
        var currentDate = now.ToString("yyyy-MM-ddTHH:mm:ss") + offset;
        string buildVersion = RunCommand("git", "rev-parse --short HEAD").Trim();

        using (var fs = new FileStream(Path.Combine("public", "metadata.txt"), FileMode.Append, FileAccess.Write))
        using (var writer = new StreamWriter(fs))
        {
            writer.WriteLine($"metadata_current_date {currentDate}");
            writer.WriteLine($"metadata_build_version {buildVersion}");
        }

        RunProcess("gawk", $"-f {Path.Combine(scriptDir, "Util.awk")} -f {Path.Combine(scriptDir, "MarkdownParser.awk")} -f {Path.Combine(scriptDir, "BlogWriter.awk")} public/metadata.txt journal.md");

        Console.WriteLine("blog generated");
    }

    static string RunCommand(string command, string arguments)
    {
        var psi = new ProcessStartInfo
        {
            FileName = command,
            Arguments = arguments,
            RedirectStandardOutput = true,
            UseShellExecute = false,
            CreateNoWindow = true
        };

        using var process = Process.Start(psi);
        string output = process.StandardOutput.ReadToEnd();
        process.WaitForExit();
        return output;
    }

    static void RunProcess(string command, string arguments)
    {
        var psi = new ProcessStartInfo
        {
            FileName = command,
            Arguments = arguments,
            RedirectStandardOutput = true,
            RedirectStandardError = true,
            UseShellExecute = false,
            CreateNoWindow = true
        };

        using var process = Process.Start(psi);
        string output = process.StandardOutput.ReadToEnd();
        string error = process.StandardError.ReadToEnd();
        process.WaitForExit();

        if (!string.IsNullOrWhiteSpace(output))
            Console.WriteLine(output);
        if (process.ExitCode != 0)
            Console.WriteLine($"{command} returned {process.ExitCode}\n{error}");
    }

    static void CopyDirectory(string sourceDir, string targetDir)
    {
        Directory.CreateDirectory(targetDir);
        foreach (var file in Directory.GetFiles(sourceDir, "*", SearchOption.AllDirectories))
        {
            var relativePath = Path.GetRelativePath(sourceDir, file);
            var destPath = Path.Combine(targetDir, relativePath);
            Directory.CreateDirectory(Path.GetDirectoryName(destPath));
            File.Copy(file, destPath, true);
        }
    }

    static bool IsSymlink(string path)
    {
        var fileInfo = new FileInfo(path);
        return fileInfo.Attributes.HasFlag(FileAttributes.ReparsePoint);
    }
}


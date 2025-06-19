using Journal.Interfaces;
using System.Diagnostics;
using System.Text;

namespace Journal;

public class Shell : IShell
{
    public void SetupEncoding()
    {
        // Set console encoding to UTF-8 to match C++ behavior
        Console.OutputEncoding = Encoding.UTF8;
        Console.InputEncoding = Encoding.UTF8;
    }

    public string RunCommand(string cmd)
    {
        try
        {
            var process = new Process();
            process.StartInfo.FileName = "cmd.exe";
            process.StartInfo.Arguments = $"/c {cmd}";
            process.StartInfo.UseShellExecute = false;
            process.StartInfo.RedirectStandardOutput = true;
            process.StartInfo.RedirectStandardError = true;
            process.StartInfo.CreateNoWindow = true;

            process.Start();
            string output = process.StandardOutput.ReadToEnd();
            string error = process.StandardError.ReadToEnd();
            process.WaitForExit();

            if (!string.IsNullOrEmpty(error))
            {
                Console.Error.Write(error);
            }

            return output;
        }
        catch (Exception ex)
        {
            Console.Error.WriteLine($"Error running command '{cmd}': {ex.Message}");
            return "";
        }
    }

    public string CurrentDateTime()
    {
        return DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss");
    }

    public void ClearDirectory(string dir)
    {
        try
        {
            if (Directory.Exists(dir))
            {
                Directory.Delete(dir, true);
            }
            Directory.CreateDirectory(dir);
        }
        catch (Exception ex)
        {
            Console.Error.WriteLine($"Error clearing directory '{dir}': {ex.Message}");
        }
    }

    public void CreateBackup(string basedir)
    {
        try
        {
            var backupDir = Path.Combine(basedir, "backup");
            var timestamp = DateTime.Now.ToString("yyyyMMdd_HHmmss");
            var backupPath = Path.Combine(backupDir, $"backup_{timestamp}");

            if (!Directory.Exists(backupDir))
            {
                Directory.CreateDirectory(backupDir);
            }

            // Copy directory recursively
            CopyDirectory(basedir, backupPath, true);
            Console.WriteLine($"Backup created at: {backupPath}");
        }
        catch (Exception ex)
        {
            Console.Error.WriteLine($"Error creating backup: {ex.Message}");
        }
    }

    public void RunScript(string scriptPath)
    {
        try
        {
            if (File.Exists(scriptPath))
            {
                var process = new Process();
                process.StartInfo.FileName = scriptPath;
                process.StartInfo.UseShellExecute = false;
                process.StartInfo.RedirectStandardOutput = true;
                process.StartInfo.RedirectStandardError = true;

                process.Start();
                string output = process.StandardOutput.ReadToEnd();
                string error = process.StandardError.ReadToEnd();
                process.WaitForExit();

                if (!string.IsNullOrEmpty(output))
                {
                    Console.Write(output);
                }
                if (!string.IsNullOrEmpty(error))
                {
                    Console.Error.Write(error);
                }
            }
            else
            {
                Console.Error.WriteLine($"Script not found: {scriptPath}");
            }
        }
        catch (Exception ex)
        {
            Console.Error.WriteLine($"Error running script '{scriptPath}': {ex.Message}");
        }
    }

    public void GitCommitPush(string path, string message)
    {
        try
        {
            var originalPath = Directory.GetCurrentDirectory();
            Directory.SetCurrentDirectory(path);

            // Git add all
            RunCommand("git add .");
            
            // Git commit
            RunCommand($"git commit -m \"{message}\"");
            
            // Git push
            RunCommand("git push");

            Directory.SetCurrentDirectory(originalPath);
        }
        catch (Exception ex)
        {
            Console.Error.WriteLine($"Error in git commit/push: {ex.Message}");
        }
    }

    public void CurrentPath(string path)
    {
        try
        {
            Directory.SetCurrentDirectory(path);
        }
        catch (Exception ex)
        {
            Console.Error.WriteLine($"Error changing directory to '{path}': {ex.Message}");
        }
    }

    private void CopyDirectory(string sourceDir, string destinationDir, bool recursive)
    {
        var dir = new DirectoryInfo(sourceDir);
        DirectoryInfo[] dirs = dir.GetDirectories();

        Directory.CreateDirectory(destinationDir);

        foreach (FileInfo file in dir.GetFiles())
        {
            string targetFilePath = Path.Combine(destinationDir, file.Name);
            file.CopyTo(targetFilePath);
        }

        if (recursive)
        {
            foreach (DirectoryInfo subDir in dirs)
            {
                string newDestinationDir = Path.Combine(destinationDir, subDir.Name);
                CopyDirectory(subDir.FullName, newDestinationDir, true);
            }
        }
    }

    public void CopyDirectory(string sourceDir, string targetDir)
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
    public string RunCommand(string command, string arguments)
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

    public void RunProcess(string command, string arguments)
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

    public void CleanDirectory(string dir)
    {
        string olddir = Directory.GetCurrentDirectory();
        Directory.SetCurrentDirectory(dir);
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
        Directory.SetCurrentDirectory(olddir);
    }

    static private bool IsSymlink(string path)
    {
        var fileInfo = new FileInfo(path);
        return fileInfo.Attributes.HasFlag(FileAttributes.ReparsePoint);
    }
} 
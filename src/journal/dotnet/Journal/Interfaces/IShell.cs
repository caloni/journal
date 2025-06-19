namespace Journal.Interfaces;

public interface IShell
{
    void SetupEncoding();
    string RunCommand(string cmd);
    string CurrentDateTime();
    void ClearDirectory(string dir);
    void CreateBackup(string basedir);
    void RunScript(string scriptPath);
    void GitCommitPush(string path, string message);
    void CurrentPath(string path);
    void CopyDirectory(string sourceDir, string targetDir);
    string RunCommand(string command, string arguments);
    void RunProcess(string command, string arguments);
    void CleanDirectory(string dir);
} 
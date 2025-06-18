namespace Journal.Interfaces;

public interface IConfig
{
    bool Parse(string[] args);
    void PrintUsage();
    bool IsBlog();
    bool IsBook();
    bool IsPublish();
    bool IsBackup();
    bool IsGitCommitPushPrivateRepo();
    string GetBasedir();
    string GetScriptdir();
    string GetPrivateDir();
    string GetPrivateRepo();
} 
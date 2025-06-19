using Journal.Interfaces;

namespace Journal;

public class Blog : IBlog
{
    private readonly IShell m_shell;

    public Blog(IShell shell)
    {
        m_shell = shell;
    }

    public int Create(string basedir, string scriptdir)
    {
        var scriptDir = "src";
        string blogPath = Path.Combine("public", "blog");

        Directory.SetCurrentDirectory(basedir);
        Console.WriteLine("basedir: " + basedir);

        if (Directory.Exists(blogPath))
        {
            m_shell.CleanDirectory(blogPath);
        }
        else
        {
            Directory.CreateDirectory(blogPath);
        }

        m_shell.CopyDirectory("blog", blogPath);
        m_shell.CopyDirectory(Path.Combine("img", "blog"), Path.Combine(blogPath, "img"));

        Environment.SetEnvironmentVariable("LC_ALL", "en_US.UTF-8");

        m_shell.RunProcess("gawk", $"-f {Path.Combine(scriptDir, "Util.awk")} -f {Path.Combine(scriptDir, "MetadataParser.awk")} -f {Path.Combine(scriptDir, "MetadataWriter.awk")} journal.md");

        var now = DateTimeOffset.Now;
        var offset = (now.Offset < TimeSpan.Zero ? "-" : "+") + now.Offset.ToString("hhmm");
        var currentDate = now.ToString("yyyy-MM-ddTHH:mm:ss") + offset;
        string buildVersion = m_shell.RunCommand("git", "rev-parse --short HEAD").Trim();

        using (var fs = new FileStream(Path.Combine("public", "metadata.txt"), FileMode.Append, FileAccess.Write))
        using (var writer = new StreamWriter(fs))
        {
            writer.WriteLine($"metadata_current_date {currentDate}");
            writer.WriteLine($"metadata_build_version {buildVersion}");
        }

        m_shell.RunProcess("gawk", $"-f {Path.Combine(scriptDir, "Util.awk")} -f {Path.Combine(scriptDir, "MarkdownParser.awk")} -f {Path.Combine(scriptDir, "BlogWriter.awk")} public/metadata.txt journal.md");

        Console.WriteLine("blog generated");
        return 0;
    }

}
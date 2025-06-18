using Journal.Interfaces;

namespace Journal;

public class Journal : IJournal
{
    private readonly IBlog m_blog;
    private readonly IBook m_book;
    private readonly IConfig m_config;
    private readonly IShell m_shell;

    public Journal(IBlog blog, IBook book, IConfig config, IShell shell)
    {
        m_blog = blog;
        m_book = book;
        m_config = config;
        m_shell = shell;
    }

    public void Run()
    {
        m_shell.SetupEncoding();
        var originalCurrentPath = Directory.GetCurrentDirectory();
        
        try
        {
            Console.WriteLine($"basedir: {m_config.GetBasedir()}");
            m_shell.CurrentPath(m_config.GetBasedir());

            if (m_config.IsBlog())
            {
                if (m_config.IsPublish() && m_config.IsBackup())
                {
                    m_shell.CreateBackup(Directory.GetCurrentDirectory());
                }
                
                m_blog.Create(m_config.GetBasedir(), m_config.GetScriptdir());

                if (m_config.IsGitCommitPushPrivateRepo())
                {
                    m_shell.GitCommitPush(m_config.GetPrivateDir(), "Add journal changes");
                }

                if (m_config.IsPublish())
                {
                    m_shell.GitCommitPush(Directory.GetCurrentDirectory(), "Add journal changes");
                    m_shell.GitCommitPush("public/blog", "Publish latest changes");
                }
            }
            
            if (m_config.IsBook())
            {
                m_book.Create(m_config.GetBasedir(), m_config.GetScriptdir());
            }
        }
        catch (Exception ex)
        {
            Console.Error.WriteLine($"Error: {ex.Message}");
        }
        
        m_shell.CurrentPath(originalCurrentPath);
    }
} 
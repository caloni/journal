namespace Journal.Interfaces;

public interface IBook
{
    int Create(string basedir, string scriptdir, bool includeprivate = true);
} 
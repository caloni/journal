namespace Journal;

class Program
{
    static int Main(string[] args)
    {
        try
        {
            var config = new Config();
            if (!config.Parse(args))
            {
                config.PrintUsage();
                return 1;
            }
            
            var shell = new Shell();
            var blog = new Blog(shell);
            var book = new Book(shell);
            var journal = new Journal(blog, book, config, shell);
            journal.Run();
        }
        catch (Exception ex)
        {
            Console.Error.WriteLine($"Error: {ex.Message}");
            return 1;
        }
        
        return 0;
    }
} 
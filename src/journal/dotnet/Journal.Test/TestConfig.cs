using Xunit;
using Journal;
using Journal.Interfaces;
using Moq;

namespace Journal.Tests;

public class TestConfig
{
    [Fact]
    public void TestParseArguments()
    {
        var config = new Config();
        var args = new[] { "blog", "--publish", "--backup=true" };
        
        var result = config.Parse(args);
        
        Assert.True(result);
        Assert.True(config.IsBlog());
        Assert.False(config.IsBook());
        Assert.True(config.IsPublish());
        Assert.True(config.IsBackup());
    }

    [Fact]
    public void TestParseArgumentsBook()
    {
        var config = new Config();
        var args = new[] { "book", "--publish" };
        
        var result = config.Parse(args);
        
        Assert.True(result);
        Assert.False(config.IsBlog());
        Assert.True(config.IsBook());
        Assert.True(config.IsPublish());
    }

    [Fact]
    public void TestParseArgumentsBoth()
    {
        var config = new Config();
        var args = new[] { "both", "--publish" };
        
        var result = config.Parse(args);
        
        Assert.True(result);
        Assert.True(config.IsBlog());
        Assert.True(config.IsBook());
        Assert.True(config.IsPublish());
    }

    [Fact]
    public void TestParseArgumentsInvalid()
    {
        var config = new Config();
        var args = new[] { "invalid" };
        
        var result = config.Parse(args);
        
        Assert.False(result);
        Assert.False(config.IsBlog());
        Assert.False(config.IsBook());
    }

    [Fact]
    public void TestDefaultConfig()
    {
        var config = new Config();
        var args = new[] { "blog" };
        
        config.Parse(args);
        
        var currentDir = Directory.GetCurrentDirectory();
        Assert.Equal(currentDir, config.GetBasedir());
        Assert.Equal(Path.Combine(currentDir, "src"), config.GetScriptdir());
        Assert.Equal(Path.Combine(currentDir, "private"), config.GetPrivateDir());
        Assert.Equal(Path.Combine(currentDir, "private", ".git"), config.GetPrivateRepo());
    }
} 
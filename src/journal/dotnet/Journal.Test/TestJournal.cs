using Xunit;
using Journal;
using Journal.Interfaces;
using Moq;

namespace Journal.Tests;

public class TestJournal
{
    [Fact]
    public void TestJournalRunWithBlog()
    {
        // Arrange
        var mockBlog = new Mock<IBlog>();
        var mockBook = new Mock<IBook>();
        var mockConfig = new Mock<IConfig>();
        var mockShell = new Mock<IShell>();

        mockConfig.Setup(c => c.IsBlog()).Returns(true);
        mockConfig.Setup(c => c.IsBook()).Returns(false);
        mockConfig.Setup(c => c.IsPublish()).Returns(false);
        mockConfig.Setup(c => c.IsBackup()).Returns(false);
        mockConfig.Setup(c => c.IsGitCommitPushPrivateRepo()).Returns(false);
        mockConfig.Setup(c => c.GetBasedir()).Returns(Directory.GetCurrentDirectory());
        mockConfig.Setup(c => c.GetScriptdir()).Returns(Path.Combine(Directory.GetCurrentDirectory(), "src"));

        mockBlog.Setup(b => b.Create(It.IsAny<string>(), It.IsAny<string>())).Returns(0);

        var journal = new Journal(mockBlog.Object, mockBook.Object, mockConfig.Object, mockShell.Object);

        // Act
        journal.Run();

        // Assert
        mockShell.Verify(s => s.SetupEncoding(), Times.Once);
        mockShell.Verify(s => s.CurrentPath(It.IsAny<string>()), Times.Exactly(2)); // Once to change, once to restore
        mockBlog.Verify(b => b.Create(It.IsAny<string>(), It.IsAny<string>()), Times.Once);
        mockBook.Verify(b => b.Create(It.IsAny<string>(), It.IsAny<string>(), It.IsAny<bool>()), Times.Never);
    }

    [Fact]
    public void TestJournalRunWithBook()
    {
        // Arrange
        var mockBlog = new Mock<IBlog>();
        var mockBook = new Mock<IBook>();
        var mockConfig = new Mock<IConfig>();
        var mockShell = new Mock<IShell>();

        mockConfig.Setup(c => c.IsBlog()).Returns(false);
        mockConfig.Setup(c => c.IsBook()).Returns(true);
        mockConfig.Setup(c => c.GetBasedir()).Returns(Directory.GetCurrentDirectory());
        mockConfig.Setup(c => c.GetScriptdir()).Returns(Path.Combine(Directory.GetCurrentDirectory(), "src"));

        mockBook.Setup(b => b.Create(It.IsAny<string>(), It.IsAny<string>(), It.IsAny<bool>())).Returns(0);

        var journal = new Journal(mockBlog.Object, mockBook.Object, mockConfig.Object, mockShell.Object);

        // Act
        journal.Run();

        // Assert
        mockShell.Verify(s => s.SetupEncoding(), Times.Once);
        mockShell.Verify(s => s.CurrentPath(It.IsAny<string>()), Times.Exactly(2)); // Once to change, once to restore
        mockBlog.Verify(b => b.Create(It.IsAny<string>(), It.IsAny<string>()), Times.Never);
        mockBook.Verify(b => b.Create(It.IsAny<string>(), It.IsAny<string>(), It.IsAny<bool>()), Times.Once);
    }

    [Fact]
    public void TestJournalRunWithBlogAndPublish()
    {
        // Arrange
        var mockBlog = new Mock<IBlog>();
        var mockBook = new Mock<IBook>();
        var mockConfig = new Mock<IConfig>();
        var mockShell = new Mock<IShell>();

        mockConfig.Setup(c => c.IsBlog()).Returns(true);
        mockConfig.Setup(c => c.IsBook()).Returns(false);
        mockConfig.Setup(c => c.IsPublish()).Returns(true);
        mockConfig.Setup(c => c.IsBackup()).Returns(false);
        mockConfig.Setup(c => c.IsGitCommitPushPrivateRepo()).Returns(false);
        mockConfig.Setup(c => c.GetBasedir()).Returns(Directory.GetCurrentDirectory());
        mockConfig.Setup(c => c.GetScriptdir()).Returns(Path.Combine(Directory.GetCurrentDirectory(), "src"));

        mockBlog.Setup(b => b.Create(It.IsAny<string>(), It.IsAny<string>())).Returns(0);

        var journal = new Journal(mockBlog.Object, mockBook.Object, mockConfig.Object, mockShell.Object);

        // Act
        journal.Run();

        // Assert
        mockShell.Verify(s => s.SetupEncoding(), Times.Once);
        mockShell.Verify(s => s.CurrentPath(It.IsAny<string>()), Times.Exactly(2)); // Once to change, once to restore
        mockBlog.Verify(b => b.Create(It.IsAny<string>(), It.IsAny<string>()), Times.Once);
        mockShell.Verify(s => s.GitCommitPush(It.IsAny<string>(), It.IsAny<string>()), Times.Exactly(2)); // Current dir and public/blog
    }

    [Fact]
    public void TestJournalRunWithBlogAndBackup()
    {
        // Arrange
        var mockBlog = new Mock<IBlog>();
        var mockBook = new Mock<IBook>();
        var mockConfig = new Mock<IConfig>();
        var mockShell = new Mock<IShell>();

        mockConfig.Setup(c => c.IsBlog()).Returns(true);
        mockConfig.Setup(c => c.IsBook()).Returns(false);
        mockConfig.Setup(c => c.IsPublish()).Returns(true);
        mockConfig.Setup(c => c.IsBackup()).Returns(true);
        mockConfig.Setup(c => c.IsGitCommitPushPrivateRepo()).Returns(false);
        mockConfig.Setup(c => c.GetBasedir()).Returns(Directory.GetCurrentDirectory());
        mockConfig.Setup(c => c.GetScriptdir()).Returns(Path.Combine(Directory.GetCurrentDirectory(), "src"));

        mockBlog.Setup(b => b.Create(It.IsAny<string>(), It.IsAny<string>())).Returns(0);

        var journal = new Journal(mockBlog.Object, mockBook.Object, mockConfig.Object, mockShell.Object);

        // Act
        journal.Run();

        // Assert
        mockShell.Verify(s => s.SetupEncoding(), Times.Once);
        mockShell.Verify(s => s.CreateBackup(It.IsAny<string>()), Times.Once);
        mockBlog.Verify(b => b.Create(It.IsAny<string>(), It.IsAny<string>()), Times.Once);
    }

    [Fact]
    public void TestJournalRunWithBlogAndPrivateRepo()
    {
        // Arrange
        var mockBlog = new Mock<IBlog>();
        var mockBook = new Mock<IBook>();
        var mockConfig = new Mock<IConfig>();
        var mockShell = new Mock<IShell>();

        mockConfig.Setup(c => c.IsBlog()).Returns(true);
        mockConfig.Setup(c => c.IsBook()).Returns(false);
        mockConfig.Setup(c => c.IsPublish()).Returns(false);
        mockConfig.Setup(c => c.IsBackup()).Returns(false);
        mockConfig.Setup(c => c.IsGitCommitPushPrivateRepo()).Returns(true);
        mockConfig.Setup(c => c.GetBasedir()).Returns(Directory.GetCurrentDirectory());
        mockConfig.Setup(c => c.GetScriptdir()).Returns(Path.Combine(Directory.GetCurrentDirectory(), "src"));
        mockConfig.Setup(c => c.GetPrivateDir()).Returns(Path.Combine(Directory.GetCurrentDirectory(), "private"));

        mockBlog.Setup(b => b.Create(It.IsAny<string>(), It.IsAny<string>())).Returns(0);

        var journal = new Journal(mockBlog.Object, mockBook.Object, mockConfig.Object, mockShell.Object);

        // Act
        journal.Run();

        // Assert
        mockShell.Verify(s => s.SetupEncoding(), Times.Once);
        mockShell.Verify(s => s.GitCommitPush(It.IsAny<string>(), "Add journal changes"), Times.Once);
        mockBlog.Verify(b => b.Create(It.IsAny<string>(), It.IsAny<string>()), Times.Once);
    }
} 
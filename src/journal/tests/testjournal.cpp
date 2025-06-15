#include "doctest.h"
#include "fakeit.hpp"
#include "..\iblog.h"
#include "..\ibook.h"
#include "..\iconfig.h"
#include "..\journal.h"
#include "..\ishell.h"

using namespace fakeit;

TEST_CASE("Do nothing") {
    Mock<IConfig> configMock;
    Mock<IShell> shellMock;
    Mock<IBlog> blogMock;
    Mock<IBook> bookMock;

    Fake(Method(configMock, get_basedir));
    Fake(Method(configMock, get_scriptdir));
    Fake(Method(configMock, is_blog));
    Fake(Method(configMock, is_book));
    Fake(Method(configMock, is_publish));
    Fake(Method(configMock, is_backup));
    Fake(Method(configMock, is_git_commit_push_private_repo));
    Fake(Method(configMock, get_private_dir));
    Fake(Method(shellMock, setup_encoding));
    Fake(Method(shellMock, create_backup));
    Fake(Method(shellMock, git_commit_push));
    Fake(Method(shellMock, current_path));
    Fake(Method(blogMock, create));
    Fake(Method(bookMock, create));

    Journal sut(blogMock.get(), bookMock.get(), configMock.get(), shellMock.get());
    sut.run();
}

TEST_CASE("Create blog") {
    Mock<IConfig> configMock;
    Mock<IShell> shellMock;
    Mock<IBlog> blogMock;
    Mock<IBook> bookMock;

    Fake(Method(configMock, get_basedir));
    Fake(Method(configMock, get_scriptdir));
    When(Method(configMock, is_blog)).AlwaysReturn(true);
    Fake(Method(configMock, is_book));
    Fake(Method(configMock, is_publish));
    Fake(Method(configMock, is_backup));
    Fake(Method(configMock, is_git_commit_push_private_repo));
    Fake(Method(configMock, get_private_dir));
    Fake(Method(shellMock, setup_encoding));
    Fake(Method(shellMock, create_backup));
    Fake(Method(shellMock, git_commit_push));
    Fake(Method(shellMock, current_path));
    Fake(Method(blogMock, create));
    Fake(Method(bookMock, create));

    Journal sut(blogMock.get(), bookMock.get(), configMock.get(), shellMock.get());
    sut.run();

    Verify(Method(shellMock, create_backup)).Never();
    Verify(Method(shellMock, git_commit_push)).Never();
    Verify(Method(blogMock, create)).Once();
    Verify(Method(bookMock, create)).Never();
}

TEST_CASE("Create book") {
    Mock<IConfig> configMock;
    Mock<IShell> shellMock;
    Mock<IBlog> blogMock;
    Mock<IBook> bookMock;

    Fake(Method(configMock, get_basedir));
    Fake(Method(configMock, get_scriptdir));
    Fake(Method(configMock, is_blog));
    When(Method(configMock, is_book)).AlwaysReturn(true);
    Fake(Method(configMock, is_publish));
    Fake(Method(configMock, is_backup));
    Fake(Method(configMock, is_git_commit_push_private_repo));
    Fake(Method(configMock, get_private_dir));
    Fake(Method(shellMock, setup_encoding));
    Fake(Method(shellMock, create_backup));
    Fake(Method(shellMock, git_commit_push));
    Fake(Method(shellMock, current_path));
    Fake(Method(blogMock, create));
    Fake(Method(bookMock, create));

    Journal sut(blogMock.get(), bookMock.get(), configMock.get(), shellMock.get());
    sut.run();

    Verify(Method(shellMock, create_backup)).Never();
    Verify(Method(shellMock, git_commit_push)).Never();
    Verify(Method(blogMock, create)).Never();
    Verify(Method(bookMock, create)).Once();
}

TEST_CASE("Publish blog") {
    Mock<IConfig> configMock;
    Mock<IShell> shellMock;
    Mock<IBlog> blogMock;
    Mock<IBook> bookMock;

    Fake(Method(configMock, get_basedir));
    Fake(Method(configMock, get_scriptdir));
    When(Method(configMock, is_blog)).AlwaysReturn(true);
    Fake(Method(configMock, is_book));
    When(Method(configMock, is_publish)).AlwaysReturn(true);
    Fake(Method(configMock, is_backup));
    Fake(Method(configMock, is_git_commit_push_private_repo));
    Fake(Method(configMock, get_private_dir));
    Fake(Method(shellMock, setup_encoding));
    Fake(Method(shellMock, create_backup));
    Fake(Method(shellMock, git_commit_push));
    Fake(Method(shellMock, current_path));
    Fake(Method(blogMock, create));
    Fake(Method(bookMock, create));

    Journal sut(blogMock.get(), bookMock.get(), configMock.get(), shellMock.get());
    sut.run();

    Verify(Method(shellMock, create_backup)).Never();
    Verify(Method(shellMock, git_commit_push)).Twice();
    Verify(Method(blogMock, create)).Once();
    Verify(Method(bookMock, create)).Never();
}

TEST_CASE("Publish blog and backup") {
    Mock<IConfig> configMock;
    Mock<IShell> shellMock;
    Mock<IBlog> blogMock;
    Mock<IBook> bookMock;

    Fake(Method(configMock, get_basedir));
    Fake(Method(configMock, get_scriptdir));
    When(Method(configMock, is_blog)).AlwaysReturn(true);
    Fake(Method(configMock, is_book));
    When(Method(configMock, is_publish)).AlwaysReturn(true);
    When(Method(configMock, is_backup)).AlwaysReturn(true);
    Fake(Method(configMock, is_git_commit_push_private_repo));
    Fake(Method(configMock, get_private_dir));
    Fake(Method(shellMock, setup_encoding));
    Fake(Method(shellMock, create_backup));
    Fake(Method(shellMock, git_commit_push));
    Fake(Method(shellMock, current_path));
    Fake(Method(blogMock, create));
    Fake(Method(bookMock, create));

    Journal sut(blogMock.get(), bookMock.get(), configMock.get(), shellMock.get());
    sut.run();

    Verify(Method(shellMock, create_backup)).Once();
    Verify(Method(shellMock, git_commit_push)).Twice();
    Verify(Method(blogMock, create)).Once();
    Verify(Method(bookMock, create)).Never();
}

TEST_CASE("Publish private notes") {
    Mock<IConfig> configMock;
    Mock<IShell> shellMock;
    Mock<IBlog> blogMock;
    Mock<IBook> bookMock;

    Fake(Method(configMock, get_basedir));
    Fake(Method(configMock, get_scriptdir));
    When(Method(configMock, is_blog)).AlwaysReturn(true);
    Fake(Method(configMock, is_book));
    When(Method(configMock, is_publish)).AlwaysReturn(true);
    When(Method(configMock, is_backup)).AlwaysReturn(true);
    When(Method(configMock, is_git_commit_push_private_repo)).AlwaysReturn(true);
    Fake(Method(configMock, get_private_dir));
    Fake(Method(shellMock, setup_encoding));
    Fake(Method(shellMock, create_backup));
    Fake(Method(shellMock, git_commit_push));
    Fake(Method(shellMock, current_path));
    Fake(Method(blogMock, create));
    Fake(Method(bookMock, create));

    Journal sut(blogMock.get(), bookMock.get(), configMock.get(), shellMock.get());
    sut.run();

    Verify(Method(shellMock, create_backup)).Once();
    Verify(Method(shellMock, git_commit_push)).Exactly(3_Times);
    Verify(Method(blogMock, create)).Once();
    Verify(Method(bookMock, create)).Never();
}


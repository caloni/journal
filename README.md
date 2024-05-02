# Write for people, computers and food.

Hi. This is the source from my [blogue] posts published using [GitHub Pages], my ebook and other texts published elsewhere.

This repo should stay partially private. It contains work and personal journal and sensitive data. The public_branch branch can be published as public. The private_branch not.

How to do that?

```
git init # or just use existing and rename master
gvim journal.txt journal_private.txt
# write write write
git add --all
git commit -m "first private commit"
git branch -m master private_branch
git checkout --orphan public_branch
git checkout private_branch -- .
git reset
echo private_stuff >> .gitignore
git add --all
git commit -m "first public commit."
git checkout private_branch
git merge --allow-unrelated-histories -X ours public_branch
git status

On branch private_branch
nothing to commit, working tree clean

git log --graph
 private_branch)
|\  Merge: de55682 5dbe679

| | Date:   Wed May 1 21:39:01 2024 -0300
| |
| |     Merge branch 'public_branch' into private_branch
| |
| * commit 5dbe679aa2466eeaedd1a675a09b4913b56f2270 (public_branch)

|   Date:   Wed May 1 21:38:10 2024 -0300
|
|       first public commit.
|
* commit de55682a3ce3ce55114b05ecf466c664e60f11c6

  Date:   Wed May 1 21:37:01 2024 -0300

      first private commit.

git remote add private <private_address>
git push -u private private_branch
git remote add public <public_address>
git push -u public public_branch
# do not mix public and private upstreams =)
```

The public folder will contain publishable content, but I am publishing my ebook with the work journal, so I think this will depend of the public =).

If you just cloned this repo start by reading some scripts.  I used an operating system called Windows, but it is is pretty workable from Unix env, since uses gawk and stuff. You guys are using Unix stuff yet, right?

Thank you, person from the future, for the interest into my texts. Hope you find something useful here. Bye bye and thank you for all the steak.

[blogue]: https://caloni.com.br
[GitHub Pages]: https://github.com/Caloni/caloni.github.io
